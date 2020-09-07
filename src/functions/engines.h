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

#ifdef ESP_32
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

extern size_t current_speed_left;
extern size_t current_speed_right;

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

void steady_left(const CommandBuffer &b);
void steady_right(const CommandBuffer &b);
void steady(const CommandBuffer &b);

void update_speed();

#endif // __ENGINES_H__