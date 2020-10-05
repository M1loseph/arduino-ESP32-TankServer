#ifndef __LED_STRIP_H__
#define __LED_STRIP_H__

#include <FastLED.h>
#include "buffer/buffer.h"

namespace leds
{
    namespace commands
    {
        extern const char *LED_POLISH;
        extern const char *LED_EUROBEAT;
        extern const char *LED_CUSTOM;
        extern const char *LED_RANDOM;
        extern const char *LED_CREATE_CUSTOM;
        extern const char *LED_ANIMATION_FORWARD;
        extern const char *LED_ANIMATION_BACKWARD;
        extern const char *LED_STOP;
        extern const char *LED_CLEAR;

        extern const char *LED_SET_BRIGHTNESS;
        extern const char *LED_SET_ANIMATION_SPEED;
    } // namespace commands

    void init_leds();
    void polish_animation(const CommandBuffer &b);
    void eurobeat_animation(const CommandBuffer &b);
    void custom_animation(const CommandBuffer &b);
    void random_animation(const CommandBuffer &b);
    void create_custom_animation(const CommandBuffer &b);
    void set_brightness(const CommandBuffer &b);
    void set_animation_speed(const CommandBuffer &b);
    void stop_animation(const CommandBuffer &b);
    void clear_animation(const CommandBuffer &b);
    void animate_forward(const CommandBuffer &b);
    void animate_backward(const CommandBuffer &b);
    void update_led_animation();
} // namespace leds

#endif // __LED_STRIP_H__