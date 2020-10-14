#ifndef __MP3_H__
#define __MP3_H__

#include <Arduino.h>
#include "buffer/buffer.h"

namespace mp3
{
    namespace commands
    {
        extern const char *MP3_PROPAGANDA;
        extern const char *MP3_MIGHTY_POLISH_TANK;
        extern const char *MP3_WINDOWS_XP;
        extern const char *MP3_HIGH_GROUND;
        extern const char *MP3_FINE_ADDITION;
        extern const char *MP3_I_DONT_LIKE_SAND;
        extern const char *MP3_HELLO_THERE;
        extern const char *MP3_IM_THE_SENATE;
        extern const char *MP3_FOREVER_YOUNG;
        extern const char *MP3_REVENGE;
        extern const char *MP3_SILHOUETTE;
        extern const char *MP3_THE_BAD_TOUCH;
        extern const char *MP3_HERO;
        extern const char *MP3_GAS_GAS_GAS;
        extern const char *MP3_RUNNING_IN_THE_90S;
        extern const char *MP3_DEJA_VU;
        extern const char *MP3_RUNNING_IN_THE_90S_SHORT;
        extern const char *MP3_DEJA_VU_SHORT;
        extern const char *MP3_TRUE_SURVIVOR;

        extern const char *MP3_STOP;
        extern const char *MP3_RESUME;
        extern const char *MP3_SET_VOLUME;
    } // namespace commands

    void init_mp3();

    void stop_playing(const CommandBuffer &b);
    void set_volume(const CommandBuffer &b);
    void resume(const CommandBuffer& b);

    void propaganda(const CommandBuffer &b);
    void mighty_polish_tank(const CommandBuffer &b);
    void high_ground(const CommandBuffer &b);
    void fine_addition(const CommandBuffer &b);
    void i_dont_like_sand(const CommandBuffer &b);
    void hello_there(const CommandBuffer &b);
    void im_the_senate(const CommandBuffer &b);
    void forever_young(const CommandBuffer &b);
    void revenge(const CommandBuffer &b);
    void silhouette(const CommandBuffer &b);
    void the_bad_touch(const CommandBuffer &b);
    void hero(const CommandBuffer &b);
    void gas_gas_gas(const CommandBuffer &b);
    void running_in_the_90s(const CommandBuffer &b);
    void deja_vu(const CommandBuffer &b);
    void running_in_the_90s_short(const CommandBuffer &b);
    void deja_vu_short(const CommandBuffer &b);
    void true_survivor(const CommandBuffer &b);
    void windows_xp(const CommandBuffer &b);

} // namespace mp3

#endif //__MP3_H__