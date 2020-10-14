#ifndef __ENGINES_H__
#define __ENGINES_H__

#include <Arduino.h>
#include "buffer/buffer.h"

namespace engines
{
    namespace commands
    {
        extern const char *FORWARD;
        extern const char *FORWARD_LEFT;
        extern const char *FORWARD_RIGHT;
        extern const char *BACKWARD;
        extern const char *BACKWARD_LEFT;
        extern const char *BACKWARD_RIGHT;
        extern const char *STOP;
        extern const char *STOP_LEFT;
        extern const char *STOP_RIGHT;
        extern const char *FASTER;
        extern const char *FASTER_LEFT;
        extern const char *FASTER_RIGHT;
        extern const char *SLOWER;
        extern const char *SLOWER_LEFT;
        extern const char *SLOWER_RIGHT;
        extern const char *KEEP_SPEED;
        extern const char *KEEP_SPEED_LEFT;
        extern const char *KEEP_SPEED_RIGHT;
        extern const char *SPEED;
        extern const char *SPEED_LEFT;
        extern const char *SPEED_RIGHT;
    } // namespace commands

    enum class speed_controll
    {
        SLOWER,
        FASTER,
        KEEP_SPEED
    };

    enum class engine_direction
    {
        BACKWARD,
        FORWARD,
        STOP
    };

    extern speed_controll speed_controll_left;
    extern speed_controll speed_controll_right;

    extern engine_direction direction_left;
    extern engine_direction direction_right;

    extern size_t speed_left;
    extern size_t speed_right;

    // ================
    // EXPORT
    // ================

    void init_engines();

    void forward(const CommandBuffer &b);
    void forward_left(const CommandBuffer &b);
    void forward_right(const CommandBuffer &b);

    void backward(const CommandBuffer &b);
    void backward_left(const CommandBuffer &b);
    void backward_right(const CommandBuffer &b);

    void stop(const CommandBuffer &b);
    void stop_left(const CommandBuffer &b);
    void stop_right(const CommandBuffer &b);

    void rotate_left(const CommandBuffer &b);
    void rotate_right(const CommandBuffer &b);

    void slower(const CommandBuffer &b);
    void slower_left(const CommandBuffer &b);
    void slower_right(const CommandBuffer &b);

    void faster(const CommandBuffer &b);
    void faster_left(const CommandBuffer &b);
    void faster_right(const CommandBuffer &b);

    void keep_speed(const CommandBuffer &b);
    void keep_speed_left(const CommandBuffer &b);
    void keep_speed_right(const CommandBuffer &b);

    void set_speed(const CommandBuffer &b);
    void set_speed_left(const CommandBuffer &b);
    void set_speed_right(const CommandBuffer &b);

    void forward();
    void forward_left();
    void forward_right();

    void backward();
    void backward_left();
    void backward_right();

    void stop();
    void stop_left();
    void stop_right();

    void rotate_left();
    void rotate_right();

    void slower();
    void slower_left();
    void slower_right();

    void faster();
    void faster_left();
    void faster_right();

    void keep_speed();
    void keep_speed_left();
    void keep_speed_right();

    void set_speed(size_t new_speed);
    void set_speed_left(size_t new_speed);
    void set_speed_right(size_t new_speed);

    void update_speed();

} // namespace engines

#endif // __ENGINES_H__