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
#include "functions/leds.h"

// ==============
// SETUP AND LOOP
// ==============

#ifdef SMART_TANK_DEBUG
CommandBuffer serial_buffer;
#endif // SMART_TANK_DEBUG

void setup()
{
#if SMART_TANK_DEBUG
  Serial.begin(115200);
#endif // DEBUG

  LOG_NL("Initing engine pins...");
  driving::init_engines();

  LOG_NL("Initing global semaphore...");
  global_parser::init_global_semaphore();

  LOG_NL("Initing arm...");
  arm::init_arm();

  LOG_NL("Initing leds...");
  leds::init_leds();

  LOG_NL("Adding events...");
  // driving events
  global_parser::parser.add_event(driving::commands::FORWARD, driving::forward);
  global_parser::parser.add_event(driving::commands::FORWARD_LEFT, driving::forward_left);
  global_parser::parser.add_event(driving::commands::FORWARD_RIGHT, driving::forward_right);

  global_parser::parser.add_event(driving::commands::BACKWARD, driving::backward);
  global_parser::parser.add_event(driving::commands::BACKWARD_LEFT, driving::backward_left);
  global_parser::parser.add_event(driving::commands::BACKWARD_RIGHT, driving::backward_right);

  global_parser::parser.add_event(driving::commands::STOP, driving::stop);
  global_parser::parser.add_event(driving::commands::STOP_LEFT, driving::stop_left);
  global_parser::parser.add_event(driving::commands::STOP_RIGHT, driving::stop_right);

  global_parser::parser.add_event(driving::commands::FASTER, driving::faster);
  global_parser::parser.add_event(driving::commands::FASTER_LEFT, driving::faster_left);
  global_parser::parser.add_event(driving::commands::FASTER_RIGHT, driving::faster_right);

  global_parser::parser.add_event(driving::commands::SLOWER, driving::slower);
  global_parser::parser.add_event(driving::commands::SLOWER_LEFT, driving::slower_left);
  global_parser::parser.add_event(driving::commands::SLOWER_RIGHT, driving::slower_right);

  global_parser::parser.add_event(driving::commands::KEEP_SPEED, driving::keep_speed);
  global_parser::parser.add_event(driving::commands::KEEP_SPEED_LEFT, driving::keep_speed_left);
  global_parser::parser.add_event(driving::commands::KEEP_SPEED_RIGHT, driving::keep_speed_right);

  global_parser::parser.add_event(driving::commands::SPEED, driving::set_speed);
  global_parser::parser.add_event(driving::commands::SPEED_LEFT, driving::set_speed_left);
  global_parser::parser.add_event(driving::commands::SPEED_RIGHT, driving::set_speed_right);

  // led events
  global_parser::parser.add_event(leds::commands::LED_ANIMATION_BACKWARD, leds::animate_backward);
  global_parser::parser.add_event(leds::commands::LED_ANIMATION_FORWARD, leds::animate_forward);
  global_parser::parser.add_event(leds::commands::LED_CREATE_CUSTOM, leds::create_custom_animation);
  global_parser::parser.add_event(leds::commands::LED_CUSTOM, leds::custom_animation);
  global_parser::parser.add_event(leds::commands::LED_EUROBEAT, leds::eurobeat_animation);
  global_parser::parser.add_event(leds::commands::LED_POLISH, leds::polish_animation);
  global_parser::parser.add_event(leds::commands::LED_RANDOM, leds::random_animation);
  global_parser::parser.add_event(leds::commands::LED_SET_ANIMATION_SPEED, leds::set_animation_speed);
  global_parser::parser.add_event(leds::commands::LED_SET_BRIGHTNESS, leds::set_brightness);
  global_parser::parser.add_event(leds::commands::LED_STOP, leds::stop_animation);

  // arm events
  // global_parser::parser.add_event(arm::commands::BASE, arm::move_base);
  // global_parser::parser.add_event(arm::commands::SHOULDER, arm::move_shoulder);
  // global_parser::parser.add_event(arm::commands::ELBOW, arm::move_elbow);
  // global_parser::parser.add_event(arm::commands::WRIST, arm::move_wrist);
  // global_parser::parser.add_event(arm::commands::ROTATION, arm::move_rotation);
  // global_parser::parser.add_event(arm::commands::CLAW, arm::move_claw);

  LOG("Is event queue full: ");
  LOG_NL(global_parser::parser.is_full());

  LOG_NL("Creating WiFi...");
  webserver::init_entire_web();
}

void loop()
{
  webserver::dns.processNextRequest();
  webserver::web_socket.cleanupClients();
  leds::update_led_animation();
#ifdef SMART_TANK_DEBUG
  // JUST FOR DEBUG
  if (serial_buffer.read_stream(Serial))
  {
    // wait for mutex
    while (xSemaphoreTake(global_parser::semaphore, (TickType_t)10) != pdTRUE)
      ;

    global_parser::parser.exec_buffer(serial_buffer);
    xSemaphoreGive(global_parser::semaphore);
    serial_buffer.clear();
  }
#endif // SMART_TANK_DEBUG
}

#endif // UNIT_TEST
#endif // defined(NODEMCUS32) || defined(NODEMCUV2)