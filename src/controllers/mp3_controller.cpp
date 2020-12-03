#include "mp3_controller.hpp"
#include "debug.hpp"

#if MP3_DEBUG

#define LOG_MP3(message) LOG(message)
#define LOG_MP3_NL(message) LOG_NL(message)
#define LOG_MP3_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_MP3(message)
#define LOG_MP3_NL(message)
#define LOG_MP3_F(...)

#endif // MP3_DEBUG

namespace json_parser
{
    mp3_controller::mp3_controller() : templated_controller("mp3"),
                                       _mp3(Serial2),
                                       _volume(DEF_VOLUME),
                                       _last_song(nullptr)
    {
    }

    bool mp3_controller::initialize()
    {
        Serial2.begin(9600);
        _mp3.begin();
        _mp3.setSynchronous(false);
        _mp3.volume(_volume);
        windows_xp(nullptr);

        bool if_added = true;
        if_added &= add_event(PROPAGANDA, &mp3_controller::propaganda);
        if_added &= add_event(MIGHTY_POLISH_TANK, &mp3_controller::mighty_polish_tank);
        if_added &= add_event(HIGH_GROUND, &mp3_controller::high_ground);
        if_added &= add_event(FINE_ADDITION, &mp3_controller::fine_addition);
        if_added &= add_event(I_DONT_LIKE_SAND, &mp3_controller::i_dont_like_sand);
        if_added &= add_event(HELLO_THERE, &mp3_controller::hello_there);
        if_added &= add_event(IM_THE_SENATE, &mp3_controller::im_the_senate);
        if_added &= add_event(FOREVER_YOUNG, &mp3_controller::forever_young);
        if_added &= add_event(REVENGE, &mp3_controller::revenge);
        if_added &= add_event(SILHOUETTE, &mp3_controller::silhouette);
        if_added &= add_event(THE_BAD_TOUCH, &mp3_controller::the_bad_touch);
        if_added &= add_event(HERO, &mp3_controller::hero);
        if_added &= add_event(GAS_GAS_GAS, &mp3_controller::gas_gas_gas);
        if_added &= add_event(RUNNING_IN_THE_90S, &mp3_controller::running_in_the_90s);
        if_added &= add_event(DEJA_VU, &mp3_controller::deja_vu);
        if_added &= add_event(RUNNING_IN_THE_90S_SHORT, &mp3_controller::running_in_the_90s_short);
        if_added &= add_event(DEJA_VU_SHORT, &mp3_controller::deja_vu_short);
        if_added &= add_event(TRUE_SURVIVOR, &mp3_controller::true_survivor);
        if_added &= add_event(WINDOWS_XP, &mp3_controller::windows_xp);
        if_added &= add_event(STOP, &mp3_controller::stop_playing);
        if_added &= add_event(SET_VOLUME, &mp3_controller::set_volume);
        if_added &= add_event(RESUME, &mp3_controller::resume);

        return if_added;
    }

    bool mp3_controller::stop_playing(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] stop playback\n", _name)
        _mp3.playStop();
        return true;
    }

    bool mp3_controller::set_volume(const JsonObjectConst *json)
    {
        if (json)
        {
            if (json->containsKey(VOLUME_KEY))
            {
                uint8_t new_volume = (*json)[VOLUME_KEY];
                if (_mp3.volume(new_volume))
                {
                    _volume = new_volume;
                    LOG_MP3_F("[%s] new volume: %d\n", _name, new_volume)
                    return true;
                }
                else
                {
                    LOG_MP3_F("[%s] volume too big: %d\n", _name, new_volume)
                }
            }
            else
            {
                LOG_MP3_F("[%s] JSON didn't have volume key\n", _name)
            }
        }
        else
        {
            LOG_MP3_F("[%s] JSON was null\n", _name)
        }
        return false;
    }

    bool mp3_controller::resume(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] resuming song\n", _name)
        _mp3.playStart();
        return true;
    }

    bool mp3_controller::propaganda(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing propaganda\n", _name)
        _last_song = PROPAGANDA;
        _mp3.playSpecific(1, 1);
        return true;
    }

    bool mp3_controller::mighty_polish_tank(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing mighty polish tank\n", _name)
        _last_song = MIGHTY_POLISH_TANK;
        _mp3.playSpecific(1, 2);
        return true;
    }

    bool mp3_controller::high_ground(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing high ground\n", _name)
        _last_song = HIGH_GROUND;
        _mp3.playSpecific(2, 3);
        return true;
    }

    bool mp3_controller::fine_addition(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing fine addition\n", _name)
        _last_song = FINE_ADDITION;
        _mp3.playSpecific(2, 4);
        return true;
    }

    bool mp3_controller::i_dont_like_sand(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing i dont like sand\n", _name)
        _last_song = I_DONT_LIKE_SAND;
        _mp3.playSpecific(2, 5);
        return true;
    }

    bool mp3_controller::hello_there(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing hello there\n", _name)
        _last_song = HELLO_THERE;
        _mp3.playSpecific(2, 6);
        return true;
    }

    bool mp3_controller::im_the_senate(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing im the senate\n", _name)
        _last_song = IM_THE_SENATE;
        _mp3.playSpecific(2, 7);
        return true;
    }

    bool mp3_controller::forever_young(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing forever young\n", _name)
        _last_song = FOREVER_YOUNG;
        _mp3.playSpecific(3, 8);
        return true;
    }

    bool mp3_controller::revenge(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing revenge\n", _name)
        _last_song = REVENGE;
        _mp3.playSpecific(3, 9);
        return true;
    }

    bool mp3_controller::silhouette(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing naruto opening\n", _name)
        _last_song = REVENGE;
        _mp3.playSpecific(3, 10);
        return true;
    }

    bool mp3_controller::the_bad_touch(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing the bad touch\n", _name)
        _last_song = THE_BAD_TOUCH;
        _mp3.playSpecific(3, 11);
        return true;
    }

    bool mp3_controller::hero(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing i need a hero\n", _name)
        _last_song = HERO;
        _mp3.playSpecific(3, 12);
        return true;
    }

    bool mp3_controller::gas_gas_gas(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing gas gas gas\n", _name)
        _last_song = GAS_GAS_GAS;
        _mp3.playSpecific(3, 13);
        return true;
    }

    bool mp3_controller::running_in_the_90s(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing running in the 90s\n", _name)
        _last_song = RUNNING_IN_THE_90S;
        _mp3.playSpecific(3, 14);
        return true;
    }

    bool mp3_controller::deja_vu(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing deja vu\n", _name)
        _last_song = DEJA_VU;
        _mp3.playSpecific(3, 15);
        return true;
    }

    bool mp3_controller::running_in_the_90s_short(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing running in the 90s - short version\n", _name)
        _last_song = RUNNING_IN_THE_90S_SHORT;
        _mp3.playSpecific(3, 16);
        return true;
    }

    bool mp3_controller::deja_vu_short(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing deja vu - short version\n", _name)
        _last_song = DEJA_VU_SHORT;
        _mp3.playSpecific(3, 17);
        return true;
    }

    bool mp3_controller::true_survivor(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing true survivor\n", _name)
        _last_song = TRUE_SURVIVOR;
        _mp3.playSpecific(3, 18);
        return true;
    }

    bool mp3_controller::windows_xp(const JsonObjectConst *json)
    {
        LOG_MP3_F("[%s] playing windows starup song\n", _name);
        _last_song = WINDOWS_XP;
        _mp3.playSpecific(4, 19);
        return true;
    }

    DynamicJsonDocument mp3_controller::retrive_data()
    {
        DynamicJsonDocument json(100);
        json[NAME_FIELD] = _name;
        JsonObject data = json.createNestedObject(DATA_FIELD);
        data["playing"] = _last_song;
        data[VOLUME_KEY] = _volume;
        return json;
    }
} // namespace json_parser