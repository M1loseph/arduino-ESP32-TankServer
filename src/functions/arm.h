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
        extern const char *SET_ANGLE;
        extern const char *BASE;
        extern const char *SHOULDER;
        extern const char *ELBOW;
        extern const char *WRIST;
        extern const char *ROTATION;
        extern const char *CLAW;

        namespace modifiers
        {
            extern const char *MINUS;
            extern const char *PLUS;
            extern const char *STOP;
        } // namespace modifiers
    }     // namespace commands

    enum class servo_direction
    {
        MINUS,
        PLUS,
        STOP
    };

    typedef struct
    {
        const uchar MIN_ANGLE;
        const uchar MAX_ANGLE;
        uchar current_angle;
        uchar pin;
        servo_direction direction;
    } servo_data;

    extern Adafruit_PWMServoDriver pwm;
    extern servo_data SERVOS[];

    void init_arm();

    void move_base(const CommandBuffer &b);
    void move_shoulder(const CommandBuffer &b);
    void move_elbow(const CommandBuffer &b);
    void move_wrist(const CommandBuffer &b);
    void move_rotation(const CommandBuffer &b);
    void move_claw(const CommandBuffer &b);

    void update_servos_movement();

} // namespace arm

#endif // __ARM_H__