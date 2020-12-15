#ifndef __MP3_H__
#define __MP3_H__

#include <Arduino.h>
#include <MD_YX5300.h>
#include "abstract/templated_controller.hpp"

namespace json_parser
{
    class mp3_controller final : public templated_controller<mp3_controller>
    {
    public:
        explicit mp3_controller();
        bool initialize() override;
        void update() override
        {}
        DynamicJsonDocument retrive_data() override;

    private:
        bool stop_playing(const JsonObject *json);
        bool set_volume(const JsonObject *json);
        bool resume(const JsonObject *json);

        bool windows_xp(const JsonObject *json);
        bool mighty_polish_tank(const JsonObject *json);
        bool high_ground(const JsonObject *json);
        bool fine_addition(const JsonObject *json);
        bool i_dont_like_sand(const JsonObject *json);
        bool hello_there(const JsonObject *json);
        bool im_the_senate(const JsonObject *json);
        bool forever_young(const JsonObject *json);
        bool revenge(const JsonObject *json);
        bool silhouette(const JsonObject *json);
        bool the_bad_touch(const JsonObject *json);
        bool hero(const JsonObject *json);
        bool gas_gas_gas(const JsonObject *json);
        bool running_in_the_90s(const JsonObject *json);
        bool deja_vu(const JsonObject *json);
        bool running_in_the_90s_short(const JsonObject *json);
        bool deja_vu_short(const JsonObject *json);
        bool true_survivor(const JsonObject *json);
        bool propaganda(const JsonObject *json);
        bool giorno(const JsonObject *json);
        bool noble_pope(const JsonObject *json);
        bool torture_dance(const JsonObject *json);
        bool awaken(const JsonObject *json);
        bool dio_vs_jotaro(const JsonObject *json);

        static constexpr const char *MIGHTY_POLISH_TANK = "mighty_polish_tank";
        static constexpr const char *WINDOWS_XP = "windows_xp";
        static constexpr const char *HIGH_GROUND = "high_ground";
        static constexpr const char *FINE_ADDITION = "fine_addition";
        static constexpr const char *I_DONT_LIKE_SAND = "i_dont_like_sand";
        static constexpr const char *HELLO_THERE = "hello_there";
        static constexpr const char *IM_THE_SENATE = "im_the_senate";
        static constexpr const char *FOREVER_YOUNG = "forever_young";
        static constexpr const char *REVENGE = "revenge";
        static constexpr const char *SILHOUETTE = "silhouette";
        static constexpr const char *THE_BAD_TOUCH = "the_bad_touch";
        static constexpr const char *HERO = "hero";
        static constexpr const char *GAS_GAS_GAS = "gas_gas_gas";
        static constexpr const char *RUNNING_IN_THE_90S = "running_in_the_90s";
        static constexpr const char *DEJA_VU = "deja_vu";
        static constexpr const char *RUNNING_IN_THE_90S_SHORT = "running_in_the_90s_short";
        static constexpr const char *DEJA_VU_SHORT = "deja_vu_short";
        static constexpr const char *TRUE_SURVIVOR = "true_survivor";
        static constexpr const char *PROPAGANDA = "propaganda";
        static constexpr const char *NOBLE_POPE = "noble_pope";
        static constexpr const char *GIORNO = "giorno";
        static constexpr const char *TORTURE_DANCE = "torture_dance";
        static constexpr const char *AWAKEN = "awaken";
        static constexpr const char *DIO_VS_JOTARO = "dio_vs_jotaro";

        static constexpr const char *STOP = "stop";
        static constexpr const char *RESUME = "resume";
        static constexpr const char *SET_VOLUME = "set_volume";

        static constexpr const char *VOLUME_KEY = "volume";

        MD_YX5300 _mp3;
        uint8_t _volume = 15;
        const char *_last_song = nullptr;
    };
} // namespace json_parser

#endif //__MP3_H__