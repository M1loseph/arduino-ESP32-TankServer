#include "engines.h"

#if ENGINE_DEBUG

#define LOG_ENGINE(message) LOG(message)
#define LOG_ENGINE_NL(message) LOG_NL(message)

#else

#define LOG_ENGINE(message)
#define LOG_ENGINE_NL(message)

#endif

// ================
// VARIABLES
// ================

SpeedControll speed_controll_left;
SpeedControll speed_controll_right;

Direction direction_left;
Direction direction_right;

size_t speed_left = SPEED_DEFAULT;
size_t speed_right = SPEED_DEFAULT;

// ================
// PRIVATE
// ================

static void disable_left_speed()
{
#ifdef ESP32
    ledcWrite(PWM_CHANNEL_LEFT, 0);
#else
    digitalWrite(PIN_SPEED_LEFT, LOW);
#endif
}

static void enable_left_speed()
{
#ifdef ESP32
    ledcWrite(PWM_CHANNEL_LEFT, speed_left);
#else
    analogWrite(PIN_SPEED_LEFT, speed_left);
#endif
}

static void disable_right_speed()
{
#ifdef ESP32
    ledcWrite(PWM_CHANNEL_RIGHT, 0);
#else
    digitalWrite(PIN_SPEED_RIGHT, LOW);
#endif
}

static void enable_right_speed()
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

void forward_left()
{
    // disabling speed pin just in case
    disable_left_speed();
    digitalWrite(PIN_FRONT_LEFT, HIGH);
    digitalWrite(PIN_BACK_LEFT, LOW);
    direction_left = Direction::FORWARD;
    enable_left_speed();
    LOG_ENGINE_NL("Forward Left");
}

void forward_right()
{
    disable_right_speed();
    digitalWrite(PIN_FRONT_RIGHT, HIGH);
    digitalWrite(PIN_BACK_RIGHT, LOW);
    direction_right = Direction::FORWARD;
    enable_right_speed();
    LOG_ENGINE_NL("Forward Right");
}

void forward()
{
    forward_left();
    forward_right();
    LOG_ENGINE_NL("Both Forward");
}

void backward_left()
{
    disable_left_speed();
    digitalWrite(PIN_FRONT_LEFT, LOW);
    digitalWrite(PIN_BACK_LEFT, HIGH);
    direction_left = Direction::BACKWARD;
    enable_left_speed();
    LOG_ENGINE_NL("Backward Left");
}

void backward_right()
{
    disable_right_speed();
    digitalWrite(PIN_FRONT_RIGHT, LOW);
    digitalWrite(PIN_BACK_RIGHT, HIGH);
    direction_right = Direction::BACKWARD;
    enable_right_speed();
    LOG_ENGINE_NL("Backward Right");
}

void backward()
{
    backward_left();
    backward_right();
    LOG_ENGINE_NL("Both Backward");
}

void stop_left()
{
    disable_left_speed();
    digitalWrite(PIN_FRONT_LEFT, LOW);
    digitalWrite(PIN_BACK_LEFT, LOW);
    direction_left = Direction::STOP;
    LOG_ENGINE_NL("Stop Left");
}

void stop_right()
{
    disable_right_speed();
    digitalWrite(PIN_FRONT_RIGHT, LOW);
    digitalWrite(PIN_BACK_RIGHT, LOW);
    direction_right = Direction::STOP;
    LOG_ENGINE_NL("Stop Right");
}

void stop()
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

void slower()
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

void faster()
{
    faster_left();
    faster_right();
    LOG_ENGINE_NL("Both Faster");
}

void keep_speed_left()
{
    speed_controll_left = SpeedControll::STEADY;
    LOG_ENGINE_NL("Steady Left");
}

void keep_speed_right()
{
    speed_controll_right = SpeedControll::STEADY;
    LOG_ENGINE_NL("Steady Right");
}

void keep_speed()
{
    keep_speed_left();
    keep_speed_right();
    LOG_ENGINE_NL("Both Steady");
}

void forward_left(const CommandBuffer &b)
{
    forward_left();
}

void forward_right(const CommandBuffer &b)
{
    forward_right();
}

void forward(const CommandBuffer &b)
{
    forward();
}

void backward_left(const CommandBuffer &b)
{
    backward_left();
}

void backward_right(const CommandBuffer &b)
{
    backward_right();
}

void backward(const CommandBuffer &b)
{
    backward();
}

void stop_left(const CommandBuffer &b)
{
    stop_left();
}

void stop_right(const CommandBuffer &b)
{
    stop_right();
}

void stop(const CommandBuffer &b)
{
    stop();
}

void rotate_left(const CommandBuffer &b)
{
    rotate_left();
}

void rotate_right(const CommandBuffer &b)
{
    rotate_right();
}

void slower_left(const CommandBuffer &b)
{
    slower_left();
}

void slower_right(const CommandBuffer &b)
{
    slower_right();
}

void slower(const CommandBuffer &b)
{
    slower();
}

void faster_left(const CommandBuffer &b)
{
    faster_left();
}

void faster_right(const CommandBuffer &b)
{
    faster_right();
}

void faster(const CommandBuffer &b)
{
    faster();
}

void keep_speed_left(const CommandBuffer &b)
{
    keep_speed_left();
}

void keep_speed_right(const CommandBuffer &b)
{
    keep_speed_right();
}

void keep_speed(const CommandBuffer &b)
{
    keep_speed();
}

void update_speed()
{
    static unsigned long last_update = millis();
    if (millis() - last_update > SPEED_CHANGE_INTERVAL)
    {
        if (speed_controll_left == SpeedControll::SLOWER)
            if (speed_left > 0U)
                speed_left--;

        if (speed_controll_right == SpeedControll::SLOWER)
            if (speed_right > 0U)
                speed_right--;

        if (speed_controll_left == SpeedControll::FASTER)
            if (speed_left < SPEED_MAX)
                speed_left++;

        if (speed_controll_right == SpeedControll::FASTER)
            if (speed_right < SPEED_MAX)
                speed_right++;

        last_update = millis();
        LOG_ENGINE_NL("Updated Speed");
    }
}
