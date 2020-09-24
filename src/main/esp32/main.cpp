#if defined(ESP32) || defined(ESP8266)
#ifndef UNIT_TEST

// https://www.youtube.com/watch?v=7h2bE2vNoaY
// I've been using ESP32 Devkit V1 30 pin version
// The following hasn't been implemented yet
// ESP83266 are NodeMcu V2 and V3

#ifndef ESP32
#error "Not implemented yet"
#endif

#include <Arduino.h>
#include <Wire.h>
// #include <OneWire.h>
// #include <DallasTemperature.h>
// #include <I2Cdev.h>
// #include <MPU6050.h>
// #include <SPI.h>
// #include <SD.h>

#include "debug.h"

#include "functions/global_parser.h"
#include "functions/webserver.h"
#include "functions/engines.h"
#include "functions/arm.h"

// ==============
// SETUP AND LOOP
// ==============

void setup()
{
  LOG_NL("Initing engine pins...");
  driving::init_engines();

  LOG_NL("Initing global semaphore...");
  global_parser::init_global_semaphore();

  LOG_NL("Initing arm...");
  arm::init_arm();

  LOG_NL("Adding events...");
  // driving events
  global_parser::parser.add_event(driving::commands::FORWARD, driving::forward);
  global_parser::parser.add_event(driving::commands::BACKWARD, driving::backward);
  global_parser::parser.add_event(driving::commands::STOP, driving::stop);
  global_parser::parser.add_event(driving::commands::FASTER, driving::faster);
  global_parser::parser.add_event(driving::commands::SLOWER, driving::slower);
  global_parser::parser.add_event(driving::commands::KEEP_SPEED, driving::keep_speed);
  global_parser::parser.add_event(driving::commands::SET_SPEED, driving::set_speed);

  // arm events
  global_parser::parser.add_event(arm::commands::BASE, arm::move_base);
  global_parser::parser.add_event(arm::commands::SHOULDER, arm::move_shoulder);
  global_parser::parser.add_event(arm::commands::ELBOW, arm::move_elbow);
  global_parser::parser.add_event(arm::commands::WRIST, arm::move_wrist);
  global_parser::parser.add_event(arm::commands::ROTATION, arm::move_rotation);
  global_parser::parser.add_event(arm::commands::CLAW, arm::move_claw);

#if SMART_TANK_DEBUG
  Serial.begin(115200);
#endif // DEBUG

  LOG("Is event queue full: ");
  LOG_NL(global_parser::parser.is_full());

  LOG_NL("Creating WiFi...");
  webserver::init_entire_web();
}

void loop()
{
  webserver::dns.processNextRequest();
  webserver::web_socket.cleanupClients();
#ifdef SMART_TANK_DEBUG
  // JUST FOR DEBUG
  if (xSemaphoreTake(global_parser::semaphore, (TickType_t)10) == pdTRUE)
  {
    if (global_parser::parser.read_stream(Serial))
      global_parser::parser.exec_buffer();

    xSemaphoreGive(global_parser::semaphore);
  }
#endif // SMART_TANK_DEBUG
}

#endif // UNIT_TEST
#endif // defined(NODEMCUS32) || defined(NODEMCUV2)