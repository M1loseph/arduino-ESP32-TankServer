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
    leds_controller::leds_controller() : templated_controller("leds", JSON_OBJECT_SIZE(5) + JSON_ARRAY_SIZE(NUM_LEDS))
    {
    }

    bool leds_controller::initialize()
    {
        FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ODER>(_leds, NUM_LEDS);
        FastLED.setBrightness(_brightness);
        show_leds();

        bool if_added = true;

        if_added &= add_event(EUROBEAT, &leds_controller::eurobeat);
        if_added &= add_event(CUSTOM, &leds_controller::custom);
        if_added &= add_event(RANDOM, &leds_controller::random);
        if_added &= add_event(FORWARD, &leds_controller::forward);
        if_added &= add_event(BACKWARD, &leds_controller::backward);
        if_added &= add_event(STOP, &leds_controller::stop);
        if_added &= add_event(OFF, &leds_controller::off);

        if_added &= add_event(CUSTOM_COLOR, &leds_controller::custom_color);
        if_added &= add_event(LENGTH, &leds_controller::length);
        if_added &= add_event(BRIGHTNESS, &leds_controller::brightness);
        if_added &= add_event(UPDATE_INTERVAL, &leds_controller::update_interval);
        if_added &= add_event(REPETITIONS, &leds_controller::repetitions);
        if_added &= add_event(COLOR_LENGTH, &leds_controller::color_length);
        if_added &= add_event(RESTORE_DEFAULT, &leds_controller::restore_default);

        return if_added;
    }

    bool leds_controller::eurobeat(const JsonObject *json)
    {
        LOG_LEDS_F("[%s] set eurobeat animation\n", _name)
        _current_animation = &leds_controller::rainbow_factory;
        show_leds();
        return true;
    }

    bool leds_controller::custom(const JsonObject *json)
    {
        LOG_LEDS_F("[%s] set custiom animation\n", _name)
        _current_animation = &leds_controller::custom_factory;
        show_leds();
        return true;
    }

    bool leds_controller::random(const JsonObject *json)
    {
        LOG_LEDS_F("[%s] set random animation\n", _name)
        _current_animation = &leds_controller::random_factory;
        show_leds();
        return true;
    }

    bool leds_controller::custom_color(const JsonObject *json)
    {
        if (json)
        {
            if (json->containsKey(COLORS_KEY) && json->containsKey(INDEX_KEY))
            {
                uint32_t index = (*json)[INDEX_KEY];
                if (index < NUM_LEDS)
                {
                    uint8_t red = (*json)[COLORS_KEY][0];
                    uint8_t green = (*json)[COLORS_KEY][1];
                    uint8_t blue = (*json)[COLORS_KEY][2];

                    _custom_colors[index] = CRGB(red, green, blue);

                    LOG_LEDS_F("[%s] new color r: %u, g: %u, b: %u, i: %u\n", _name, red, green, blue, index)
                    return true;
                }
                else
                {
                    LOG_LEDS_F("[%s] index too big: %u\n", _name, index)
                }
            }
            else
            {
                LOG_LEDS_F("[%s] no keys: %s: %u, %s: %u\n", _name, COLORS_KEY,
                           json->containsKey(COLORS_KEY), INDEX_KEY, json->containsKey(INDEX_KEY))
            }
        }
        else
        {
            LOG_LEDS_F("[%s] JSON was null\n", _name)
        }
        return false;
    }

    bool leds_controller::brightness(const JsonObject *json)
    {
        if (json)
        {
            if (json->containsKey(BRIGHTNESS_KEY))
            {
                _brightness = (*json)[BRIGHTNESS_KEY];
                LOG_LEDS_F("new brightness: %u\n", _brightness)
                FastLED.setBrightness(_brightness);
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

    bool leds_controller::update_interval(const JsonObject *json)
    {
        if (json)
        {
            if (json->containsKey(INTERVAL_KEY))
            {
                _animation_updates_interval = (*json)[INTERVAL_KEY];
                LOG_LEDS_F("New update interval: %u\n", _animation_updates_interval);
                return true;
            }
            else
            {
                LOG_LEDS_F("[%s] no interval key\n", _name)
            }
        }
        else
        {
            LOG_LEDS_F("[%s] JSON was null\n", _name)
        }
        return false;
    }

    bool leds_controller::stop(const JsonObject *json)
    {
        LOG_LEDS_F("[%s] stop animation\n", _name)
        _direction = animation_direction::STOP;
        return true;
    }

    bool leds_controller::off(const JsonObject *json)
    {
        LOG_LEDS_F("[%s] turn off leds\n", _name);
        _current_animation = nullptr;
        show_leds();
        return true;
    }

    bool leds_controller::forward(const JsonObject *json)
    {
        LOG_LEDS_F("[%s] animate forward\n", _name);
        _direction = animation_direction::FORWARD;
        return true;
    }

    bool leds_controller::backward(const JsonObject *json)
    {
        LOG_LEDS_F("[%s] animate backward\n", _name);
        _direction = animation_direction::BACKWARD;
        return true;
    }

    bool leds_controller::length(const JsonObject *json)
    {
        if (json)
        {
            if (json->containsKey(LENGTH_KEY))
            {
                uint32_t length = (*json)[LENGTH_KEY];
                if (length < NUM_LEDS)
                {
                    _length = length;
                    LOG_F("[%s] new length: %u\n", _name, _length)
                    return true;
                }
                else
                {
                    LOG_LEDS_F("[%s] length was too big\n", _name)
                }
            }
            else
            {
                LOG_LEDS_F("[%s] no length key\n", _name)
            }
        }
        else
        {
            LOG_LEDS_F("[%s] JSON was null\n", _name)
        }
        return false;
    }

    bool leds_controller::color_length(const JsonObject *json)
    {
        if (json)
        {
            if (json->containsKey(LENGTH_KEY))
            {
                uint32_t length = (*json)[LENGTH_KEY];
                if (length < NUM_LEDS)
                {
                    _color_length = length;
                    LOG_F("[%s] new length: %u\n", _name, _length)
                    return true;
                }
                else
                {
                    LOG_LEDS_F("[%s] length was too big\n", _name)
                }
            }
            else
            {
                LOG_LEDS_F("[%s] no length key\n", _name)
            }
        }
        else
        {
            LOG_LEDS_F("[%s] JSON was null\n", _name)
        }
        return false;
    }

    bool leds_controller::repetitions(const JsonObject *json)
    {
        if (json)
        {
            if (json->containsKey(REPEPTIONS_KEY))
            {
                _repetitions = (*json)[REPEPTIONS_KEY];
                LOG_F("[%s] new repetitions: %u\n", _name, _repetitions)
                return true;
            }
            else
            {
                LOG_LEDS_F("[%s] no interval key\n", _name)
            }
        }
        else
        {
            LOG_LEDS_F("[%s] JSON was null\n", _name)
        }
        return false;
    }

    bool leds_controller::restore_default(const JsonObject *json)
    {
        LOG_LEDS_F("[%s] restored default\n", _name)
        _length = DEF_LENGTH;
        _repetitions = DEF_REPETITIONS;
        _color_length = DEF_COLOR_LENGTH;
        return true;
    }

    void leds_controller::show_leds()
    {
        if (_current_animation)
        {
            for (uint32_t i = 0; i < NUM_LEDS; i++)
            {
                if (i < _length)
                {
                    uint32_t index = (i + _animation_index) % _length;
                    if (_color_length)
                        index /= _color_length;

                    if (_repetitions)
                        index %= _repetitions;

                    _leds[i] = (this->*_current_animation)(index);
                }
                else
                    _leds[i] = CRGB::Black;
            }
        }
        else
        {
            for (uint32_t i = 0; i < NUM_LEDS; i++)
                _leds[i] = CRGB::Black;
        }
        FastLED.show();
    }

    CRGB leds_controller::rainbow_factory(uint32_t index)
    {
        const uint8_t h = static_cast<uint8_t>((index * 255U) / NUM_LEDS);
        return CHSV(h, 255, 255);
    }

    CRGB leds_controller::random_factory(uint32_t index)
    {
        return CRGB(esp_random());
    }

    CRGB leds_controller::custom_factory(uint32_t index)
    {
        return _custom_colors[index];
    }

    void leds_controller::update()
    {
        static unsigned long last_measute = millis();

        if (_direction != animation_direction::STOP && _current_animation && millis() - last_measute > _animation_updates_interval)
        {
            if (_direction == animation_direction::FORWARD)
            {
                _animation_index = (_animation_index + 1U) % (UINT32_MAX - NUM_LEDS);
            }
            else if (_direction == animation_direction::BACKWARD)
            {
                if (_animation_index == 0U)
                    _animation_index = UINT32_MAX - 1U - NUM_LEDS;
                else
                    _animation_index--;
            }

            LOG_LEDS_F("[%s] animation index: %u\n", _name, _animation_index)
            show_leds();

            last_measute = millis();
        }
    }

    DynamicJsonDocument leds_controller::retrive_data()
    {
        DynamicJsonDocument json(1600);
        json[NAME_FIELD] = _name;
        JsonObject data = json.createNestedObject(DATA_FIELD);

        data[BRIGHTNESS_KEY] = _brightness;
        data[INTERVAL_KEY] = _animation_updates_interval;
        JsonArray array = data.createNestedArray(COLORS_KEY);
        for (uint8_t i = 0; i < NUM_LEDS; i++)
        {
            auto &color = _leds[i];
            uint32_t buffer = color.red;
            buffer |= static_cast<uint32_t>(color.green) >> 8;
            buffer |= static_cast<uint32_t>(color.blue) >> 16;

            array.add(buffer);
        }
        return json;
    }
} // namespace json_parser