#include "engines.h"

#if ENGINE_DEBUG

#define LOG_ENGINE(message) LOG(message)
#define LOG_ENGINE_NL(message) LOG_NL(message)

#else

#define LOG_ENGINE(message)
#define LOG_ENGINE_NL(message)

#endif

namespace driving
{
    // ================
    // VARIABLES
    // ================

    namespace commands
    {
        const char *FORWARD = "FORWARD";
        const char *BACKWARD = "BACKWARD";
        const char *STOP = "STOP";
        const char *FASTER = "FASTER";
        const char *SLOWER = "SLOWER";
        const char *KEEP_SPEED = "KEEP_SPEED";
        const char *SET_SPEED = "SET_SPEED";

        namespace modifiers
        {
            const char *LEFT = "LEFT";
            const char *RIGHT = "RIGHT";
            const char *BOTH = "BOTH";
        } // namespace modifiers
    }     // namespace commands

    constexpr uchar PIN_FRONT_RIGHT = 33;
    constexpr uchar PIN_BACK_RIGHT = 25;
    constexpr uchar PIN_FRONT_LEFT = 27;
    constexpr uchar PIN_BACK_LEFT = 26;

    constexpr uchar PIN_SPEED_LEFT = 14;
    constexpr uchar PIN_SPEED_RIGHT = 32;

    constexpr size_t SPEED_MAX = 1023U;
    constexpr size_t SPEED_DEFAULT = 700U;
    constexpr size_t SPEED_CHANGE_INTERVAL = 5U;

#ifdef ESP32
    constexpr uchar PWM_CHANNEL_LEFT = 1U;
    constexpr uchar PWM_CHANNEL_RIGHT = 2U;
    // in bits
    constexpr uchar PWM_RESOLUTION = 10U;
    // Hz
    constexpr size_t PWM_FREQUENCY = 1000U;
#endif

    SpeedControll speed_controll_left;
    SpeedControll speed_controll_right;

    Direction direction_left;
    Direction direction_right;

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

    // this function doesn't provide any safety, check agrs beforehand
    template <typename fun1, typename fun2, typename fun3, typename... Args>
    static void check_modifier_3_options(const char *command, fun1 if_both, fun2 if_left, fun3 if_right, Args... args)
    {
        if (strcmp(command, commands::modifiers::BOTH) == 0)
            if_both(args...);
        else if (strcmp(command, commands::modifiers::LEFT) == 0)
            if_left(args...);
        else if (strcmp(command, commands::modifiers::RIGHT) == 0)
            if_right(args...);
        else
        {
            LOG_ENGINE("Unknown modifier ");
            LOG_NL(command);
        }
    }

    template <typename fun1, typename fun2, typename... Args>
    static void check_modifier_2_options(const char *command, fun1 if_left, fun2 if_right, Args... args)
    {
        if (strcmp(command, commands::modifiers::LEFT) == 0)
            if_left(args...);
        else if (strcmp(command, commands::modifiers::RIGHT) == 0)
            if_right(args...);
        else
        {
            LOG_ENGINE("Unknown modifier ");
            LOG_NL(command);
        }
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

    void forward_left()
    {
        // disabling speed pin just in case
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, HIGH);
        digitalWrite(PIN_BACK_LEFT, LOW);
        direction_left = Direction::FORWARD;
        enable_speed_left();
        LOG_ENGINE_NL("Forward Left");
    }

    void forward_right()
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, HIGH);
        digitalWrite(PIN_BACK_RIGHT, LOW);
        direction_right = Direction::FORWARD;
        enable_speed_right();
        LOG_ENGINE_NL("Forward Right");
    }

    void forward_both()
    {
        forward_left();
        forward_right();
        LOG_ENGINE_NL("Both Forward");
    }

    void backward_left()
    {
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, LOW);
        digitalWrite(PIN_BACK_LEFT, HIGH);
        direction_left = Direction::BACKWARD;
        enable_speed_left();
        LOG_ENGINE_NL("Backward Left");
    }

    void backward_right()
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, LOW);
        digitalWrite(PIN_BACK_RIGHT, HIGH);
        direction_right = Direction::BACKWARD;
        enable_speed_right();
        LOG_ENGINE_NL("Backward Right");
    }

    void backward_both()
    {
        backward_left();
        backward_right();
        LOG_ENGINE_NL("Both Backward");
    }

    void stop_left()
    {
        disable_speed_left();
        digitalWrite(PIN_FRONT_LEFT, LOW);
        digitalWrite(PIN_BACK_LEFT, LOW);
        direction_left = Direction::STOP;
        LOG_ENGINE_NL("Stop Left");
    }

    void stop_right()
    {
        disable_speed_right();
        digitalWrite(PIN_FRONT_RIGHT, LOW);
        digitalWrite(PIN_BACK_RIGHT, LOW);
        direction_right = Direction::STOP;
        LOG_ENGINE_NL("Stop Right");
    }

    void stop_both()
    {
        stop_left();
        stop_right();
        LOG_ENGINE_NL("Both Stop");
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

    void slower_left()
    {
        speed_controll_left = SpeedControll::SLOWER;
        LOG_ENGINE_NL("Slower Left");
    }

    void slower_right()
    {
        speed_controll_right = SpeedControll::SLOWER;
        LOG_ENGINE_NL("Slower Right");
    }

    void slower_both()
    {
        slower_left();
        slower_right();
        LOG_ENGINE_NL("Both Slower");
    }

    void faster_left()
    {
        speed_controll_left = SpeedControll::FASTER;
        LOG_ENGINE_NL("Faster Left");
    }

    void faster_right()
    {
        speed_controll_right = SpeedControll::FASTER;
        LOG_ENGINE_NL("Faster Right");
    }

    void faster_both()
    {
        faster_left();
        faster_right();
        LOG_ENGINE_NL("Both Faster");
    }

    void keep_speed_left()
    {
        speed_controll_left = SpeedControll::KEEP_SPEED;
        LOG_ENGINE_NL("Left keeps speed");
    }

    void keep_speed_right()
    {
        speed_controll_right = SpeedControll::KEEP_SPEED;
        LOG_ENGINE_NL("Right keeps speed");
    }

    void keep_speed_both()
    {
        keep_speed_left();
        keep_speed_right();
        LOG_ENGINE_NL("Both keep speed");
    }

    void set_speed_left(size_t new_speed)
    {
        if (new_speed < SPEED_MAX)
        {
            speed_left = new_speed;
            if(direction_left != Direction::STOP)
                enable_speed_left();
            LOG_ENGINE("New left speed: ");
        }
        else
        {
            LOG_ENGINE("New left speed too high: ");
        }
        LOG_ENGINE_NL(new_speed);
    }

    void set_speed_right(size_t new_speed)
    {
        if (new_speed < SPEED_MAX)
        {
            speed_right = new_speed;
            if(direction_right != Direction::STOP)
                enable_speed_right();
            LOG_ENGINE("New right speed: ");
        }
        else
        {
            LOG_ENGINE("New right speed too high: ");
        }
        LOG_ENGINE_NL(new_speed);
    }

    void set_speed_both(size_t new_speed)
    {
        set_speed_left(new_speed);
        set_speed_right(new_speed);
    }

    void forward(const CommandBuffer &b)
    {
        const char *command = b.word_at(1);
        if (command)
            check_modifier_3_options(command, forward_both, forward_left, forward_right);
    }

    void backward(const CommandBuffer &b)
    {
        const char *command = b.word_at(1);
        if (command)
            check_modifier_3_options(command, backward_both, backward_left, backward_right);
    }

    void stop(const CommandBuffer &b)
    {
        const char *command = b.word_at(1);
        if (command)
            check_modifier_3_options(command, stop_both, stop_left, stop_right);
    }

    void rotate(const CommandBuffer &b)
    {
        const char *command = b.word_at(1);
        if (command)
            check_modifier_2_options(command, rotate_left, rotate_right);
    }

    void slower(const CommandBuffer &b)
    {
        const char *command = b.word_at(1);
        if (command)
            check_modifier_3_options(command, stop_both, stop_left, stop_right);
    }

    void faster(const CommandBuffer &b)
    {
        const char *command = b.word_at(1);
        if (command)
            check_modifier_3_options(command, faster_both, faster_left, faster_right);
    }

    void keep_speed(const CommandBuffer &b)
    {
        const char *command = b.word_at(1);
        if (command)
            check_modifier_3_options(command, keep_speed_both, keep_speed_left, keep_speed_right);
    }

    void set_speed(const CommandBuffer &b)
    {
        const char *command = b.word_at(1);
        Integer new_speed = b.int_at(2);

        // check if new_speed is in bounderies
        if (command && new_speed.success && new_speed.value > 0 && static_cast<size_t>(new_speed.value) < SPEED_MAX)
            check_modifier_3_options(command, set_speed_both, set_speed_left, set_speed_right, static_cast<int>(new_speed.value));
        else
        {
            LOG_ENGINE_NL("Wrong parameters");
            LOG_ENGINE("Command: ");
            LOG_ENGINE_NL(command ? command : "NULL");
            LOG_ENGINE("Number: ");
            if (new_speed.success)
                LOG_ENGINE_NL(new_speed.value);
            else
                LOG_ENGINE_NL("NULL");
        }
    }

    void update_speed()
    {
        static unsigned long last_update = millis();
        if (millis() - last_update > SPEED_CHANGE_INTERVAL)
        {
            if (speed_controll_left == SpeedControll::SLOWER)
                if (speed_left > 0U)
                    set_speed_left(speed_left - 1);

            if (speed_controll_right == SpeedControll::SLOWER)
                if (speed_right > 0U)
                    set_speed_right(speed_right - 1);

            if (speed_controll_left == SpeedControll::FASTER)
                if (speed_left < SPEED_MAX)
                    set_speed_left(speed_left + 1);

            if (speed_controll_right == SpeedControll::FASTER)
                if (speed_right < SPEED_MAX)
                    set_speed_right(speed_right + 1);

            last_update = millis();
            LOG_ENGINE_NL("Updated Speed");
        }
    }

} // namespace driving