#ifndef __ARM_H__
#define __ARM_H__
#include <Arduino.h>
#include <buffer/buffer.h>
#include <Adafruit_PWMServoDriver.h>

typedef unsigned char uchar;

constexpr size_t PULSE_MS_MIN = 600;
constexpr size_t PULSE_MS_MAX = 2500;
constexpr uchar SERVO_FREQ = 50;

Adafruit_PWMServoDriver pwm;

constexpr uchar SERVOS = 6;

constexpr uchar MIN_ANGLES[] = {5, 40, 0, 70, 0, 60};
constexpr uchar MAX_ANGLES[] = {165, 150, 130, 180, 180, 115};
constexpr uchar DEF_ANGLES[] = {90, 140, 120, 90, 90, 80};

constexpr int SERVO_BACKWARD = -1;
constexpr int SERVO_STOP = 0;
constexpr int SERVO_FORWARD = 1;
constexpr size_t SERVO_TIMEOUT = 20;

int directions[] = {SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP, SERVO_STOP};
uchar currentAngles[SERVOS];

constexpr uchar BASE_PIN = 0;
constexpr uchar SHOULDER_PIN = 1;
constexpr uchar ELBOW_1_PIN = 2;
constexpr uchar ELBOW_2_PIN = 3;
constexpr uchar WRIST_PIN = 4;
constexpr uchar CLAW_PIN = 5;


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


#endif // __ARM_H__