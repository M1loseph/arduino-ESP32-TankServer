// main file, don't compile with unit tests
#ifndef UNIT_TEST
#ifdef NANO

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include "parser/parser.h"
#include "debug.h"
#include "commands.h"

typedef unsigned char uchar;
void moveServo(uchar servo, const Number &number);

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
SoftwareSerial mp3(MP3_RX, MP3_TX);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

constexpr uchar MIN[] = {0, 30, 0, 0, 0, 40};
constexpr uchar MAX[] = {180, 180, 180, 180, 180, 60};
constexpr uchar DEF_ANGLES[] = {90, 90, 90, 90, 90, 50};

constexpr uchar SERVOS = 6;

constexpr uchar BASE_PIN = 0;
constexpr uchar SHOULDER_PIN = 1;
constexpr uchar ELBOW_1_PIN = 2;
constexpr uchar ELBOW_2_PIN = 3;
constexpr uchar WRIST_PIN = 4;
constexpr uchar CLAW_PIN = 5;

auto BASE_FUN = [](const CommandBuffer &b) { moveServo(BASE_PIN, b.FindNumber(1)); };
auto SHOULDER_FUN = [](const CommandBuffer &b) { moveServo(SHOULDER_PIN, b.FindNumber(1)); };
auto ELBOW_1_FUN = [](const CommandBuffer &b) { moveServo(ELBOW_1_PIN, b.FindNumber(1)); };
auto ELBOW_2_FUN = [](const CommandBuffer &b) { moveServo(ELBOW_2_PIN, b.FindNumber(1)); };
auto WRIST_FUN = [](const CommandBuffer &b) { moveServo(WRIST_PIN, b.FindNumber(1)); };
auto CLAW_FUN = [](const CommandBuffer &b) { moveServo(CLAW_PIN, b.FindNumber(1)); };

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
    mp3.write(Send_buf[i]);
}

void readMP3FromBuffer(const CommandBuffer &buffer)
{
  static constexpr size_t NUMBERS = 3U;
  Number numbers[NUMBERS];
  for (size_t i = 0; i < NUMBERS; i++)
    numbers[i] = buffer.FindNumber(i + 1);

  // check the success of each number
  // later check if each of them is 0 <= value <= 255
  for (size_t i = 0; i < NUMBERS; i++)
    if (!numbers[i].success || numbers[i].value < 0 || numbers[i].value > 255)
      return;

  MP3command(numbers[0].value, numbers[1].value, numbers[2].value);
}

void moveServo(uchar servo, const Number &number)
{
  if (number.success && servo < SERVOS)
  {
    int pulse = number.value;
    int angle = (int)map(pulse, PULSE_MS_MIN, PULSE_MS_MAX, 0, 180);
    // check if value is in bounds
    if (angle >= MIN[servo] && angle <= MAX[servo])
      pwm.writeMicroseconds(servo, pulse);
  }
}

void setup()
{
  Serial.begin(115200);
  // mp3.begin(9600);
  // MP3command(6, 0, 15);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates

  parser.AddEvents(Command::Nano::BASE, BASE_FUN);
  parser.AddEvents(Command::Nano::SHOULDER, SHOULDER_FUN);
  parser.AddEvents(Command::Nano::ELBOW_1, ELBOW_1_FUN);
  parser.AddEvents(Command::Nano::ELBOW_2, ELBOW_2_FUN);
  parser.AddEvents(Command::Nano::WRIST, WRIST_FUN);
  parser.AddEvents(Command::Nano::CLAW, CLAW_FUN);

  for (size_t i = 0; i < SERVOS; i++)
    pwm.writeMicroseconds(i, map(DEF_ANGLES[i], 0, 180, PULSE_MS_MIN, PULSE_MS_MAX));
}

void loop()
{
  if (parser.ReadStream(&Serial)){
    for(uint16_t i = 0; i < parser.GetBuff().Length(); i++)
      Serial.write(parser.GetBuff().C_Ptr()[i]);
    
    parser.ExecuteMessege();
  }
}

#endif // NANO
#endif // UNIT_TEST