#ifndef __ARM_H__
#define __ARM_H__

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_PWMServoDriver.h>
#include "abstract/templated_controller.hpp"

namespace json_parser
{
    class arm_controller final : public templated_controller<arm_controller>
    {
    public:
        explicit arm_controller();

        void update() override;
        bool initialize() override;

    private:
        typedef struct
        {
            const char *const NAME;
            const uint8_t MIN_ANGLE;
            const uint8_t MAX_ANGLE;
            uint8_t current_angle;
            uint8_t destination_angle;
            uint8_t extern_module_pin;
        } servo_data;

        bool servo_minus(const JsonObjectConst *json);
        bool servo_plus(const JsonObjectConst *json);
        bool servo_stop(const JsonObjectConst *json);
        bool servo_angle(const JsonObjectConst *json);

        void send_angle(uint8_t index);
        servo_data *get_servo_ptr(const JsonObjectConst *json);

        void update_servos_movement();

        static constexpr const char *SERVO_MINUS = "MINUS";
        static constexpr const char *SERVO_PLUS = "PLUS";
        static constexpr const char *SERVO_STOP = "STOP";
        static constexpr const char *SERVO_ANGLE = "ANGLE";
        static constexpr uint8_t SERVOS = 6;

        static constexpr uint32_t PULSE_MS_MIN = 600U;
        static constexpr uint32_t PULSE_MS_MAX = 2500U;
        static constexpr uint8_t PULSES_FREQUENCY = 50U;
        static constexpr uint32_t SERVO_TIMEOUT = 20U;

        static constexpr const char *NAME_KEY = "servo";
        static constexpr const char *ANGLE_KEY = "angle";

        Adafruit_PWMServoDriver _pwm;

        servo_data arm[SERVOS] = {
            servo_data{"BASE", 5, 165, 90, 90, 0},
            servo_data{"SHOULDER", 40, 150, 90, 90, 3},
            servo_data{"ELBOW", 0, 130, 90, 90, 7},
            servo_data{"WRIST", 70, 180, 90, 90, 8},
            servo_data{"ROTATION", 0, 180, 90, 90, 12},
            servo_data{"CLAW", 60, 115, 80, 80, 11},
        };
    };
} // namespace json_parser

#endif // __ARM_H__