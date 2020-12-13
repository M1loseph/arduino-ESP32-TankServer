#include <StreamUtils.h>
#include "sd_controller.hpp"
#include "global_queue.hpp"
#include "debug.hpp"

#if SD_DEBUG

#define LOG_SD(message) LOG(message)
#define LOG_SD_NL(message) LOG_NL(message)
#define LOG_SD_F(...) LOG_F(__VA_ARGS__)
#define LOG_SD_JSON_PRETTY(json) LOG_JSON_PRETTY(json)

#else

#define LOG_SD(message)
#define LOG_SD_NL(message)
#define LOG_SD_F(...)
#define LOG_SD_JSON_PRETTY(json)

#endif // SD_DEBUG

namespace json_parser
{
    sd_controller::sd_controller() : controller("sd", JSON_OBJECT_SIZE(2)),
                                     _execute(false)
    {
    }

    bool sd_controller::initialize()
    {
        bool succ = SD.begin(CHIP_SELECT);
        if(succ)
        {
            if(!SD.exists(LOG_FILE))
            {
                LOG_SD_F("[%s] creating logs file...", _name)
                auto temp_file = SD.open(LOG_FILE);
                if(temp_file)
                    temp_file.close();
            }
        }
        return succ;
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
                LOG_SD_F("[%s] peek before read char: %c int: %d\n", _name, _file.peek(), _file.peek())
                auto error = deserializeJson(*json, _file);

                if(!error)
                {
                    LOG_SD_JSON_PRETTY(*json)
                }

                if (error || !global_queue::queue.push(&json))
                {
                    LOG_SD_F("[%s] closing file after execution\n", _name)
                    LOG_SD_F("[%s] error: %s (might be queue fault)", _name, error.c_str())
                    delete json;
                    _file.close();
                    _execute = false;
                }
            }
            else
            {
                _execute = false;
            }
        }
    }

    bool sd_controller::handle(const JsonObject &json)
    {
        if (json.containsKey(COMMAND_KEY) && !strcmp(json[COMMAND_KEY].as<const char *>(), EXECUTE))
        {
            LOG_SD_F("[%s] recived execute command\n", _name)
            if (json.containsKey(FILE_KEY))
            {
                _file_to_execute = json[FILE_KEY].as<String>();
                _file.close();
                _execute = true;
                return true;
            }
        }
        else if (!json.containsKey(TIME_KEY))
        {
            _file.close();

            _file = SD.open(LOG_FILE, "a");
            if (_file)
            {
                json[TIME_KEY] = millis();
                WriteBufferingStream bufferedFile(_file, 64);
                serializeJson(json, _file);
                bufferedFile.write('\n');
                bufferedFile.flush();
                _file.close();
                LOG_SD_F("[%s] serialized JSON\n", _name)
                return true;
            }
            else
            {
                LOG_SD_F("[%s] unable to open the file\n", _name)
            }
        }
        else
        {
            LOG_SD_F("[%s] unable to handle\n", _name)
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