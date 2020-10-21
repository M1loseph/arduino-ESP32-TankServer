#include "engines.h"
#include "debug.h"
#include "global_parser.h"

#if ENGINE_DEBUG

#define LOG_ENGINE(message) LOG(message)
#define LOG_ENGINE_NL(message) LOG_NL(message)
#define LOG_ENGINE_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_ENGINE(message)
#define LOG_ENGINE_NL(message)
#define LOG_ENGINE_F(...)

#endif // ENGINE_DEBUG

namespace engines
{
    // ================
    // VARIABLES
    // ================

    namespace commands
    {
        const char *FORWARD = "FORWARD";
        const char *FORWARD_LEFT = "FORWARD_LEFT";
        const char *FORWARD_RIGHT = "FORWARD_RIGHT";
        const char *BACKWARD = "BACKWARD";
        const char *BACKWARD_LEFT = "BACKWARD_LEFT";
        const char *BACKWARD_RIGHT = "BACKWARD_RIGHT";
        const char *STOP = "STOP";
        const char *STOP_LEFT = "STOP_LEFT";
        const char *STOP_RIGHT = "STOP_RIGHT";
        const char *FASTER = "FASTER";
        const char *FASTER_LEFT = "FASTER_LEFT";
        const char *FASTER_RIGHT = "FASTER_RIGHT";
        const char *SLOWER = "SLOWER";
        const char *SLOWER_LEFT = "SLOWER_LEFT";
        const char *SLOWER_RIGHT = "SLOWER_RIGHT";
        const char *KEEP_SPEED = "KEEP_SPEED";
        const char *KEEP_SPEED_LEFT = "KEEP_SPEED_LEFT";
        const char *KEEP_SPEED_RIGHT = "KEEP_SPEED_RIGHT";
        const char *SPEED = "SPEED";
        const char *SPEED_LEFT = "SPEED_LEFT";
        const char *SPEED_RIGHT = "SPEED_RIGHT";
    } // namespace commands

    constexpr uint8_t PIN_FRONT_RIGHT = 33;
    constexpr uint8_t PIN_BACK_RIGHT = 25;
    constexpr uint8_t PIN_FRONT_LEFT = 27;
    constexpr uint8_t PIN_BACK_LEFT = 26;

    constexpr uint8_t PIN_SPEED_LEFT = 14;
    constexpr uint8_t PIN_SPEED_RIGHT = 32;

    constexpr size_t SPEED_MAX = 1023U;
    constexpr size_t SPEED_DEFAULT = 700U;
    constexpr size_t SPEED_CHANGE_INTERVAL = 5U;

#ifdef ESP32
    constexpr uint8_t PWM_CHANNEL_LEFT = 1U;
    constexpr uint8_t PWM_CHANNEL_RIGHT = 2U;
    // in bits
    constexpr uint8_t PWM_RESOLUTION = 10U;
    // Hz
    constexpr size_t PWM_FREQUENCY = 1000U;
#endif

    // ================
    // CONFIG
    // ================

    speed_controll speed_controll_left = speed_controll::KEEP_SPEED;
    speed_controll speed_controll_right = speed_controll::KEEP_SPEED;

    engine_direction direction_left = engine_direction::STOP;
    engine_direction direction_right = engine_direction::STOP;

    size_t speed_left = SPEED_DEFAULT;
    size_t speed_right = SPEED_DEFAULT;

    // ================
    // PRIVATE
    // ================

    static void disable_speed_left()
    {
#ifdef ESP32
        ledcWrite(PWM_CHANNEL_LEFT, 0);
#else
        digitalWrite(PIN_SPEED_LEFT, LOW);
#endif
    }

    static void enable_speed_left()
    {
#ifdef ESP32
        ledcWrite(PWM_CHANNEL_LEFT, speed_left);
#else
        analogWrite(PIN_SPEED_LEFT, speed_left);
#endif
    }

    static void disable_speed_right()
    {
#ifdef ESP32
        ledcWrite(PWM_CHANNEL_RIGHT, 0);
#else
        digitalWrite(PIN_SPEED_RIGHT, LOW);
#endif
    }

    static void enable_speed_right()
    {
#ifdef ESP32
        ledcWrite(PWM_CHANNEL_RIGHT, speed_right);
#else
        analogWrite(PIN_SPEED_RIGHT, speed_right);
#endif
    }

    // ================
    // EXPORT
    // ================

    void init_engines()
    {
        pinMode(PIN_FRONT_LEFT, OUTPUT);
        pinMode(PIN_FRONT_RIGHT, OUTPUT);
        pinMode(PIN_BACK_LEFT, OUTPUT);
        pinMode(PIN_BACK_RIGHT, OUTPUT);
#ifdef ESP32
        // two 1000 Hz channels
        ledcSetup(PWM_CHANNEL_LEFT, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcSetup(PWM_CHANNEL_RIGHT, PWM_FREQUENCY, PWM_RESOLUTION);

        ledcAttachPin(PIN_SPEED_LEFT, PWM_CHANNEL_LEFT);
        ledcAttachPin(PIN_SPEED_RIGHT, PWM_CHANNEL_RIGHT);
#else
        pinMode(PIN_SPEED_LEFT, OUTPUT);
        pinMode(PIN_SPEED_RIGHT, OUTPUT);
#endif
        LOG_ENGINE_NL("Initialized engine pins");
    }

    void forward()
    {
        forward_left();
        forward_right();
        LOG_ENGINE_NL("Both Forward");
    }

    void forward_left()
    {
        // disabling speed pin just in case
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, HIGH);
        digitalWrite(PIN_BACK_LEFT, LOW);
        direction_left = engine_direction::FORWARD;
        enable_speed_left();
        LOG_ENGINE_NL("Forward Left");
    }

    void forward_right()
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, HIGH);
        digitalWrite(PIN_BACK_RIGHT, LOW);
        direction_right = engine_direction::FORWARD;
        enable_speed_right();
        LOG_ENGINE_NL("Forward Right");
    }

    void backward()
    {
        backward_left();
        backward_right();
        LOG_ENGINE_NL("Both Backward");
    }

    void backward_left()
    {
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, LOW);
        digitalWrite(PIN_BACK_LEFT, HIGH);
        direction_left = engine_direction::BACKWARD;
        enable_speed_left();
        LOG_ENGINE_NL("Backward Left");
    }

    void backward_right()
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, LOW);
        digitalWrite(PIN_BACK_RIGHT, HIGH);
        direction_right = engine_direction::BACKWARD;
        enable_speed_right();
        LOG_ENGINE_NL("Backward Right");
    }

    void stop()
    {
        stop_left();
        stop_right();
        LOG_ENGINE_NL("Both Stop");
    }

    void stop_left()
    {
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, LOW);
        digitalWrite(PIN_BACK_LEFT, LOW);
        direction_left = engine_direction::STOP;
        LOG_ENGINE_NL("Stop Left");
    }

    void stop_right()
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, LOW);
        digitalWrite(PIN_BACK_RIGHT, LOW);
        direction_right = engine_direction::STOP;
        LOG_ENGINE_NL("Stop Right");
    }

    void rotate_left()
    {
        backward_left();
        forward_right();
        LOG_ENGINE_NL("Rotate Left");
    }

    void rotate_right()
    {
        forward_left();
        backward_right();
        LOG_ENGINE_NL("Rotate Right");
    }

    void slower()
    {
        slower_left();
        slower_right();
        LOG_ENGINE_NL("Both Slower");
    }

    void slower_left()
    {
        speed_controll_left = speed_controll::SLOWER;
        LOG_ENGINE_NL("Slower Left");
    }

    void slower_right()
    {
        speed_controll_right = speed_controll::SLOWER;
        LOG_ENGINE_NL("Slower Right");
    }

    void faster()
    {
        faster_left();
        faster_right();
        LOG_ENGINE_NL("Both Faster");
    }

    void faster_left()
    {
        speed_controll_left = speed_controll::FASTER;
        LOG_ENGINE_NL("Faster Left");
    }

    void faster_right()
    {
        speed_controll_right = speed_controll::FASTER;
        LOG_ENGINE_NL("Faster Right");
    }

    void keep_speed()
    {
        keep_speed_left();
        keep_speed_right();
        LOG_ENGINE_NL("Both keep speed");
    }

    void keep_speed_left()
    {
        speed_controll_left = speed_controll::KEEP_SPEED;
        LOG_ENGINE_NL("Left keeps speed");
    }

    void keep_speed_right()
    {
        speed_controll_right = speed_controll::KEEP_SPEED;
        LOG_ENGINE_NL("Right keeps speed");
    }

    void set_speed(size_t new_speed)
    {
        set_speed_left(new_speed);
        set_speed_right(new_speed);
    }

    void set_speed_left(size_t new_speed)
    {
        if (new_speed < SPEED_MAX)
        {
            speed_left = new_speed;
            if (direction_left != engine_direction::STOP)
                enable_speed_left();
            LOG_ENGINE_F("New left speed: %d", new_speed);
        }
        else
        {
            LOG_ENGINE_F("New left speed too high: %d\n", new_speed);
        }
    }

    void set_speed_right(size_t new_speed)
    {
        if (new_speed < SPEED_MAX)
        {
            speed_right = new_speed;
            if (direction_right != engine_direction::STOP)
                enable_speed_right();
            LOG_ENGINE_F("New right speed: %d\n", new_speed);
        }
        else
        {
            LOG_ENGINE_F("New right speed too high: %d\n", new_speed);
        }
    }

    void forward(const CommandBuffer *b)
    {
        forward();
    }

    void forward_left(const CommandBuffer *b)
    {
        forward_left();
    }

    void forward_right(const CommandBuffer *b)
    {
        forward_right();
    }

    void backward(const CommandBuffer *b)
    {
        backward();
    }

    void backward_left(const CommandBuffer *b)
    {
        backward_left();
    }

    void backward_right(const CommandBuffer *b)
    {
        backward_right();
    }

    void stop(const CommandBuffer *b)
    {
        stop();
    }

    void stop_left(const CommandBuffer *b)
    {
        stop_left();
    }

    void stop_right(const CommandBuffer *b)
    {
        stop_right();
    }

    void rotate_left(const CommandBuffer *b)
    {
        rotate_left();
    }

    void rotate_right(const CommandBuffer *b)
    {
        rotate_right();
    }

    void slower(const CommandBuffer *b)
    {
        slower();
    }

    void slower_left(const CommandBuffer *b)
    {
        slower_left();
    }

    void slower_right(const CommandBuffer *b)
    {
        slower_right();
    }

    void faster(const CommandBuffer *b)
    {
        faster();
    }

    void faster_left(const CommandBuffer *b)
    {
        faster_left();
    }

    void faster_right(const CommandBuffer *b)
    {
        faster_right();
    }

    void keep_speed(const CommandBuffer *b)
    {
        keep_speed();
    }

    void keep_speed_left(const CommandBuffer *b)
    {
        keep_speed_left();
    }

    void keep_speed_right(const CommandBuffer *b)
    {
        keep_speed_right();
    }

    void set_speed(const CommandBuffer *b)
    {
        if (b)
        {
            auto new_speed = b->int_at(1);
            if (new_speed.success && new_speed.i >= 0 && static_cast<size_t>(new_speed.i) <= SPEED_MAX)
                // logging info will happen inside next function
                set_speed(static_cast<size_t>(new_speed.i));
            else
            {
                LOG_ENGINE_F("Wrong new speed parameters - success: %d, val: %d\n", new_speed.success, new_speed.i);
            }
        }
        else
        {
            LOG_ENGINE_NL("Abort set speed - buffer was null");
        }
    }

    void set_speed_left(const CommandBuffer *b)
    {
        if (b)
        {
            auto new_speed = b->int_at(1);
            if (new_speed.success && new_speed.i >= 0 && static_cast<size_t>(new_speed.i) <= SPEED_MAX)
                set_speed_left(static_cast<size_t>(new_speed.i));
            else
            {
                LOG_ENGINE_F("Wrong new speed parameters - success: %d, val: %d\n", new_speed.success, new_speed.i);
            }
        }
        else
        {
            LOG_ENGINE_NL("Abort set speed left - buffer was null");
        }
    }

    void set_speed_right(const CommandBuffer *b)
    {
        if (b)
        {
            auto new_speed = b->int_at(1);
            if (new_speed.success && new_speed.i > 0 && static_cast<size_t>(new_speed.i) <= SPEED_MAX)
                set_speed_right(static_cast<size_t>(new_speed.i));
            else
            {
                LOG_ENGINE_F("Wrong new speed parameters - success: %d, val: %d\n", new_speed.success, new_speed.i);
            }
        }
        else
        {
            LOG_ENGINE_NL("Abort set speed right - buffer was null");
        }
    }

    void update_speed()
    {
        static unsigned long last_update = millis();
        if (millis() - last_update > SPEED_CHANGE_INTERVAL)
        {
            while (xSemaphoreTake(global_parser::semaphore, (TickType_t)10) != pdTRUE)
                ;
            if (speed_controll_left == speed_controll::SLOWER)
                if (speed_left > 0U)
                {
                    set_speed_left(speed_left - 1);
                    LOG_ENGINE_F("New left speed: %d\n", speed_left);
                }

            if (speed_controll_right == speed_controll::SLOWER)
                if (speed_right > 0U)
                {
                    set_speed_right(speed_right - 1);
                    LOG_ENGINE_F("New right speed: %d\n", speed_right);
                }

            if (speed_controll_left == speed_controll::FASTER)
                if (speed_left < SPEED_MAX)
                {
                    set_speed_left(speed_left + 1);
                    LOG_ENGINE_F("New left speed: %d\n", speed_left);
                }

            if (speed_controll_right == speed_controll::FASTER)
                if (speed_right < SPEED_MAX)
                {
                    set_speed_right(speed_right + 1);
                    LOG_ENGINE_F("New right speed: %d\n", speed_right);
                }

            xSemaphoreGive(global_parser::semaphore);
            last_update = millis();
        }
    }

} // namespace engines