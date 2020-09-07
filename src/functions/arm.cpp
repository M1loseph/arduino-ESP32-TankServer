#include "arm.h"

Adafruit_PWMServoDriver pwm;

void change_servo_direction(uchar servo, const Integer &number)
{
  if (number.success && servo < SERVOS)
  {
    int direction = number.value;
    if (direction == SERVO_STOP || direction == SERVO_BACKWARD || direction == SERVO_FORWARD)
      directions[servo] = direction;
  }
}

void update_servos_movement()
{
  static unsigned long timer = millis();
  if (millis() - timer > SERVO_TIMEOUT)
  {
    for (size_t i = 0; i < SERVOS; i++)
    {
      bool changed = false;
      if (directions[i] == SERVO_FORWARD && current_angles[i] < MAX_ANGLES[i])
      {
        current_angles[i]++;
        changed = true;
      }
      else if (directions[i] == SERVO_BACKWARD && current_angles[i] > MIN_ANGLES[i])
      {

        current_angles[i]--;
        changed = true;
      }
      if (changed)
      {
        int pulse = (int)map(current_angles[i], 0, 180, PULSE_MS_MIN, PULSE_MS_MAX);
        pwm.writeMicroseconds(i, pulse);
      }
    }
    timer = millis();
  }
}
