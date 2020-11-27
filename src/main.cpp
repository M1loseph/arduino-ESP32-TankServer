#ifndef UNIT_TEST

#include <Arduino.h>
#include <memory>
#include "debug.hpp"

#include "webserver.hpp"
#include "controllers/engines_controller.hpp"
#include "controllers/arm_controller.hpp"
#include "controllers/leds_controller.hpp"
#include "controllers/mp3_controller.hpp"
#include "controllers/sd_controller.hpp"
#include "json_parser/parser.hpp"
#include "global_queue.hpp"

json_parser::parser parser;

void setup()
{
#if SMART_TANK_DEBUG
    Serial.begin(115200);
#endif // SMART_TANK_DEBUG

    LOG_NL("[main] adding controllers...")
    bool if_added = true;
    if_added &= parser.add_controller(std::unique_ptr<json_parser::engines_controller>(new json_parser::engines_controller()));
    if_added &= parser.add_controller(std::unique_ptr<json_parser::arm_controller>(new json_parser::arm_controller()));
    if_added &= parser.add_controller(std::unique_ptr<json_parser::leds_controller>(new json_parser::leds_controller()));
    if_added &= parser.add_controller(std::unique_ptr<json_parser::mp3_controller>(new json_parser::mp3_controller()));
    if_added &= parser.add_controller(std::unique_ptr<json_parser::sd_controller>(new json_parser::sd_controller()));
    LOG_F("[main] adding controllers: %s\n", if_added ? "success" : "failed")

    parser.initialize_all();
    LOG_NL("[main] creating WiFi...")
    webserver::init_entire_web();

    LOG_NL("[main] initing global queue...")
    if(!global_queue::queue.initialize())
    {
        LOG_NL("[main] could not create queue")
    }
}

void loop()
{
    webserver::process_web();
    DynamicJsonDocument* json = nullptr;
    if(global_queue::queue.read(json))
    {
        parser.handle(json->as<JsonObjectConst>());
        delete json;
    }
    parser.hadnle_updates();
#ifdef SMART_TANK_DEBUG
    if (Serial.available())
    {
        json = new DynamicJsonDocument(256);
        deserializeJson(*json, Serial);
        global_queue::queue.push(json);
    }
#endif // SMART_TANK_DEBUG
}

#endif // UNIT_TEST