#include <StreamUtils.h>
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
    sd_controller::sd_controller() : controller("sd", JSON_OBJECT_SIZE(2))
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
        if(_file)
            _file.close();

        _file = SD.open(LOG_FILE);
        if(_file)
        {
            ReadBufferingStream bufferedFile(_file, 64);
            serializeJson(json, _file);
            LOG_SD_F("[%s] serialized JSON\n", _name)
            _file.close();
            return true;
        }
        LOG_SD_F("[%s] unable to open the file\n", _name)
        return false;
    }

    DynamicJsonDocument sd_controller::retrive_data()
    {
        DynamicJsonDocument json(_json_size);
        json[NAME_FIELD] = _name;
        json[DATA_FIELD] = nullptr;
        return json;
    }
} // namespace json_parser