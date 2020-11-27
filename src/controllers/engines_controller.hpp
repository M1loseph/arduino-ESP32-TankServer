#ifndef __ENGINES_H__
#define __ENGINES_H__

#include <Arduino.h>
#include "abstract/templated_controller.hpp"

namespace json_parser
{
    class engines_controller final : public templated_controller<engines_controller>
    {
    public:

        explicit engines_controller();
        void update() override;
        bool initialize() override;

        enum class speed_controll
        {
            SLOWER,
            FASTER,
            KEEP_SPEED
        };

        enum class engine_direction
        {
            BACKWARD,
            FORWARD,
            STOP
        };

    private:
        void disable_speed_left();
        void enable_speed_left();
        void disable_speed_right();
        void enable_speed_right();

        bool forward(const JsonObjectConst *json);
        bool forward_left(const JsonObjectConst *json);
        bool forward_right(const JsonObjectConst *json);

        bool backward(const JsonObjectConst *json);
        bool backward_left(const JsonObjectConst *json);
        bool backward_right(const JsonObjectConst *json);

        bool stop(const JsonObjectConst *json);
        bool stop_left(const JsonObjectConst *json);
        bool stop_right(const JsonObjectConst *json);

        bool rotate_left(const JsonObjectConst *json);
        bool rotate_right(const JsonObjectConst *json);

        bool slower(const JsonObjectConst *json);
        bool slower_left(const JsonObjectConst *json);
        bool slower_right(const JsonObjectConst *json);

        bool faster(const JsonObjectConst *json);
        bool faster_left(const JsonObjectConst *json);
        bool faster_right(const JsonObjectConst *json);

        bool keep_speed(const JsonObjectConst *json);
        bool keep_speed_left(const JsonObjectConst *json);
        bool keep_speed_right(const JsonObjectConst *json);

        bool set_speed(const JsonObjectConst *json);
        bool set_speed_left(const JsonObjectConst *json);
        bool set_speed_right(const JsonObjectConst *json);

        size_t get_speed_from_json(const JsonObjectConst *json, bool *succ);

        static constexpr const char *FORWARD = "FORWARD";
        static constexpr const char *FORWARD_LEFT = "FORWARD_LEFT";
        static constexpr const char *FORWARD_RIGHT = "FORWARD_RIGHT";
        static constexpr const char *BACKWARD = "BACKWARD";
        static constexpr const char *BACKWARD_LEFT = "BACKWARD_LEFT";
        static constexpr const char *BACKWARD_RIGHT = "BACKWARD_RIGHT";
        static constexpr const char *STOP = "STOP";
        static constexpr const char *STOP_LEFT = "STOP_LEFT";
        static constexpr const char *STOP_RIGHT = "STOP_RIGHT";
        static constexpr const char *FASTER = "FASTER";
        static constexpr const char *FASTER_LEFT = "FASTER_LEFT";
        static constexpr const char *FASTER_RIGHT = "FASTER_RIGHT";
        static constexpr const char *SLOWER = "SLOWER";
        static constexpr const char *SLOWER_LEFT = "SLOWER_LEFT";
        static constexpr const char *SLOWER_RIGHT = "SLOWER_RIGHT";
        static constexpr const char *KEEP_SPEED = "KEEP_SPEED";
        static constexpr const char *KEEP_SPEED_LEFT = "KEEP_SPEED_LEFT";
        static constexpr const char *KEEP_SPEED_RIGHT = "KEEP_SPEED_RIGHT";
        static constexpr const char *SPEED = "SPEED";
        static constexpr const char *SPEED_LEFT = "SPEED_LEFT";
        static constexpr const char *SPEED_RIGHT = "SPEED_RIGHT";

        static constexpr uint8_t PIN_FRONT_RIGHT = 33;
        static constexpr uint8_t PIN_BACK_RIGHT = 25;
        static constexpr uint8_t PIN_FRONT_LEFT = 27;
        static constexpr uint8_t PIN_BACK_LEFT = 26;

        static constexpr uint8_t PIN_SPEED_LEFT = 14;
        static constexpr uint8_t PIN_SPEED_RIGHT = 32;

        static constexpr uint32_t SPEED_MAX = 1023U;
        static constexpr uint32_t SPEED_DEFAULT = SPEED_MAX;
        static constexpr uint32_t SPEED_CHANGE_INTERVAL = 5U;
#ifdef ESP32
        static constexpr uint8_t PWM_CHANNEL_LEFT = 1U;
        static constexpr uint8_t PWM_CHANNEL_RIGHT = 2U;
        static constexpr uint8_t PWM_RESOLUTION = 10U;
        static constexpr uint32_t PWM_FREQUENCY = 1000U;
#endif
        static constexpr const char* SPEED_KEY = "speed";

        speed_controll _speed_controll_left = speed_controll::KEEP_SPEED;
        speed_controll _speed_controll_right = speed_controll::KEEP_SPEED;

        engine_direction _direction_left = engine_direction::STOP;
        engine_direction _direction_right = engine_direction::STOP;

        uint32_t _speed_left = SPEED_DEFAULT;
        uint32_t _speed_right = SPEED_DEFAULT;
    };
} // namespace json_parser

#endif // __ENGINES_H__