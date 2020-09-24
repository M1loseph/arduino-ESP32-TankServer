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

// #include <Wire.h>
// #include <OneWire.h>
// #include <DallasTemperature.h>
// #include <I2Cdev.h>
// #include <MPU6050.h>
// #include <SPI.h>
// #include <SD.h>

#include "debug.h"

#include "functions/webserver.h"
#include "functions/engines.h"
#include "functions/global_parser.h"

// ==============
// SETUP AND LOOP
// ==============

void setup()
{
  LOG_NL("Initing engine pins...");
  driving::init_engines();

  LOG_NL("Initing global semaphore...");
  init_global_semaphore();
  LOG_NL("Adding events...");
  global_parser.add_event(driving::commands::FORWARD, driving::forward);
  global_parser.add_event(driving::commands::BACKWARD, driving::backward);
  global_parser.add_event(driving::commands::STOP, driving::stop);
  global_parser.add_event(driving::commands::FASTER, driving::faster);
  global_parser.add_event(driving::commands::SLOWER, driving::slower);
  global_parser.add_event(driving::commands::KEEP_SPEED, driving::keep_speed);
  global_parser.add_event(driving::commands::SET_SPEED, driving::set_speed);

  Serial.begin(115200);

  LOG("Is event queue full: ");
  LOG_NL(global_parser.is_full());

  LOG_NL("Creating WiFi...");
  init_entire_web();
}

void loop()
{
  dns.processNextRequest();
  web_socket.cleanupClients();
  // JUST FOR DEBUG
  if (xSemaphoreTake(global_parser_semaphore, (TickType_t)10) == pdTRUE)
  {
    if (global_parser.read_stream(Serial))
      global_parser.exec_buffer();

    xSemaphoreGive(global_parser_semaphore);
  }
}

#endif // UNIT_TEST
#endif // defined(NODEMCUS32) || defined(NODEMCUV2)