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

    public:
        explicit arm_controller();

        bool initialize() override;
        void update() override;
        DynamicJsonDocument retrive_data() override;
        servo_data* get_servo_by_name(const char* servo_name);

    private:
        bool servo_minus(const JsonObjectConst *json);
        bool servo_plus(const JsonObjectConst *json);
        bool servo_stop(const JsonObjectConst *json);
        bool servo_angle(const JsonObjectConst *json);

        void send_angle(uint8_t index);
        servo_data *get_servo_ptr(const JsonObjectConst *json);

        static constexpr const char *SERVO_MINUS = "minus";
        static constexpr const char *SERVO_PLUS = "plus";
        static constexpr const char *SERVO_STOP = "stop";
        static constexpr const char *SERVO_ANGLE = "angle";
        static constexpr uint8_t SERVOS = 6;

        static constexpr uint32_t PULSE_MS_MIN = 600U;
        static constexpr uint32_t PULSE_MS_MAX = 2500U;
        static constexpr uint8_t PULSES_FREQUENCY = 50U;
        static constexpr uint32_t SERVO_TIMEOUT = 20U;

        static constexpr const char *NAME_KEY = "servo";
        static constexpr const char *ANGLE_KEY = "angle";

        Adafruit_PWMServoDriver _pwm;

        servo_data arm[SERVOS] = {
            servo_data{"base", 5, 165, 90, 90, 0},
            servo_data{"shoulder", 40, 150, 90, 90, 3},
            servo_data{"elbow", 0, 130, 90, 90, 7},
            servo_data{"wrist", 70, 180, 90, 90, 8},
            servo_data{"rotation", 0, 180, 90, 90, 12},
            servo_data{"claw", 60, 115, 80, 80, 11},
        };
    };
} // namespace json_parser

#endif // __ARM_H__