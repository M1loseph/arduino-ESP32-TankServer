#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <memory>
#include <utility>
#include "controllers/abstract/controller.hpp"

namespace json_parser
{
    class parser
    {
    public:
        std::pair<uint8_t, uint8_t> handle(const JsonObjectConst& json);
        void handle_updates();
        bool add_controller(std::unique_ptr<controller>&& controller);
        bool initialize_all();
        DynamicJsonDocument retrive_data();

    private:
        std::vector<std::unique_ptr<controller>> _controllers;
    };
} // namespace parser
#endif // __PARSER_HPP__