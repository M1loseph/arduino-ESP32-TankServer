#include <StreamUtils.h>
#include "debug.hpp"
#include "config_controller.hpp"
#include "webserver.hpp"

#if CONFIG_DEBUG

#define LOG_CONFIG(message) LOG(message)
#define LOG_CONFIG_NL(message) LOG_NL(message)
#define LOG_CONFIG_F(...) LOG_F(__VA_ARGS__)
#define LOG_CONFIG_JSON_PRETTY(json) LOG_JSON_PRETTY(json)

#else

#define LOG_CONFIG(message)
#define LOG_CONFIG_NL(message)
#define LOG_CONFIG_F(...)
#define LOG_CONFIG_JSON_PRETTY(json)

#endif // CONFIG_DEBUG

namespace json_parser
{
    config_controller::config_controller(const parser &parser) : templated_controller("config"),
                                                                 _parser(parser)
    {
    }

    bool config_controller::initialize()
    {
        return add_event(GET_DATA, &config_controller::get_data);
    }

    bool config_controller::get_data(const JsonObjectConst *json)
    {
        auto retrived_json = _parser.retrive_data();
        LOG_CONFIG_JSON_PRETTY(retrived_json)
        webserver::send_ws(retrived_json);
        return true;
    }

    DynamicJsonDocument config_controller::retrive_data()
    {
        DynamicJsonDocument json(100);
        json[NAME_FIELD] = _name;
        json[DATA_FIELD] = nullptr;
        return json;
    }
} // namespace json_parser
