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
    mp3_controller::mp3_controller() : templated_controller("mp3", JSON_OBJECT_SIZE(4)),
                                       _mp3(Serial2)
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
        if_added &= add_event(WINDOWS_XP, &mp3_controller::windows_xp);
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
        if_added &= add_event(PROPAGANDA, &mp3_controller::propaganda);
        if_added &= add_event(GIORNO, &mp3_controller::giorno);
        if_added &= add_event(NOBLE_POPE, &mp3_controller::noble_pope);
        if_added &= add_event(TORTURE_DANCE, &mp3_controller::torture_dance);
        if_added &= add_event(AWAKEN, &mp3_controller::awaken);
        if_added &= add_event(DIO_VS_JOTARO, &mp3_controller::dio_vs_jotaro);

        if_added &= add_event(STOP, &mp3_controller::stop_playing);
        if_added &= add_event(SET_VOLUME, &mp3_controller::set_volume);
        if_added &= add_event(RESUME, &mp3_controller::resume);

        return if_added;
    }

    bool mp3_controller::stop_playing(const JsonObject *json)
    {
        LOG_MP3_F("[%s] stop playback\n", _name)
        _mp3.playStop();
        return true;
    }

    bool mp3_controller::set_volume(const JsonObject *json)
    {
        if (json)
        {
            if (json->containsKey(VOLUME_KEY))
            {
                uint8_t new_volume = (*json)[VOLUME_KEY];
                if (new_volume <= _mp3.volumeMax())
                {
                    _mp3.volume(new_volume);
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

    bool mp3_controller::resume(const JsonObject *json)
    {
        LOG_MP3_F("[%s] resuming song\n", _name)
        _mp3.playStart();
        return true;
    }

    bool mp3_controller::windows_xp(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, WINDOWS_XP);
        _last_song = WINDOWS_XP;
        _mp3.playSpecific(1, 1);
        return true;
    }

    bool mp3_controller::mighty_polish_tank(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, MIGHTY_POLISH_TANK)
        _last_song = MIGHTY_POLISH_TANK;
        _mp3.playSpecific(1, 2);
        return true;
    }

    bool mp3_controller::high_ground(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, HIGH_GROUND)
        _last_song = HIGH_GROUND;
        _mp3.playSpecific(2, 3);
        return true;
    }

    bool mp3_controller::fine_addition(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, FINE_ADDITION)
        _last_song = FINE_ADDITION;
        _mp3.playSpecific(2, 4);
        return true;
    }

    bool mp3_controller::i_dont_like_sand(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, I_DONT_LIKE_SAND)
        _last_song = I_DONT_LIKE_SAND;
        _mp3.playSpecific(2, 5);
        return true;
    }

    bool mp3_controller::hello_there(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, HELLO_THERE)
        _last_song = HELLO_THERE;
        _mp3.playSpecific(2, 6);
        return true;
    }

    bool mp3_controller::im_the_senate(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, IM_THE_SENATE)
        _last_song = IM_THE_SENATE;
        _mp3.playSpecific(2, 7);
        return true;
    }

    bool mp3_controller::forever_young(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, FOREVER_YOUNG)
        _last_song = FOREVER_YOUNG;
        _mp3.playSpecific(3, 8);
        return true;
    }

    bool mp3_controller::revenge(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, REVENGE)
        _last_song = REVENGE;
        _mp3.playSpecific(3, 9);
        return true;
    }

    bool mp3_controller::silhouette(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, SILHOUETTE)
        _last_song = SILHOUETTE;
        _mp3.playSpecific(3, 10);
        return true;
    }

    bool mp3_controller::the_bad_touch(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, THE_BAD_TOUCH)
        _last_song = THE_BAD_TOUCH;
        _mp3.playSpecific(3, 11);
        return true;
    }

    bool mp3_controller::hero(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, HERO)
        _last_song = HERO;
        _mp3.playSpecific(3, 12);
        return true;
    }

    bool mp3_controller::gas_gas_gas(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, GAS_GAS_GAS)
        _last_song = GAS_GAS_GAS;
        _mp3.playSpecific(3, 13);
        return true;
    }

    bool mp3_controller::running_in_the_90s(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, RUNNING_IN_THE_90S)
        _last_song = RUNNING_IN_THE_90S;
        _mp3.playSpecific(3, 14);
        return true;
    }

    bool mp3_controller::deja_vu(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, DEJA_VU)
        _last_song = DEJA_VU;
        _mp3.playSpecific(3, 15);
        return true;
    }

    bool mp3_controller::running_in_the_90s_short(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, RUNNING_IN_THE_90S_SHORT)
        _last_song = RUNNING_IN_THE_90S_SHORT;
        _mp3.playSpecific(3, 16);
        return true;
    }

    bool mp3_controller::deja_vu_short(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, DEJA_VU_SHORT)
        _last_song = DEJA_VU_SHORT;
        _mp3.playSpecific(3, 17);
        return true;
    }

    bool mp3_controller::true_survivor(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, TRUE_SURVIVOR)
        _last_song = TRUE_SURVIVOR;
        _mp3.playSpecific(3, 18);
        return true;
    }

    bool mp3_controller::propaganda(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, PROPAGANDA)
        _last_song = PROPAGANDA;
        _mp3.playSpecific(4, 19);
        return true;
    }

    bool mp3_controller::giorno(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, GIORNO)
        _last_song = GIORNO;
        _mp3.playSpecific(4, 20);
        return true;
    }

    bool mp3_controller::noble_pope(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, NOBLE_POPE)
        _last_song = NOBLE_POPE;
        _mp3.playSpecific(4, 21);
        return true;
    }

    bool mp3_controller::torture_dance(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, TORTURE_DANCE)
        _last_song = TORTURE_DANCE;
        _mp3.playSpecific(4, 22);
        return true;
    }

    bool mp3_controller::awaken(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, AWAKEN)
        _last_song = AWAKEN;
        _mp3.playSpecific(4, 23);
        return true;
    }

    bool mp3_controller::dio_vs_jotaro(const JsonObject *json)
    {
        LOG_MP3_F("[%s] playing %s\n", _name, DIO_VS_JOTARO)
        _last_song = DIO_VS_JOTARO;
        _mp3.playSpecific(4, 24);
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