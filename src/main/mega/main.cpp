// main file, don't compile with unit tests
#ifndef UNIT_TEST
#ifdef MEGA

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "parser/parser.h"
#include "commands.h"

typedef unsigned char uchar;

// ================
// VARIABLES
// ================

constexpr uchar MP3_RX = 11;
constexpr uchar MP3_TX = 12;
// PWM library shit
constexpr size_t PULSE_MS_MIN = 550;
constexpr size_t PULSE_MS_MAX = 2500;
constexpr uchar SERVO_FREQ = 50;

Parser parser;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

constexpr uchar SERVOS = 6;

constexpr uchar MIN_ANGLES[] = {5, 40, 0, 70, 0, 60};
constexpr uchar MAX_ANGLES[] = {165, 150, 130, 180, 180, 115};
constexpr uchar DEF_ANGLES[] = {90, 140, 120, 90, 90, 80};

constexpr uchar SERVO_BACKWARD = 0;
constexpr uchar SERVO_STOP = 1;
constexpr uchar SERVO_FORWARD = 2;
constexpr uchar SERVO_TIMEOUT = 20;

uchar DIRECTIONS[] = {SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP};
uchar CURRENT_ANGLES[SERVOS];

constexpr uchar BASE_PIN = 0;
constexpr uchar SHOULDER_PIN = 1;
constexpr uchar ELBOW_1_PIN = 2;
constexpr uchar ELBOW_2_PIN = 3;
constexpr uchar WRIST_PIN = 4;
constexpr uchar CLAW_PIN = 5;

constexpr size_t TEMPERATURE_TIMEOUT = 500; // timeout in milliseconds
constexpr uchar ONE_WIRE_BUS = 13;
constexpr size_t TEMPERATURE_RESOLUTION = 9;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress inside, outside;

bool awaitingForState = false;
// needs to be init in setup
unsigned long temperatureCheck;

// ==============
// MP3 FUNCTIONS
// ==============

void MP3command(int8_t command, int8_t data_one, int8_t data_two)
{
  int8_t Send_buf[8];
  Send_buf[0] = 0x7e;             //starting byte
  Send_buf[1] = 0xff;             //version
  Send_buf[2] = 0x06;             //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command;          //
  Send_buf[4] = 0x00;             //0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = data_one;         //datah
  Send_buf[6] = data_two;         //datal
  Send_buf[7] = 0xef;             //ending byte
  for (uint8_t i = 0; i < 8; i++) //
    Serial3.write(Send_buf[i]);
}

void readMP3FromBuffer(const CommandBuffer &buffer)
{
  static constexpr size_t NUMBERS = 3U;
  Integer numbers[NUMBERS];
  for (size_t i = 0; i < NUMBERS; i++)
    numbers[i] = buffer.IntAt(i + 1);

  // check the success of each number
  // later check if each of them is 0 <= value <= 255
  for (size_t i = 0; i < NUMBERS; i++)
    if (!numbers[i].success || numbers[i].value < 0 || numbers[i].value > 255)
      return;

  MP3command(numbers[0].value, numbers[1].value, numbers[2].value);
}

// ==============
//  SERVOS
// ==============

void changeServoDirection(uchar servo, const Integer &number)
{
  if (number.success && servo < SERVOS)
  {
    int direction = number.value;
    if (direction == SERVO_STOP || direction == SERVO_BACKWARD || direction == SERVO_FORWARD)
      DIRECTIONS[servo] = direction;
  }
}

void updateServos()
{
  static unsigned long timer = millis();
  if (millis() - timer > SERVO_TIMEOUT)
  {
    for (size_t i = 0; i < SERVOS; i++)
    {
      bool changed = false;
      if (DIRECTIONS[i] == SERVO_FORWARD && CURRENT_ANGLES[i] < MAX_ANGLES[i])
      {
        CURRENT_ANGLES[i]++;
        changed = true;
      }
      else if (DIRECTIONS[i] == SERVO_BACKWARD && CURRENT_ANGLES[i] > MIN_ANGLES[i])
      {

        CURRENT_ANGLES[i]--;
        changed = true;
      }
      if (changed)
      {
        int pulse = (int)map(CURRENT_ANGLES[i], 0, 180, PULSE_MS_MIN, PULSE_MS_MAX);
        pwm.writeMicroseconds(i, pulse);
      }
    }
    timer = millis();
  }
}

void requestState(const CommandBuffer &buffer)
{
  Serial.println("Requesting state...");
  // to make sure we dont get flooded with requests
  if (!awaitingForState)
  {
    // we need to ask termometers for temperature
    sensors.requestTemperatures();
    awaitingForState = true;
    temperatureCheck = millis();
  }
}

void sendState()
{
  // if ${TEMPERATURE_TIMEOUT} has passed and we want a timeout
  if (awaitingForState && millis() - temperatureCheck > TEMPERATURE_TIMEOUT)
  {
    int tempOutside = sensors.getTempC(outside);

    // time for sending current state
    Serial.print(tempOutside);
    Serial.write(' ');

    // sevo positions
    for(size_t i = 0; i < SERVOS; i++)
    {
      Serial.print(CURRENT_ANGLES[i]);
      Serial.write(' ');
    }

    awaitingForState = false;
  }
}

// ==============
// Parser functions
// ==============

auto BASE_FUN = [](const CommandBuffer &b) { changeServoDirection(BASE_PIN, b.IntAt(1)); };
auto SHOULDER_FUN = [](const CommandBuffer &b) { changeServoDirection(SHOULDER_PIN, b.IntAt(1)); };
auto ELBOW_1_FUN = [](const CommandBuffer &b) { changeServoDirection(ELBOW_1_PIN, b.IntAt(1)); };
auto ELBOW_2_FUN = [](const CommandBuffer &b) { changeServoDirection(ELBOW_2_PIN, b.IntAt(1)); };
auto WRIST_FUN = [](const CommandBuffer &b) { changeServoDirection(WRIST_PIN, b.IntAt(1)); };
auto CLAW_FUN = [](const CommandBuffer &b) { changeServoDirection(CLAW_PIN, b.IntAt(1)); };

void setup()
{
  Serial.begin(115200);
  Serial3.begin(9600);
  MP3command(6, 0, 15);

  sensors.begin();
  sensors.getAddress(outside, 0);

  // resolution and NO waiting for conversion
  sensors.setResolution(TEMPERATURE_RESOLUTION);
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();
  temperatureCheck = millis();

  LOG(sensors.getDeviceCount());
  LOG(sensors.getResolution(outside));

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates

  for (size_t i = 0; i < SERVOS; i++)
  {
    pwm.writeMicroseconds(i, map(DEF_ANGLES[i], 0, 180, PULSE_MS_MIN, PULSE_MS_MAX));
    CURRENT_ANGLES[i] = DEF_ANGLES[i];
  }

  parser.AddEvents(Command::Mega::BASE, BASE_FUN);
  parser.AddEvents(Command::Mega::SHOULDER, SHOULDER_FUN);
  parser.AddEvents(Command::Mega::ELBOW_1, ELBOW_1_FUN);
  parser.AddEvents(Command::Mega::ELBOW_2, ELBOW_2_FUN);
  parser.AddEvents(Command::Mega::WRIST, WRIST_FUN);
  parser.AddEvents(Command::Mega::CLAW, CLAW_FUN);
  parser.AddEvents(Command::Mega::MP3_COMMAND, readMP3FromBuffer);
  parser.AddEvents(Command::Mega::STATE, requestState);
}

void loop()
{
  if (parser.ReadStream(&Serial))
    parser.ExecuteMessege();
  updateServos();
  sendState();
}

#endif // MEGA
#endif // UNIT_TEST