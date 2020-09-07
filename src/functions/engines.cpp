#include "engines.h"

// ================
// VARIABLES
// ================

#if ENGINE_DEBUG

#define LOG_ENGINE(message) LOG(message)
#define LOG_ENGINE_NL(message) LOG_NL(message)

#else

#define LOG_ENGINE(message)
#define LOG_ENGINE_NL(message)

#endif

SpeedControll speed_controll_left;
SpeedControll speed_controll_right;

Direction direction_left;
Direction direction_right;

size_t current_speed_left = SPEED_DEFAULT;
size_t current_speed_right = SPEED_DEFAULT;

// ================
// PRIVATE
// ================

static void disable_left_speed()
{
#ifdef ESP_32
    ledcWrite(PWM_CHANNEL_LEFT, 0);
#else
    digitalWrite(PIN_SPEED_RIGHT, LOW);
#endif
}

static void enable_left_speed()
{
#ifdef ESP_32
    ledcWrite(PWM_CHANNEL_RIGHT, current_speed_right);
#else
    analogWrite(PIN_SPEED_RIGHT, speed_right);
#endif
}

static void disable_right_speed()
{
#ifdef ESP_32
    ledcWrite(PWM_CHANNEL_RIGHT, 0);
#else
    digitalWrite(PIN_SPEED_RIGHT, LOW);
#endif
}

static void enable_right_speed()
{
#ifdef ESP_32
    ledcWrite(PWM_CHANNEL_RIGHT, current_speed_right);
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
#ifdef ESP_32
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

void forward_left(const CommandBuffer &b)
{
    // disabling speed pin just in case
    disable_left_speed();
    digitalWrite(PIN_FRONT_LEFT, HIGH);
    digitalWrite(PIN_BACK_LEFT, LOW);
    direction_left = Direction::FORWARD;
    enable_left_speed();
    LOG_ENGINE_NL("Forward Left");
}

void forward_right(const CommandBuffer &b)
{
    disable_right_speed();
    digitalWrite(PIN_FRONT_RIGHT, HIGH);
    digitalWrite(PIN_BACK_RIGHT, LOW);
    direction_right = Direction::FORWARD;
    enable_right_speed();
    LOG_ENGINE_NL("Forward Right");
}

void forward(const CommandBuffer &b)
{
    forward_left(b);
    forward_right(b);
    LOG_ENGINE_NL("Both Forward");
}

void backward_left(const CommandBuffer &b)
{
    disable_left_speed();
    digitalWrite(PIN_FRONT_LEFT, LOW);
    digitalWrite(PIN_BACK_LEFT, HIGH);
    direction_left = Direction::BACKWARD;
    enable_left_speed();
    LOG_ENGINE_NL("Backward Left");
}

void backward_right(const CommandBuffer &b)
{
    disable_right_speed();
    digitalWrite(PIN_FRONT_RIGHT, LOW);
    digitalWrite(PIN_BACK_RIGHT, HIGH);
    direction_right = Direction::BACKWARD;
    enable_right_speed();
    LOG_ENGINE_NL("Backward Right");
}

void backward(const CommandBuffer &b)
{
    backward_left(b);
    backward_right(b);
    LOG_ENGINE_NL("Both Backward");
}

void stop_left(const CommandBuffer &b)
{
    disable_left_speed();
    digitalWrite(PIN_FRONT_LEFT, LOW);
    digitalWrite(PIN_BACK_LEFT, LOW);
    direction_left = Direction::STOP;
    LOG_ENGINE_NL("Stop Left");
}

void stop_right(const CommandBuffer &b)
{
    disable_right_speed();
    digitalWrite(PIN_FRONT_RIGHT, LOW);
    digitalWrite(PIN_BACK_RIGHT, LOW);
    direction_right = Direction::STOP;
    LOG_ENGINE_NL("Stop Right");
}

void stop(const CommandBuffer &b)
{
    stop_left(b);
    stop_right(b);
    LOG_ENGINE_NL("Both Stop");
}

void rotate_left(const CommandBuffer &b)
{
    backward_left(b);
    forward_right(b);
    LOG_ENGINE_NL("Rotate Left");
}

void rotate_right(const CommandBuffer &b)
{
    forward_left(b);
    backward_right(b);
    LOG_ENGINE_NL("Rotate Right");
}

void slower_left(const CommandBuffer &b)
{
    speed_controll_left = SpeedControll::SLOWER;
    LOG_ENGINE_NL("Slower Left");
}

void slower_right(const CommandBuffer &b)
{
    speed_controll_right = SpeedControll::SLOWER;
    LOG_ENGINE_NL("Slower Right");
}

void slower(const CommandBuffer &b)
{
    slower_left(b);
    slower_right(b);
    LOG_ENGINE_NL("Both Slower");
}

void faster_left(const CommandBuffer &b)
{
    speed_controll_left = SpeedControll::FASTER;
    LOG_ENGINE_NL("Faster Left");
}

void faster_right(const CommandBuffer &b)
{
    speed_controll_right = SpeedControll::FASTER;
    LOG_ENGINE_NL("Faster Right");
}

void faster(const CommandBuffer &b)
{
    faster_left(b);
    faster_right(b);
    LOG_ENGINE_NL("Both Faster");
}

void steady_left(const CommandBuffer &b)
{
    speed_controll_left = SpeedControll::STEADY;
    LOG_ENGINE_NL("Steady Left");
}

void steady_right(const CommandBuffer &b)
{
    speed_controll_right = SpeedControll::STEADY;
    LOG_ENGINE_NL("Steady Right");
}

void steady(const CommandBuffer &b)
{
    steady_left(b);
    steady_right(b);
    LOG_ENGINE_NL("Both Steady");
}

void update_speed()
{
    static unsigned long last_update = millis();
    if (millis() - last_update > SPEED_CHANGE_INTERVAL)
    {
        if (speed_controll_left == SpeedControll::SLOWER)
            if (current_speed_left > 0U)
                current_speed_left--;

        if (speed_controll_right == SpeedControll::SLOWER)
            if (current_speed_right > 0U)
                current_speed_right--;

        if (speed_controll_left == SpeedControll::FASTER)
            if (current_speed_left < SPEED_MAX)
                current_speed_left++;

        if (speed_controll_right == SpeedControll::FASTER)
            if (current_speed_right < SPEED_MAX)
                current_speed_right++;

        last_update = millis();
        LOG_ENGINE_NL("Updated Speed");
    }
}
