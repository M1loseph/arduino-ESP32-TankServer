#include "sd_controller.hpp"
#include "debug.hpp"

#if SD_DEBUG

#define LOG_SD(message) LOG(message)
#define LOG_SD_NL(message) LOG_NL(message)
#define LOG_SD_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_SD(message)
#define LOG_SD_NL(message)
#define LOG_SD_F(...)

#endif // SD_DEBUG

namespace json_parser
{
    sd_controller::sd_controller() : controller("sd")
    {
    }

    void sd_controller::update()
    {
    }

    bool sd_controller::initialize()
    {
        return SD.begin(CHIP_SELECT);
    }

    bool sd_controller::can_handle(const JsonObjectConst &json) const
    {
        if (json.containsKey(LOG_KEY))
        {
            return json[LOG_KEY].as<bool>();
        }
        return true;
    }

    bool sd_controller::handle(const JsonObjectConst &json)
    {
        _file = SD.open(LOG_FILE);
        if(_file)
        {
            serializeJson(json, _file);
            LOG_SD_F("[%s] serialized JSON\n", _name)
        }
        LOG_SD_F("[%s] unable to open the file\n", _name)
        return false;
    }
} // namespace json_parser