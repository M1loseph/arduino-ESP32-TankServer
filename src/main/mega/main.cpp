// main file, don't compile with unit tests
#ifndef UNIT_TEST
#ifdef MEGA

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <I2Cdev.h>
#include <MPU6050.h>

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

uchar directions[] = {SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP};
uchar currentAngles[SERVOS];

constexpr uchar BASE_PIN = 0;
constexpr uchar SHOULDER_PIN = 1;
constexpr uchar ELBOW_1_PIN = 2;
constexpr uchar ELBOW_2_PIN = 3;
constexpr uchar WRIST_PIN = 4;
constexpr uchar CLAW_PIN = 5;

constexpr size_t TEMPERATURE_TIMEOUT = 1000; // timeout in milliseconds
constexpr uchar ONE_WIRE_BUS = 13;
constexpr size_t TEMPERATURE_RESOLUTION = 11;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress inside, outside;

bool awaitingForState = false;
// needs to be init in setup
unsigned long lastTemperatureCheck;

// gyro and acc sensor
MPU6050 accelgyro;

// distance sensor
constexpr uchar TRIG = 31;
constexpr uchar ECHO = 33;

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

void ReadMP3FromBuffer(const CommandBuffer &buffer)
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

void ChangeServoDirection(uchar servo, const Integer &number)
{
  if (number.success && servo < SERVOS)
  {
    int direction = number.value;
    if (direction == SERVO_STOP || direction == SERVO_BACKWARD || direction == SERVO_FORWARD)
      directions[servo] = direction;
  }
}

void UpdateServos()
{
  static unsigned long timer = millis();
  if (millis() - timer > SERVO_TIMEOUT)
  {
    for (size_t i = 0; i < SERVOS; i++)
    {
      bool changed = false;
      if (directions[i] == SERVO_FORWARD && currentAngles[i] < MAX_ANGLES[i])
      {
        currentAngles[i]++;
        changed = true;
      }
      else if (directions[i] == SERVO_BACKWARD && currentAngles[i] > MIN_ANGLES[i])
      {

        currentAngles[i]--;
        changed = true;
      }
      if (changed)
      {
        int pulse = (int)map(currentAngles[i], 0, 180, PULSE_MS_MIN, PULSE_MS_MAX);
        pwm.writeMicroseconds(i, pulse);
      }
    }
    timer = millis();
  }
}

size_t ReadDistance()
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long duration = pulseIn(ECHO, HIGH);
  return (size_t)(duration * 0.034 / 2);
}

void RequestState(const CommandBuffer &buffer)
{
  // to make sure we dont get flooded with requests
  if (!awaitingForState)
  {
    // we need to ask termometers for temperature
    sensors.requestTemperatures();
    awaitingForState = true;
    lastTemperatureCheck = millis();
  }
}

void SendState()
{
  static const size_t stateLength = strlen(Command::Common::STATE);

  // if ${TEMPERATURE_TIMEOUT} has passed and we want a timeout
  if (awaitingForState && millis() - lastTemperatureCheck > TEMPERATURE_TIMEOUT)
  {
    float tempOutside = sensors.getTempC(outside);
    float tempInside = sensors.getTempC(inside);

    int16_t resoults[6];
    size_t distance = ReadDistance();
    accelgyro.getMotion6(resoults, resoults + 1, resoults + 2, resoults + 3, resoults + 4, resoults + 5);

    // we dont want a null character
    for (size_t i = 0; i < stateLength; i++)
      Serial.write(Command::Common::STATE[i]);

    Serial.write(' ');
    // time for sending current state
    Serial.print(tempOutside);
    Serial.write(' ');
    Serial.print(tempInside);
    Serial.print(' ');

    // sevo positions
    for (size_t i = 0; i < SERVOS; i++)
    {
      Serial.print(currentAngles[i]);
      Serial.write(' ');
    }

    for (size_t i = 0; i < 6; i++)
    {
      Serial.print(resoults[i]);
      Serial.write(' ');
    }

    Serial.print(distance);
    // to ensure the communication ends here
    Serial.write('\n');
    awaitingForState = false;
  }
}

void SendDistance(const CommandBuffer &b)
{
  static const size_t distanceLength = strlen(Command::Common::DISTANCE);
  size_t distance = ReadDistance();

  for (size_t i = 0; i < distanceLength; i++)
    Serial.write(Command::Common::DISTANCE[i]);

  Serial.write(' ');

  Serial.print(distance);
  // to ensure the communication ends here
  Serial.write('\n');
}

void SetIntervalFromOutside(const CommandBuffer &b)
{
  const char *command = b.WordAt(1);
  Integer interval = b.IntAt(2);
  if (command && interval.success && interval.value >= 0)
    parser.SetInterval(command, interval.value);
}

// ==============
// Parser functions
// ==============

auto BASE_FUN = [](const CommandBuffer &b) { ChangeServoDirection(BASE_PIN, b.IntAt(1)); };
auto SHOULDER_FUN = [](const CommandBuffer &b) { ChangeServoDirection(SHOULDER_PIN, b.IntAt(1)); };
auto ELBOW_1_FUN = [](const CommandBuffer &b) { ChangeServoDirection(ELBOW_1_PIN, b.IntAt(1)); };
auto ELBOW_2_FUN = [](const CommandBuffer &b) { ChangeServoDirection(ELBOW_2_PIN, b.IntAt(1)); };
auto WRIST_FUN = [](const CommandBuffer &b) { ChangeServoDirection(WRIST_PIN, b.IntAt(1)); };
auto CLAW_FUN = [](const CommandBuffer &b) { ChangeServoDirection(CLAW_PIN, b.IntAt(1)); };

void setup()
{
  Serial.begin(115200);
  Serial3.begin(9600);
  MP3command(6, 0, 15);

  sensors.begin();
  sensors.getAddress(outside, 0);
  sensors.getAddress(inside, 1);
  // resolution and NO waiting for conversion
  sensors.setResolution(TEMPERATURE_RESOLUTION);
  sensors.setWaitForConversion(false);
  lastTemperatureCheck = millis();

  LOG(sensors.getDeviceCount());
  LOG(sensors.getResolution(outside));

  Wire.begin();
  accelgyro.initialize();

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates

  for (size_t i = 0; i < SERVOS; i++)
  {
    pwm.writeMicroseconds(i, map(DEF_ANGLES[i], 0, 180, PULSE_MS_MIN, PULSE_MS_MAX));
    currentAngles[i] = DEF_ANGLES[i];
  }

  parser.AddEvents(Command::Mega::INTERVAL, SetIntervalFromOutside);
  parser.AddEvents(Command::Mega::BASE, BASE_FUN);
  parser.AddEvents(Command::Mega::SHOULDER, SHOULDER_FUN);
  parser.AddEvents(Command::Mega::ELBOW_1, ELBOW_1_FUN);
  parser.AddEvents(Command::Mega::ELBOW_2, ELBOW_2_FUN);
  parser.AddEvents(Command::Mega::WRIST, WRIST_FUN);
  parser.AddEvents(Command::Mega::CLAW, CLAW_FUN);
  parser.AddEvents(Command::Mega::MP3_COMMAND, ReadMP3FromBuffer);
  parser.AddEvents(Command::Common::STATE, RequestState);
  parser.AddEvents(Command::Common::DISTANCE, SendDistance);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIG, LOW);
}

void loop()
{
  if (parser.ReadStream(&Serial))
    parser.ExecuteMessege();
  parser.ExecuteInterval();
  UpdateServos();
  SendState();
}

#endif // MEGA
#endif // UNIT_TEST