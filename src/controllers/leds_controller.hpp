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
        bool initialize() override;
        void update() override;
        DynamicJsonDocument retrive_data() override;

    private:
        bool polish_animation(const JsonObject *json);
        bool eurobeat_animation(const JsonObject *json);
        bool custom_animation(const JsonObject *json);
        bool random_animation(const JsonObject *json);
        bool set_custom_animation(const JsonObject *json);
        bool set_brightness(const JsonObject *json);
        bool set_animation_interval(const JsonObject *json);
        bool stop_animation(const JsonObject *json);
        bool turn_off_leds(const JsonObject *json);
        bool animate_forward(const JsonObject *json);
        bool animate_backward(const JsonObject *json);

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

        static constexpr const char *POLISH = "polish";
        static constexpr const char *EUROBEAT = "eurobeat";
        static constexpr const char *CUSTOM_COLOR = "custom_color";
        static constexpr const char *RANDOM = "random";
        static constexpr const char *SET_CUSTOM_COLOR = "set_custom_color";
        static constexpr const char *ANIMATION_FORWARD = "animation_forward";
        static constexpr const char *ANIMATION_BACKWARD = "animation_backward";
        static constexpr const char *ANIMATION_STOP = "animation_stop";
        static constexpr const char *OFF = "off";

        static constexpr const char *SET_BRIGHTNESS = "set_brightness";
        static constexpr const char *SET_ANIMATION_INTERVAL = "set_interval";

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