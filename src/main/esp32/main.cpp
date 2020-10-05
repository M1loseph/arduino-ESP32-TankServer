#if defined(ESP32) || defined(ESP8266)
#ifndef UNIT_TEST

// https://www.youtube.com/watch?v=7h2bE2vNoaY
// I've been using ESP32 Devkit V1 30 pin version
// ESP8266 hasn't been implemented yet

#ifndef ESP32
#error "Not implemented yet"
#endif

#include <Arduino.h>
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
#include "functions/mp3.h"

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
#endif // SMART_TANK_DEBUG

  LOG_NL("Initing engine pins...");
  driving::init_engines();

  LOG_NL("Initing global semaphore...");
  global_parser::init_global_semaphore();

  LOG_NL("Initing arm...");
  arm::init_arm();

  LOG_NL("Initing leds...");
  leds::init_leds();

  LOG_NL("Initing mp3...");
  mp3::init_mp3();

  LOG_NL("Adding events...");
  bool if_added = true;
  // driving events
  if_added &= global_parser::parser.add_event(driving::commands::FORWARD, driving::forward);
  if_added &= global_parser::parser.add_event(driving::commands::FORWARD_LEFT, driving::forward_left);
  if_added &= global_parser::parser.add_event(driving::commands::FORWARD_RIGHT, driving::forward_right);

  if_added &= global_parser::parser.add_event(driving::commands::BACKWARD, driving::backward);
  if_added &= global_parser::parser.add_event(driving::commands::BACKWARD_LEFT, driving::backward_left);
  if_added &= global_parser::parser.add_event(driving::commands::BACKWARD_RIGHT, driving::backward_right);

  if_added &= global_parser::parser.add_event(driving::commands::STOP, driving::stop);
  if_added &= global_parser::parser.add_event(driving::commands::STOP_LEFT, driving::stop_left);
  if_added &= global_parser::parser.add_event(driving::commands::STOP_RIGHT, driving::stop_right);

  if_added &= global_parser::parser.add_event(driving::commands::FASTER, driving::faster);
  if_added &= global_parser::parser.add_event(driving::commands::FASTER_LEFT, driving::faster_left);
  if_added &= global_parser::parser.add_event(driving::commands::FASTER_RIGHT, driving::faster_right);

  if_added &= global_parser::parser.add_event(driving::commands::SLOWER, driving::slower);
  if_added &= global_parser::parser.add_event(driving::commands::SLOWER_LEFT, driving::slower_left);
  if_added &= global_parser::parser.add_event(driving::commands::SLOWER_RIGHT, driving::slower_right);

  if_added &= global_parser::parser.add_event(driving::commands::KEEP_SPEED, driving::keep_speed);
  if_added &= global_parser::parser.add_event(driving::commands::KEEP_SPEED_LEFT, driving::keep_speed_left);
  if_added &= global_parser::parser.add_event(driving::commands::KEEP_SPEED_RIGHT, driving::keep_speed_right);

  if_added &= global_parser::parser.add_event(driving::commands::SPEED, driving::set_speed);
  if_added &= global_parser::parser.add_event(driving::commands::SPEED_LEFT, driving::set_speed_left);
  if_added &= global_parser::parser.add_event(driving::commands::SPEED_RIGHT, driving::set_speed_right);

  LOG_F("Adding driving events: %s\n", if_added ? "successful" : "error");

  // led events
  if_added &= global_parser::parser.add_event(leds::commands::LED_ANIMATION_BACKWARD, leds::animate_backward);
  if_added &= global_parser::parser.add_event(leds::commands::LED_ANIMATION_FORWARD, leds::animate_forward);
  if_added &= global_parser::parser.add_event(leds::commands::LED_CREATE_CUSTOM, leds::create_custom_animation);
  if_added &= global_parser::parser.add_event(leds::commands::LED_CUSTOM, leds::custom_animation);
  if_added &= global_parser::parser.add_event(leds::commands::LED_EUROBEAT, leds::eurobeat_animation);
  if_added &= global_parser::parser.add_event(leds::commands::LED_POLISH, leds::polish_animation);
  if_added &= global_parser::parser.add_event(leds::commands::LED_RANDOM, leds::random_animation);
  if_added &= global_parser::parser.add_event(leds::commands::LED_CLEAR, leds::clear_animation);
  if_added &= global_parser::parser.add_event(leds::commands::LED_SET_ANIMATION_SPEED, leds::set_animation_speed);
  if_added &= global_parser::parser.add_event(leds::commands::LED_SET_BRIGHTNESS, leds::set_brightness);
  if_added &= global_parser::parser.add_event(leds::commands::LED_STOP, leds::stop_animation);

  LOG_F("Adding LED events: %s\n", if_added ? "successful" : "error");

  // arm events
  // global_parser::parser.add_event(arm::commands::BASE, arm::move_base);
  // global_parser::parser.add_event(arm::commands::SHOULDER, arm::move_shoulder);
  // global_parser::parser.add_event(arm::commands::ELBOW, arm::move_elbow);
  // global_parser::parser.add_event(arm::commands::WRIST, arm::move_wrist);
  // global_parser::parser.add_event(arm::commands::ROTATION, arm::move_rotation);
  // global_parser::parser.add_event(arm::commands::CLAW, arm::move_claw);

  // mp3 events
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_PROPAGANDA, mp3::propaganda);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_MIGHTY_POLISH_TANK, mp3::mighty_polish_tank);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_HIGH_GROUND, mp3::high_ground);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_FINE_ADDITION, mp3::fine_addition);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_I_DONT_LIKE_SAND, mp3::i_dont_like_sand);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_HELLO_THERE, mp3::hello_there);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_IM_THE_SENATE, mp3::im_the_senate);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_FOREVER_YOUNG, mp3::forever_young);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_REVENGE, mp3::revenge);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_NARUTO, mp3::naruto);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_THE_BAD_TOUCH, mp3::the_bad_touch);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_HERO, mp3::hero);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_GAS_GAS_GAS, mp3::gas_gas_gas);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_RUNNING_IN_THE_90S, mp3::running_in_the_90s);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_DEJA_VU, mp3::deja_vu);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_RUNNING_IN_THE_90S_SHORT, mp3::running_in_the_90s_short);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_DEJA_VU_SHORT, mp3::deja_vu_short);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_TRUE_SURVIVOR, mp3::true_survivor);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_WINDOWS_XP, mp3::windows_xp);
  if_added &= global_parser::parser.add_event(mp3::commands::MP3_STOP, mp3::stop_playing);
  if_added &= global_parser::parser.add_event(mp3::commands::SET_VOLUME, mp3::set_volume);
  if_added &= global_parser::parser.add_event(mp3::commands::RESUME, mp3::resume);

  LOG_F("Adding MP3 events: %s\n", if_added ? "successful" : "error");

  LOG_F("Is event queue full: %s\n", global_parser::parser.is_full() ? "true" : "false");

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