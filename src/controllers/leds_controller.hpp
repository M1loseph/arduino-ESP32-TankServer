#ifndef __LED_STRIP_H__
#define __LED_STRIP_H__

#include <FastLED.h>
#include "abstract/templated_controller.hpp"

namespace json_parser
{
    class leds_controller final : public templated_controller<leds_controller>
    {
    public:
        explicit leds_controller();
        void update() override;
        bool initialize() override;

    private:
        bool polish_animation(const JsonObjectConst *json);
        bool eurobeat_animation(const JsonObjectConst *json);
        bool custom_animation(const JsonObjectConst *json);
        bool random_animation(const JsonObjectConst *json);
        bool set_custom_animation(const JsonObjectConst *json);
        bool set_brightness(const JsonObjectConst *json);
        bool set_animation_interval(const JsonObjectConst *json);
        bool stop_animation(const JsonObjectConst *json);
        bool turn_off_leds(const JsonObjectConst *json);
        bool animate_forward(const JsonObjectConst *json);
        bool animate_backward(const JsonObjectConst *json);

        void show_leds();

        CRGB polish(uint32_t index);
        CRGB rainbow(uint32_t index);
        CRGB random(uint32_t index);
        CRGB custom(uint32_t index);

        enum class animation_direction
        {
            FORWARD,
            BACKWARD,
            STOP,
        };

        static constexpr const char *POLISH = "POLISH";
        static constexpr const char *EUROBEAT = "EUROBEAT";
        static constexpr const char *CUSTOM_COLOR = "CUSTOM_COLOR";
        static constexpr const char *RANDOM = "RANDOM";
        static constexpr const char *SET_CUSTOM_COLOR = "SET_CUSTOM_COLOR";
        static constexpr const char *ANIMATION_FORWARD = "ANIMATION_FORWARD";
        static constexpr const char *ANIMATION_BACKWARD = "ANIMATION_BACKWARD";
        static constexpr const char *ANIMATION_STOP = "ANIMATION_STOP";
        static constexpr const char *OFF = "OFF";

        static constexpr const char *SET_BRIGHTNESS = "SET_BRIGHTNESS";
        static constexpr const char *SET_ANIMATION_INTERVAL = "SET_INTERVAL";

        static constexpr const char *COLORS_KEY = "colors";
        static constexpr const char *BRIGHTNESS_KEY = "brightness";
        static constexpr const char *INTERVAL_KEY = "interval";

        static constexpr uint32_t NUM_LEDS = 90U;
        static constexpr uint8_t DATA_PIN = 4U;
        static constexpr EOrder COLOR_ODER = GRB;

        static constexpr uint32_t POL_ANIME_INTERVAL = 20U;

        CRGB _leds[NUM_LEDS];
        uint8_t _brightness = 100U;
        uint32_t _animation_index = 0;
        uint32_t _animation_interval = 50U;
        animation_direction _direction = animation_direction::STOP;
        CRGB(leds_controller::*_current_animation)(uint32_t index) = nullptr;                
        CRGB _custom_color;
    };
} // namespace json_parser

#endif // __LED_STRIP_H__