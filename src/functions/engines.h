#ifndef __ENGINES_H__
#define __ENGINES_H__

#include <Arduino.h>
#include "buffer/buffer.h"
#include "typedefs.h"
#include "debug.h"

namespace driving
{
    namespace commands
    {
        extern const char *FORWARD;
        extern const char *BACKWARD;
        extern const char *STOP;
        extern const char *FASTER;
        extern const char *SLOWER;
        extern const char *KEEP_SPEED;
        extern const char *SET_SPEED;

        namespace modifiers
        {
            extern const char *LEFT;
            extern const char *RIGHT;
            extern const char *BOTH;
        } // namespace modifiers
    }     // namespace commands

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
    void backward(const CommandBuffer &b);
    void stop(const CommandBuffer &b);
    void rotate(const CommandBuffer &b);
    void slower(const CommandBuffer &b);
    void faster(const CommandBuffer &b);
    void keep_speed(const CommandBuffer &b);
    void set_speed(const CommandBuffer &b);

    void forward_left();
    void forward_right();
    void forward();

    void backward_left();
    void backward_right();
    void backward_both();

    void stop_left();
    void stop_right();
    void stop_both();

    void rotate_left();
    void rotate_right();

    void slower_left();
    void slower_right();
    void slower_both();

    void faster_left();
    void faster_right();
    void faster_both();

    void keep_speed_left();
    void keep_speed_right();
    void keep_speed_both();

    void set_speed_left(size_t new_speed);
    void set_speed_right(size_t new_speed);
    void set_speed_both(size_t new_speed);

    void update_speed();

} // namespace driving

#endif // __ENGINES_H__