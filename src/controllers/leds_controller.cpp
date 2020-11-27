#include <functional>
#include "leds_controller.hpp"
#include "debug.hpp"

#if LEDS_DEBUG

#define LOG_LEDS(message) LOG(message)
#define LOG_LEDS_NL(message) LOG_NL(message)
#define LOG_LEDS_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_LEDS(message)
#define LOG_LEDS_NL(message)
#define LOG_LEDS_F(...)

#endif // LEDS_DEBUG

namespace json_parser
{
    leds_controller::leds_controller() : templated_controller("leds")
    {
    }

    bool leds_controller::initialize()
    {
        FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ODER>(_leds, NUM_LEDS);
        FastLED.setBrightness(_brightness);
        show_leds();

        bool if_added = true;

        if_added &= add_event(ANIMATION_BACKWARD, &leds_controller::animate_backward);
        if_added &= add_event(ANIMATION_FORWARD, &leds_controller::animate_forward);
        if_added &= add_event(SET_CUSTOM_COLOR, &leds_controller::set_custom_animation);
        if_added &= add_event(CUSTOM_COLOR, &leds_controller::custom_animation);
        if_added &= add_event(EUROBEAT, &leds_controller::eurobeat_animation);
        if_added &= add_event(POLISH, &leds_controller::polish_animation);
        if_added &= add_event(RANDOM, &leds_controller::random_animation);
        if_added &= add_event(OFF, &leds_controller::turn_off_leds);
        if_added &= add_event(SET_ANIMATION_INTERVAL, &leds_controller::set_animation_interval);
        if_added &= add_event(SET_BRIGHTNESS, &leds_controller::set_brightness);
        if_added &= add_event(ANIMATION_STOP, &leds_controller::stop_animation);

        return if_added;
    }

    bool leds_controller::polish_animation(const JsonObjectConst *json)
    {
        LOG_LEDS_F("[%s] set polish animation\n", _name)
        _current_animation = &leds_controller::polish;
        show_leds();
        return true;
    }

    bool leds_controller::eurobeat_animation(const JsonObjectConst *json)
    {
        LOG_LEDS_F("[%s] set eurobeat animation\n", _name)
        _current_animation = &leds_controller::rainbow;
        show_leds();
        return true;
    }

    bool leds_controller::custom_animation(const JsonObjectConst *json)
    {
        LOG_LEDS_F("[%s] set custiom animation\n", _name)
        _current_animation = &leds_controller::custom;
        show_leds();
        return true;
    }

    bool leds_controller::random_animation(const JsonObjectConst *json)
    {
        LOG_LEDS_F("[%s] set random animation\n", _name)
        _current_animation = &leds_controller::random;
        show_leds();
        return true;
    }

    bool leds_controller::set_custom_animation(const JsonObjectConst *json)
    {
        if (json)
        {
            if (json->containsKey(COLORS_KEY))
            {
                uint8_t red = (*json)[COLORS_KEY][0];
                uint8_t green = (*json)[COLORS_KEY][1];
                uint8_t blue = (*json)[COLORS_KEY][2];

                _custom_color = CRGB(red, green, blue);
                custom_animation(json);

                LOG_LEDS_F("[%s] new color r: %d, g %d, b%d\n", _name, red, green, blue)
                return true;
            }
            else
            {
                LOG_LEDS_F("[%s] JSON didn't have colors key\n", _name)
            }
        }
        else
        {
            LOG_LEDS_F("[%s] JSON was null\n", _name)
        }
        return false;
    }

    bool leds_controller::set_brightness(const JsonObjectConst *json)
    {
        if (json)
        {
            if (json->containsKey(BRIGHTNESS_KEY))
            {
                uint8_t new_brightness = (*json)[BRIGHTNESS_KEY];
                LOG_LEDS_F("new brightness: %d\n", new_brightness)
                FastLED.setBrightness(new_brightness);
                show_leds();
                return true;
            }
            else
            {
                LOG_LEDS_F("[%s] JSON didn'thave brightness key\n", _name)
            }
        }
        else
        {
            LOG_LEDS_F("[%s] JSON was null\n", _name)
        }
        return false;
    }

    bool leds_controller::set_animation_interval(const JsonObjectConst *json)
    {
        if (json)
        {
            if (json->containsKey(INTERVAL_KEY))
            {
                uint32_t new_interval = (*json)[INTERVAL_KEY];
                LOG_LEDS_F("New animation interval: %d\n", new_interval);
                _animation_interval = new_interval;
                return true;
            }
            else
            {
                LOG_LEDS_F("[%s] JSON didn'thave interval key\n", _name)
            }
        }
        else
        {
            LOG_LEDS_F("[%s] JSON was null\n", _name)
        }
        return false;
    }

    bool leds_controller::stop_animation(const JsonObjectConst *json)
    {
        LOG_LEDS_F("[%s] stop animation\n", _name)
        _direction = animation_direction::STOP;
        return true;
    }

    bool leds_controller::turn_off_leds(const JsonObjectConst *json)
    {
        LOG_LEDS_NL("Turn off leds");
        _current_animation = nullptr;
        show_leds();
        return true;
    }

    bool leds_controller::animate_forward(const JsonObjectConst *json)
    {
        LOG_LEDS_NL("Animate forward");
        _direction = animation_direction::FORWARD;
        return true;
    }

    bool leds_controller::animate_backward(const JsonObjectConst *json)
    {
        LOG_LEDS_NL("Animate backward");
        _direction = animation_direction::BACKWARD;
        return true;
    }

    void leds_controller::show_leds()
    {
        if (_current_animation)
        {
            for (uint32_t i = 0; i < NUM_LEDS; i++)
                _leds[i] = std::__invoke(_current_animation, this, i + _animation_index);
        }
        else
        {
            for (uint32_t i = 0; i < NUM_LEDS; i++)
                _leds[i] = CRGB::Black;
        }
        FastLED.show();
    }

    CRGB leds_controller::polish(uint32_t index)
    {
        return (index % POL_ANIME_INTERVAL) < (POL_ANIME_INTERVAL / 2) ? CRGB::White : CRGB::Red;
    }

    CRGB leds_controller::rainbow(uint32_t index)
    {
        const uint8_t h = static_cast<uint8_t>((index * 255U) / NUM_LEDS);
        return CHSV(h, 255, 255);
    }

    CRGB leds_controller::random(uint32_t index)
    {
        return CRGB(esp_random());
    }

    CRGB leds_controller::custom(uint32_t index)
    {
        return _custom_color;
    }

    void leds_controller::update()
    {
        static unsigned long last_measute = millis();

        if (_direction != animation_direction::STOP && _current_animation && millis() - last_measute > _animation_interval)
        {
            LOG_LEDS_NL("[leds_controller] updating animation...");
            if (_direction == animation_direction::FORWARD)
            {
                _animation_index = (_animation_index + 1U) % NUM_LEDS;
            }
            else if (_direction == animation_direction::BACKWARD)
            {
                if (_animation_index == 0U)
                    _animation_index = NUM_LEDS - 1U;
                else
                    _animation_index--;
            }

            LOG_LEDS_F("[%s] animation index: %d\n", _name, _animation_index)
            show_leds();

            last_measute = millis();
        }
    }
} // namespace json_parser