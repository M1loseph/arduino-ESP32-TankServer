#ifndef __ENGINES_H__
#define __ENGINES_H__

#include <Arduino.h>
#include "buffer/buffer.h"
#include "typedefs.h"
#include "debug.h"

// ================
// PINS
// ================

// TODO => FIX PIN NUMBERS
static constexpr uchar PIN_FRONT_RIGHT = 27;
static constexpr uchar PIN_BACK_RIGHT = 26;
static constexpr uchar PIN_FRONT_LEFT = 12;
static constexpr uchar PIN_BACK_LEFT = 14;

static constexpr uchar PIN_SPEED_LEFT = 13;
static constexpr uchar PIN_SPEED_RIGHT = 25;

// ================
// CONFIGURATION
// ================

static constexpr size_t SPEED_MAX = 1023U;
static constexpr size_t SPEED_DEFAULT = 500U;
static constexpr size_t SPEED_CHANGE_INTERVAL = 5U;

#ifdef ESP32
static constexpr uchar PWM_CHANNEL_LEFT = 1U;
static constexpr uchar PWM_CHANNEL_RIGHT = 2U;
// in bits
static constexpr uchar PWM_RESOLUTION = 10U;
// Hz
static constexpr size_t PWM_FREQUENCY = 1000U;
#endif

// ================
// EXPORT
// ================

enum class SpeedControll
{
    SLOWER,
    FASTER,
    STEADY
};

enum class Direction
{
    BACKWARD,
    FORWARD,
    STOP
};

extern SpeedControll speed_controll_left;
extern SpeedControll speed_controll_right;

extern Direction direction_left;
extern Direction direction_right;

extern size_t speed_left;
extern size_t speed_right;

// ================
// EXPORT
// ================

void init_engines();

void forward_left(const CommandBuffer &b);
void forward_right(const CommandBuffer &b);
void forward(const CommandBuffer &b);

void backward_left(const CommandBuffer &b);
void backward_right(const CommandBuffer &b);
void backward(const CommandBuffer &b);

void stop_left(const CommandBuffer &b);
void stop_right(const CommandBuffer &b);
void stop(const CommandBuffer &b);

void rotate_left(const CommandBuffer &b);
void rotate_right(const CommandBuffer &b);

void slower_left(const CommandBuffer &b);
void slower_right(const CommandBuffer &b);
void slower(const CommandBuffer &b);

void faster_left(const CommandBuffer &b);
void faster_right(const CommandBuffer &b);
void faster(const CommandBuffer &b);

void keep_speed_left(const CommandBuffer &b);
void keep_speed_right(const CommandBuffer &b);
void keep_speed(const CommandBuffer &b);

void forward_left();
void forward_right();
void forward();

void backward_left();
void backward_right();
void backwardb();

void stop_left();
void stop_right();
void stop();

void rotate_left();
void rotate_right();

void slower_left();
void slower_right();
void slower();

void faster_left();
void faster_right();
void faster();

void keep_speed_left();
void keep_speed_right();
void keep_speed();

void update_speed();

#endif // __ENGINES_H__