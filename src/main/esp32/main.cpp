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
#include "json_messages.h"
#include "commands.h"

#include "functions/webserver.h"
#include "functions/engines.h"
#include "functions/global_parser.h"

// ==============
// SETUP AND LOOP
// ==============

void setup()
{
  LOG_NL("Initing engine pins...");
  init_engines();

  LOG_NL("Initing global semaphore...");
  init_global_semaphore();
  LOG_NL("Adding events...");
  global_parser.add_event(commands::driving::FORWARD_L, forward_left);
  global_parser.add_event(commands::driving::FORWARD_R, forward_right);
  global_parser.add_event(commands::driving::FORWARD, forward);

  global_parser.add_event(commands::driving::BACKWARD_L, backward_left);
  global_parser.add_event(commands::driving::BACKWARD_R, backward_right);
  global_parser.add_event(commands::driving::BACKWARD, backward);

  global_parser.add_event(commands::driving::STOP_L, stop_left);
  global_parser.add_event(commands::driving::STOP_R, stop_right);
  global_parser.add_event(commands::driving::STOP, stop);

  global_parser.add_event(commands::driving::FASTER_L, faster_left);
  global_parser.add_event(commands::driving::FASTER_R, faster_right);
  global_parser.add_event(commands::driving::FASTER, faster);

  global_parser.add_event(commands::driving::SLOWER_L, slower_left);
  global_parser.add_event(commands::driving::SLOWER_R, slower_right);
  global_parser.add_event(commands::driving::SLOWER, slower);

  global_parser.add_event(commands::driving::KEEP_SPEED_L, keep_speed_left);
  global_parser.add_event(commands::driving::KEEP_SPEED_R, keep_speed_right);
  global_parser.add_event(commands::driving::KEEP_SPEED, keep_speed);

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