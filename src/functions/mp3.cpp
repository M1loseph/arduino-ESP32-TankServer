#include "mp3.h"
#include "debug.h"
#include <MD_YX5300.h>

#if MP3_DEBUG

#define LOG_MP3(message) LOG(message)
#define LOG_MP3_NL(message) LOG_NL(message)
#define LOG_MP3_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_MP3(message)
#define LOG_MP3_NL(message)
#define LOG_MP3_F(...)

#endif // MP3_DEBUG

namespace mp3
{
    // ================
    // VARIABLES
    // ================

    namespace commands
    {
        const char *MP3_PROPAGANDA = "MP3_PROPAGANDA";
        const char *MP3_MIGHTY_POLISH_TANK = "MP3_MIGHTY_POLISH_TANK";
        const char *MP3_WINDOWS_XP = "MP3_WINDOWS_XP";
        const char *MP3_HIGH_GROUND = "MP3_HIGH_GROUND";
        const char *MP3_FINE_ADDITION = "MP3_FINE_ADDITION";
        const char *MP3_I_DONT_LIKE_SAND = "MP3_I_DONT_LIKE_SAND";
        const char *MP3_HELLO_THERE = "MP3_HELLO_THERE";
        const char *MP3_IM_THE_SENATE = "MP3_IM_THE_SENATE";
        const char *MP3_FOREVER_YOUNG = "MP3_FOREVER_YOUNG";
        const char *MP3_REVENGE = "MP3_REVENGE";
        const char *MP3_SILHOUETTE = "MP3_SILHOUETTE";
        const char *MP3_THE_BAD_TOUCH = "MP3_THE_BAD_TOUCH";
        const char *MP3_HERO = "MP3_HERO";
        const char *MP3_GAS_GAS_GAS = "MP3_GAS_GAS_GAS";
        const char *MP3_RUNNING_IN_THE_90S = "MP3_RUNNING_IN_THE_90S";
        const char *MP3_DEJA_VU = "MP3_DEJA_VU";
        const char *MP3_RUNNING_IN_THE_90S_SHORT = "MP3_RUNNING_IN_THE_90S_SHORT";
        const char *MP3_DEJA_VU_SHORT = "MP3_DEJA_VU_SHORT";
        const char *MP3_TRUE_SURVIVOR = "MP3_TRUE_SURVIVOR";

        const char *MP3_STOP = "MP3_STOP";
        const char *MP3_RESUME = "MP3_RESUME";
        const char *MP3_SET_VOLUME = "MP3_SET_VOLUME";
    } // namespace commands

    // ================
    // CONFIG
    // ================

    MD_YX5300 mp3(Serial2);
    uint8_t volume = 15;

    void init_mp3()
    {
        Serial2.begin(9600);
        mp3.begin();
        mp3.setSynchronous(false);
        mp3.volume(volume);
        mp3.playSpecific(4, 19);
    }

    void stop_playing(const CommandBuffer &b)
    {
        LOG_MP3_NL("Stop playback");
        mp3.playStop();
    }

    void set_volume(const CommandBuffer &b)
    {
        auto new_volume = b.int_at(1);
        if (new_volume.success && new_volume.i >= 0 && new_volume.i < mp3.volumeMax())
        {
            LOG_MP3_F("New volume: %d\n", new_volume.i);
            mp3.volume(static_cast<uint8_t>(new_volume.i));
        }
        else
        {
            LOG_MP3_F("Wrong new volume - success: %d, value: %d\n", new_volume.success, new_volume.i);
        }
    }

    void resume(const CommandBuffer &b)
    {
        LOG_MP3_NL("Resuming song");
        mp3.playStart();
    }

    void propaganda(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing propaganda");
        mp3.playSpecific(1, 1);
    }

    void mighty_polish_tank(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing mighty polish tank");
        mp3.playSpecific(1, 2);
    }

    void high_ground(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing high ground");
        mp3.playSpecific(2, 3);
    }

    void fine_addition(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing fine addition");
        mp3.playSpecific(2, 4);
    }

    void i_dont_like_sand(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing i dont like sand");
        mp3.playSpecific(2, 5);
    }

    void hello_there(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing hello there");
        mp3.playSpecific(2, 6);
    }

    void im_the_senate(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing im the senate");
        mp3.playSpecific(2, 7);
    }

    void forever_young(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing forever young");
        mp3.playSpecific(3, 8);
    }

    void revenge(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing revenge");
        mp3.playSpecific(3, 9);
    }

    void silhouette(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing naruto opening");
        mp3.playSpecific(3, 10);
    }

    void the_bad_touch(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing the bad touch");
        mp3.playSpecific(3, 11);
    }

    void hero(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing i need a hero");
        mp3.playSpecific(3, 12);
    }

    void gas_gas_gas(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing gas gas gas");
        mp3.playSpecific(3, 13);
    }

    void running_in_the_90s(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing running in the 90s");
        mp3.playSpecific(3, 14);
    }

    void deja_vu(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing deja vu");
        mp3.playSpecific(3, 15);
    }

    void running_in_the_90s_short(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing running in the 90s - short version");
        mp3.playSpecific(3, 16);
    }

    void deja_vu_short(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing deja vu - short version");
        mp3.playSpecific(3, 17);
    }

    void true_survivor(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing true survivor");
        mp3.playSpecific(3, 18);
    }

    void windows_xp(const CommandBuffer &b)
    {
        LOG_MP3_NL("Playing windows starup song");
        mp3.playSpecific(4, 19);
    }
} // namespace mp3