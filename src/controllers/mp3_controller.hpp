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
        void update() override;

    private:
        bool stop_playing(const JsonObjectConst *json);
        bool set_volume(const JsonObjectConst *json);
        bool resume(const JsonObjectConst *json);

        bool propaganda(const JsonObjectConst *json);
        bool mighty_polish_tank(const JsonObjectConst *json);
        bool high_ground(const JsonObjectConst *json);
        bool fine_addition(const JsonObjectConst *json);
        bool i_dont_like_sand(const JsonObjectConst *json);
        bool hello_there(const JsonObjectConst *json);
        bool im_the_senate(const JsonObjectConst *json);
        bool forever_young(const JsonObjectConst *json);
        bool revenge(const JsonObjectConst *json);
        bool silhouette(const JsonObjectConst *json);
        bool the_bad_touch(const JsonObjectConst *json);
        bool hero(const JsonObjectConst *json);
        bool gas_gas_gas(const JsonObjectConst *json);
        bool running_in_the_90s(const JsonObjectConst *json);
        bool deja_vu(const JsonObjectConst *json);
        bool running_in_the_90s_short(const JsonObjectConst *json);
        bool deja_vu_short(const JsonObjectConst *json);
        bool true_survivor(const JsonObjectConst *json);
        bool windows_xp(const JsonObjectConst *json);


    private:
        static constexpr const char *PROPAGANDA = "PROPAGANDA";
        static constexpr const char *MIGHTY_POLISH_TANK = "MIGHTY_POLISH_TANK";
        static constexpr const char *WINDOWS_XP = "WINDOWS_XP";
        static constexpr const char *HIGH_GROUND = "HIGH_GROUND";
        static constexpr const char *FINE_ADDITION = "FINE_ADDITION";
        static constexpr const char *I_DONT_LIKE_SAND = "I_DONT_LIKE_SAND";
        static constexpr const char *HELLO_THERE = "HELLO_THERE";
        static constexpr const char *IM_THE_SENATE = "IM_THE_SENATE";
        static constexpr const char *FOREVER_YOUNG = "FOREVER_YOUNG";
        static constexpr const char *REVENGE = "REVENGE";
        static constexpr const char *SILHOUETTE = "SILHOUETTE";
        static constexpr const char *THE_BAD_TOUCH = "THE_BAD_TOUCH";
        static constexpr const char *HERO = "HERO";
        static constexpr const char *GAS_GAS_GAS = "GAS_GAS_GAS";
        static constexpr const char *RUNNING_IN_THE_90S = "RUNNING_IN_THE_90S";
        static constexpr const char *DEJA_VU = "DEJA_VU";
        static constexpr const char *RUNNING_IN_THE_90S_SHORT = "RUNNING_IN_THE_90S_SHORT";
        static constexpr const char *DEJA_VU_SHORT = "DEJA_VU_SHORT";
        static constexpr const char *TRUE_SURVIVOR = "TRUE_SURVIVOR";

        static constexpr const char *STOP = "STOP";
        static constexpr const char *RESUME = "RESUME";
        static constexpr const char *SET_VOLUME = "SET_VOLUME";

        static constexpr const char *VOLUME_KEY = "volume";

        MD_YX5300 mp3;
        uint8_t volume = 15U;
    };
} // namespace json_parser

#endif //__MP3_H__