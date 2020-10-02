#ifndef __ARM_H__
#define __ARM_H__

#include <Arduino.h>
#include <buffer/buffer.h>
#include <Adafruit_PWMServoDriver.h>
#include "typedefs.h"

namespace arm
{
    namespace commands
    {
        extern const char *SERVO_MINUS;
        extern const char *SERVO_PLUS;
        extern const char *SERVO_STOP;
        extern const char *SERVO_ANGLE;
    } // namespace commands

    typedef struct
    {
        const uchar MIN_ANGLE;
        const uchar MAX_ANGLE;
        uchar current_angle;
        uchar destination_angle;
        uchar extern_module_pin;
    } servo_data;

    extern Adafruit_PWMServoDriver pwm;

    void init_arm();

    void arm_minus(const CommandBuffer &b);
    void arm_plus(const CommandBuffer &b);
    void arm_stop(const CommandBuffer &b);
    void arm_angle(const CommandBuffer &b);

    void update_servos_movement();

} // namespace arm

#endif // __ARM_H__