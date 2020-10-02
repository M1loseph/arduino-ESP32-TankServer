#include "leds.h"
#include "debug.h"

#if LEDS_DEBUG

#define LOG_LEDS(message) LOG(message)
#define LOG_LEDS_NL(message) LOG_NL(message)
#define LOG_LEDS_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_LEDS(message)
#define LOG_LEDS_NL(message)
#define LOG_LEDS_F(...)

#endif // LEDS_DEBUG

namespace leds
{
    // ================
    // VARIABLES
    // ================

    namespace commands
    {
        const char *LED_POLISH = "LED_POLISH";
        const char *LED_EUROBEAT = "LED_EUROBEAT";
        const char *LED_CUSTOM = "LED_CUSTOM";
        const char *LED_RANDOM = "LED_RANDOM";
        const char *LED_CREATE_CUSTOM = "LED_CREATE_CUSTOM";
        const char *LED_ANIMATION_FORWARD = "LED_ANIMATION_FORWARD";
        const char *LED_ANIMATION_BACKWARD = "LED_ANIMATION_BACKWARD";
        const char *LED_STOP = "LED_STOP";

        const char *LED_SET_BRIGHTNESS = "LED_SET_BRIGHTNESS";
        const char *LED_SET_ANIMATION_SPEED = "LED_SET_ANIMATION_SPEED";
    } // namespace commands

    constexpr size_t NUM_LEDS = 90;
    constexpr uchar DATA_PIN = 19;
    constexpr EOrder COLOR_ODER = GRB;

    CRGB leds[NUM_LEDS];

    enum class animation_direction
    {
        FORWARD,
        BACKWARD,
        STOP,
    };

    // ================
    // CONFIG
    // ================

    uchar brightness = 100U;
    size_t animation_index = 0;
    size_t animation_speed = 50U;
    size_t pol_anime_interval = 20U;
    animation_direction direction = animation_direction::STOP;

    namespace animations
    {
        CRGB polish(size_t index)
        {
            // evety 10 leds swap between white and red
            return (index % pol_anime_interval) < (pol_anime_interval / 2) ? CRGB::White : CRGB::Red;
        }

        CRGB rainbow(size_t index)
        {
            const uint8_t h = static_cast<uint8_t>((index * 255U) / NUM_LEDS);
            LOG_LEDS_NL(h);
            return CHSV(h, 255, 255);
        }

        CRGB random(size_t index)
        {
            // creates totally random color (even int size matches)
            return CRGB(esp_random());
        }

        CRGB custom_buffer[NUM_LEDS];

        CRGB custom(size_t index)
        {
            return custom_buffer[index];
        }
    } // namespace animations

    // no idea why it's formated this way, it sucks
    CRGB(*current_animation)
    (size_t index) = nullptr;

    // ================
    // PRIVATE
    // ================

    static void show_leds()
    {
        if (current_animation)
        {
            for (size_t i = 0; i < NUM_LEDS; i++)
                leds[i] = current_animation(i + animation_index);

            FastLED.show();
        }
    }

    // ================
    // EXPORT
    // ================

    void init_leds()
    {
        FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ODER>(leds, NUM_LEDS);
        FastLED.setBrightness(brightness);
        current_animation = &animations::rainbow;
        show_leds();
    }

    void polish_animation(const CommandBuffer &b)
    {
        LOG_LEDS_NL("Set polish animation");
        current_animation = &animations::polish;
        show_leds();
    }

    void eurobeat_animation(const CommandBuffer &b)
    {
        LOG_LEDS_NL("Set eurobeat animation");
        current_animation = &animations::rainbow;
        show_leds();
    }

    void custom_animation(const CommandBuffer &b)
    {
        LOG_LEDS_NL("Set custiom animation");
        current_animation = &animations::custom;
        show_leds();
    }

    void random_animation(const CommandBuffer &b)
    {
        LOG_LEDS_NL("Set random animation");
        current_animation = &animations::random;
        show_leds();
    }

    void create_custom_animation(const CommandBuffer &b)
    {
        auto index = b.int_at(1);
        auto red = b.int_at(2);
        auto green = b.int_at(3);
        auto blue = b.int_at(4);

        if (index.success && red.success && green.success && blue.success)
        {
            // check if all collors are in bounds
            if (red.i >= 0 && green.i >= 0 && blue.i >= 0 && red.i < 256 && green.i < 256 && blue.i < 256 && index.i >= 0 && index.i < NUM_LEDS)
            {
                // cast it to remove compiler warnings -> we checked everything beforehand anyway
                animations::custom_buffer[index.i] = CRGB(
                    static_cast<uint8_t>(red.i),
                    static_cast<uint8_t>(green.i),
                    static_cast<uint8_t>(blue.i));
            }
            else
            {
                LOG_LEDS_F("Parameter out of bouns - i: %d, r: %d, g: %d, b: %d", index.success, red.success, green.success, blue.success);
            }
        }
        else
        {
            LOG_LEDS_F("At least one wrong parameter - i: %d, r: %d, g: %d, b: %d", index.success, red.success, green.success, blue.success);
        }
    }

    void set_brightness(const CommandBuffer &b)
    {
        auto new_brighntess = b.int_at(1);
        if (new_brighntess.success && new_brighntess.i >= 0 && new_brighntess.i < 256)
        {
            LOG_LEDS_F("New brightness: %d\n", new_brighntess.i);
            FastLED.setBrightness(static_cast<uint8_t>(new_brighntess.i));
            show_leds();
        }
        else
        {
            LOG_LEDS_F("Wrong data - succ: %d, brigness: %d\n", new_brighntess.success, new_brighntess.i);
        }
    }

    void set_animation_speed(const CommandBuffer &b)
    {
        auto new_speed = b.int_at(1);
        if (new_speed.success && new_speed.i >= 0)
        {
            LOG_LEDS_F("New animation speed: %d\n", new_speed.i);
            animation_speed = static_cast<size_t>(new_speed.i);
        }
        else
        {
            LOG_LEDS_F("Wrong data - succ: %d, speed: %d\n", new_speed.success, new_speed.i);
        }
    }

    void stop_animation(const CommandBuffer &b)
    {
        direction = animation_direction::STOP;
    }

    void animate_forward(const CommandBuffer &b)
    {
        direction = animation_direction::FORWARD;
    }

    void animate_backward(const CommandBuffer &b)
    {
        direction = animation_direction::BACKWARD;
    }

    void update_led_animation()
    {
        static unsigned long last_measute = millis();

        if (direction != animation_direction::STOP && current_animation && millis() - last_measute > animation_speed)
        {
            LOG_LEDS_NL("Updating animation...");
            if (direction == animation_direction::FORWARD)
            {
                animation_index = (animation_index + 1U) % NUM_LEDS;
            }
            else if (direction == animation_direction::BACKWARD)
            {
                if (animation_index == 0U)
                    animation_index = NUM_LEDS - 1U;
                else
                    animation_index--;
            }

            LOG_LEDS_F("Animation index: %d\n", animation_index);
            show_leds();
            last_measute = millis();
        }
    }
} // namespace leds