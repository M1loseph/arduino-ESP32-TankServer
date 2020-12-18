#ifndef UNIT_TEST

#include <Arduino.h>
#include <StreamUtils.h>
#include <memory>
#include "debug.hpp"

#include "webserver.hpp"
#include "controllers/engines_controller.hpp"
#include "controllers/arm_controller.hpp"
#include "controllers/leds_controller.hpp"
#include "controllers/mp3_controller.hpp"
#include "controllers/sd_controller.hpp"
#include "controllers/config_controller.hpp"
#include "json_parser/parser.hpp"
#include "global_queue.hpp"

json_parser::parser parser;

void setup()
{
    INIT_LOG

    LOG_NL("[main] initing global queue...")
    if(!global_queue::queue.initialize())
    {
        LOG_NL("[main] could not create queue")
        return;
    }

    LOG_NL("[main] adding controllers...")
    bool if_ok = true;
    if_ok &= parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::engines_controller()));
    if_ok &= parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::arm_controller()));
    if_ok &= parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::leds_controller()));
    if_ok &= parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::mp3_controller()));
    if_ok &= parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::sd_controller()));
    if_ok &= parser.add_controller(std::unique_ptr<json_parser::controller>(new json_parser::config_controller(parser)));
    LOG_F("[main] adding controllers: %s\n", if_ok ? "success" : "failed")

    if_ok = parser.initialize_all();
    LOG_NL("[main] creating WiFi...")
    webserver::init_entire_web();

    DynamicJsonDocument* mp3_json = new DynamicJsonDocument(256);
    (*mp3_json)["controller"] = "mp3";
    if(if_ok)
        (*mp3_json)["command"] = "windows_xp";
    else
        (*mp3_json)["command"] = "error";
    
    if(!global_queue::queue.push(&mp3_json))
        delete mp3_json;

    LOG_F("[main] memory usage before: %d\n", esp_get_free_heap_size())
    auto device_state = parser.retrive_data();
    LOG_F("[main] memory usage after: %d\n", esp_get_free_heap_size())
    LOG_JSON_PRETTY(device_state);
    LOG_NL("[main] end of setup");
}

void loop()
{
    webserver::process_web();
    DynamicJsonDocument* json = nullptr;
    if(global_queue::queue.read(&json))
    {
        parser.handle(json->as<JsonObject>());
        delete json;
    }
    parser.handle_updates();

#ifdef SMART_TANK_DEBUG
    if (Serial.available())
    {
        json = new DynamicJsonDocument(256);
        deserializeJson(*json, Serial);
        LOG_JSON_PRETTY(*json);
        global_queue::queue.push(&json);
    }
#endif // SMART_TANK_DEBUG
}

#endif // UNIT_TEST