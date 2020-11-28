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

        static constexpr uint8_t DEF_VOLUME = 15U;

        static constexpr const char *PROPAGANDA = "propaganda";
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

        static constexpr const char *STOP = "stop";
        static constexpr const char *RESUME = "resume";
        static constexpr const char *SET_VOLUME = "set_volume";

        static constexpr const char *VOLUME_KEY = "volume";

        MD_YX5300 mp3;
    };
} // namespace json_parser

#endif //__MP3_H__