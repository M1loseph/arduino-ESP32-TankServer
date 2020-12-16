#ifndef __LED_STRIP_H__
#define __LED_STRIP_H__

#include <Arduino.h>
#include <FastLED.h>

#include "abstract/templated_controller.hpp"

namespace json_parser
{
    class leds_controller final : public templated_controller<leds_controller>
    {
    public:
        explicit leds_controller();
        bool initialize() override;
        void update() override;
        DynamicJsonDocument retrive_data() override;

    private:
        bool eurobeat(const JsonObject *json);
        bool custom(const JsonObject *json);
        bool random(const JsonObject *json);
        bool forward(const JsonObject *json);
        bool backward(const JsonObject *json);
        bool stop(const JsonObject *json);
        bool off(const JsonObject *json);

        bool custom_color(const JsonObject *json);
        bool brightness(const JsonObject *json);
        bool update_interval(const JsonObject *json);
        bool length(const JsonObject *json);
        bool color_length(const JsonObject *json);
        bool repetitions(const JsonObject *json);
        bool restore_default(const JsonObject *json);

        void show_leds();

        CRGB rainbow_factory(uint32_t index);
        CRGB random_factory(uint32_t index);
        CRGB custom_factory(uint32_t index);

        enum class animation_direction
        {
            FORWARD = 0,
            BACKWARD,
            STOP,
        };

        enum class animation_length
        {
            INCREMENT = 0,
            DONT_CHANGE,
            DECREMENT
        };

        static constexpr const char *EUROBEAT = "eurobeat";
        static constexpr const char *CUSTOM = "custom";
        static constexpr const char *RANDOM = "random";
        static constexpr const char *FORWARD = "forward";
        static constexpr const char *BACKWARD = "backward";
        static constexpr const char *STOP = "stop";
        static constexpr const char *OFF = "off";
        static constexpr const char *RESTORE_DEFAULT = "restore_default";

        /*
            color_length = 3
             ↓
            XXXOOOXXXOOOXXXOOO
                 ↑
                repetitions = 2
        */
        static constexpr const char *CUSTOM_COLOR = "custom_color";
        static constexpr const char *LENGTH = "length";
        static constexpr const char *BRIGHTNESS = "brightness";
        static constexpr const char *UPDATE_INTERVAL = "update_interval";
        static constexpr const char *REPETITIONS = "repetitions";
        static constexpr const char *COLOR_LENGTH = "color_length";

        static constexpr const char *COLORS_KEY = "colors";
        static constexpr const char *BRIGHTNESS_KEY = BRIGHTNESS;
        static constexpr const char *INTERVAL_KEY = "interval";
        static constexpr const char *INDEX_KEY = "index";
        static constexpr const char *LENGTH_KEY = LENGTH;
        static constexpr const char *REPEPTIONS_KEY = REPETITIONS;

        static constexpr uint32_t NUM_LEDS = 90U;
        static constexpr uint8_t DATA_PIN = 4U;
        static constexpr EOrder COLOR_ODER = GRB;

        static constexpr uint32_t DEF_LENGTH = NUM_LEDS;
        static constexpr uint32_t DEF_REPETITIONS = 0;
        static constexpr uint32_t DEF_COLOR_LENGTH = 0;

        CRGB _leds[NUM_LEDS];
        uint32_t _length = DEF_LENGTH;
        uint8_t _brightness = 100U;
        uint32_t _animation_index = UINT32_MAX / 2;
        uint32_t _animation_updates_interval = 50U;
        uint32_t _repetitions = DEF_REPETITIONS; // 0 -> ignore this parameter
        uint32_t _color_length = DEF_COLOR_LENGTH; // 0 -> ignore this parameter

        CRGB _custom_colors[NUM_LEDS];

        animation_direction _direction = animation_direction::STOP;
        CRGB(leds_controller::*_current_animation)(uint32_t index) = nullptr;
    };
} // namespace json_parser

#endif // __LED_STRIP_H__
