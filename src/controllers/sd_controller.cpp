#include <StreamUtils.h>
#include "sd_controller.hpp"
#include "global_queue.hpp"
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
    sd_controller::sd_controller() : controller("sd", JSON_OBJECT_SIZE(2)),
                                     _execute(false)
    {
    }

    bool sd_controller::initialize()
    {
        return SD.begin(CHIP_SELECT);
    }

    bool sd_controller::can_handle(const JsonObject &json) const
    {
        if (json.containsKey(TIME_KEY))
        {
            return false;
        }
        return true;
    }

    void sd_controller::update()
    {
        if (_execute)
        {
            if (!_file_to_execute.isEmpty() && !_file)
            {
                LOG_SD_F("[%s] opening file to execute: %s\n", _name, _file_to_execute.c_str())
                _file = SD.open(_file_to_execute);
                _file_to_execute.clear();

                if(_file)
                {
                    LOG_SD_F("[%s] opened file successfully\n", _name)
                }
                else
                {
                    LOG_SD_F("[%s] unable to open the file\n", _name)
                }
            }

            if(_file)
            {
                DynamicJsonDocument *json = new DynamicJsonDocument(512);
                ReadBufferingStream bufferedFile(_file, 64);
                bool deserialized = deserializeJson(*json, bufferedFile);
                if (!deserialized || !global_queue::queue.push(&json))
                {
                    LOG_SD_F("[%s] closing file after execution\n", _name)
                    delete json;
                    _file.close();
                    _execute = false;
                }
            }
        }
    }

    bool sd_controller::handle(const JsonObject &json)
    {
        if (json.containsKey(EXECUTE))
        {
            if (json.containsKey(FILE_KEY))
            {
                _file_to_execute = json[FILE_KEY].as<String>();
                _file.close();
                return true;
            }
        }
        else if (!json.containsKey(TIME_KEY))
        {
            _file.close();
            _file_to_execute.clear();

            _file = SD.open(LOG_FILE, "w");
            if (_file)
            {
                json[TIME_KEY] = millis();
                ReadBufferingStream bufferedFile(_file, 64);
                serializeJson(json, _file);
                LOG_SD_F("[%s] serialized JSON\n", _name)
                _file.close();
                return true;
            }
            else
            {
                LOG_SD_F("[%s] unable to open the file\n", _name)
            }
        }
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