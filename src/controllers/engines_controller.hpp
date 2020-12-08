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
        bool initialize() override;
        void update() override;
        DynamicJsonDocument retrive_data() override;

        enum class speed_controll
        {
            SLOWER = -1,
            KEEP_SPEED,
            FASTER
        };

        enum class direction
        {
            BACKWARD = -1,
            STOP,
            FORWARD
        };

        inline speed_controll get_speed_controll_left() { return _speed_controll_left; }
        inline speed_controll get_speed_controll_right() { return _speed_controll_right; }

        inline direction get_direction_left() { return _direction_left; }
        inline direction get_direction_right() { return _direction_right; }

        inline uint32_t get_speed_left() { return _speed_left; }
        inline uint32_t get_speed_right() { return _speed_right; }

    private:
        void disable_speed_left();
        void enable_speed_left();
        void disable_speed_right();
        void enable_speed_right();

        bool forward(const JsonObjectConst *json);
        void forward_left();
        void forward_right();

        bool backward(const JsonObjectConst *json);
        void backward_left();
        void backward_right();

        bool stop(const JsonObjectConst *json);
        void stop_left();
        void stop_right();

        bool rotate(const JsonObjectConst *json);
        void rotate_left();
        void rotate_right();

        bool slower(const JsonObjectConst *json);
        void slower_left();
        void slower_right();

        bool faster(const JsonObjectConst *json);
        void faster_left();
        void faster_right();

        bool keep_speed(const JsonObjectConst *json);
        void keep_speed_left();
        void keep_speed_right();

        bool set_speed(const JsonObjectConst *json);
        void set_speed_left(uint32_t new_speed);
        void set_speed_right(uint32_t new_speed);

        const char *get_engine_from_json(const JsonObjectConst *json);
        size_t get_speed_from_json(const JsonObjectConst *json, bool *succ);

        static constexpr const char *FORWARD = "forward";
        static constexpr const char *BACKWARD = "backward";
        static constexpr const char *STOP = "stop";
        static constexpr const char *FASTER = "faster";
        static constexpr const char *SLOWER = "slower";
        static constexpr const char *KEEP_SPEED = "keep_speed";
        static constexpr const char *SPEED = "speed";
        static constexpr const char *ROTATE = "rotate";

        static constexpr const char *LEFT = "left";
        static constexpr const char *RIGHT = "right";
        static constexpr const char *BOTH = "both";

        static constexpr const char* SPEED_KEY = "speed";
        static constexpr const char* ENGINE_KEY = "engine";
        static constexpr const char* DIRECTION_KEY = "direction";
        static constexpr const char* SPEED_CONTROLL_KEY = "speed_controll";

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
        speed_controll _speed_controll_left = speed_controll::KEEP_SPEED;
        speed_controll _speed_controll_right = speed_controll::KEEP_SPEED;

        direction _direction_left = direction::STOP;
        direction _direction_right = direction::STOP;

        uint32_t _speed_left = SPEED_DEFAULT;
        uint32_t _speed_right = SPEED_DEFAULT;
    };
} // namespace json_parser

#endif // __ENGINES_H__