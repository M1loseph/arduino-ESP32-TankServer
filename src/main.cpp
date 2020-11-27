#ifndef UNIT_TEST

#include <Arduino.h>
#include "debug.hpp"

#include "webserver.hpp"
#include "controllers/engines_controller.hpp"
#include "controllers/arm_controller.hpp"
#include "controllers/leds_controller.hpp"
#include "controllers/mp3_controller.hpp"
#include "controllers/sd_controller.hpp"

json_parser::engines_controller eng_con;
json_parser::arm_controller arm_con;
json_parser::leds_controller led_con;
json_parser::mp3_controller mp3_con;
json_parser::sd_controller sd_con;

void setup()
{
#if SMART_TANK_DEBUG
    Serial.begin(115200);
#endif // SMART_TANK_DEBUG

    LOG_NL("[main] initializing engine pins...");
    bool init_res = eng_con.initialize();
    LOG_F("[main] initing engines: %s\n", init_res ? "successful" : "error");

    LOG_NL("[main] initing arm...");
    init_res = arm_con.initialize();
    LOG_F("[main] initing arm: %s\n", init_res ? "successful" : "error");

    LOG_NL("[main] initing leds...");
    init_res = led_con.initialize();
    LOG_F("[main] initing lesd: %s\n", init_res ? "successful" : "error");

    LOG_NL("[main] initing mp3...");
    init_res = mp3_con.initialize();
    LOG_F("[main] initing mp3: %s\n", init_res ? "successful" : "error");

    LOG_NL("[main] initing SD card...");
    init_res = sd_con.initialize();
    LOG_F("[main] initing mp3: %s\n", init_res ? "successful" : "error");

    LOG_NL("[main] creating WiFi...");
    webserver::init_entire_web();
}

void loop()
{
    webserver::process_web();

    if (xSemaphoreTake(global_parser::semaphore, (TickType_t)10U) == pdTRUE)
    {
        global_parser::parser.exec_intervals();
        xSemaphoreGive(global_parser::semaphore);
    }

#ifdef SMART_TANK_DEBUG
    // JUST FOR DEBUG
    if (serial_buffer.read_stream(Serial))
    {
        // wait for mutex
        while (xSemaphoreTake(global_parser::semaphore, (TickType_t)10U) != pdTRUE)
            ;

        global_parser::parser.exec_buffer(&serial_buffer);
        xSemaphoreGive(global_parser::semaphore);
        serial_buffer.clear();
    }
#endif // SMART_TANK_DEBUG
}

#endif // UNIT_TEST