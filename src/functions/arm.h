#ifndef __ARM_H__
#define __ARM_H__

#include <Arduino.h>
#include <buffer/buffer.h>
#include <Adafruit_PWMServoDriver.h>
#include "typedefs.h"

constexpr size_t PULSE_MS_MIN = 600;
constexpr size_t PULSE_MS_MAX = 2500;
constexpr uchar SERVO_FREQ = 50;

constexpr uchar SERVOS = 6;

constexpr uchar MIN_ANGLES[] = {5, 40, 0, 70, 0, 60};
constexpr uchar MAX_ANGLES[] = {165, 150, 130, 180, 180, 115};
constexpr uchar DEF_ANGLES[] = {90, 90, 90, 90, 90, 80};

constexpr int SERVO_BACKWARD = -1;
constexpr int SERVO_STOP = 0;
constexpr int SERVO_FORWARD = 1;
constexpr size_t SERVO_TIMEOUT = 20;

constexpr uchar BASE_PIN = 0;
constexpr uchar SHOULDER_PIN = 1;
constexpr uchar ELBOW_1_PIN = 2;
constexpr uchar ELBOW_2_PIN = 3;
constexpr uchar WRIST_PIN = 4;
constexpr uchar CLAW_PIN = 5;

extern int directions[SERVOS];
extern uchar current_angles[SERVOS];
extern Adafruit_PWMServoDriver pwm;

void change_servo_direction(uchar servo, const Integer &number);
void update_servos_movement();

#endif // __ARM_H__