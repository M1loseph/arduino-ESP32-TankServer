#ifndef __CONFIG_CONTROLLER__
#define __CONFIG_CONTROLLER__

#include <ArduinoJson.h>
#include "abstract/templated_controller.hpp"
#include "json_parser/parser.hpp"

namespace json_parser
{
    class config_controller final : public templated_controller<config_controller> {
    public:

        explicit config_controller(const parser& parser);
        bool initialize() override;
        void update() override {}
        DynamicJsonDocument retrive_data() override;

    private:
        static constexpr const char* GET_DATA = "get";

        bool get_data(const JsonObject *json);

        const parser& _parser;
    };
}

#endif //__CONFIG_CONTROLLER__