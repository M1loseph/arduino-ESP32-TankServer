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

#include "parser/parser.h"

#include "debug.h"
#include "json_messages.h"
#include "commands.h"

#include "functions/webserver.h"
#include "functions/engines.h"

// ================
// VARIABLES
// ================

Parser parser;

// ==============
// SETUP AND LOOP
// ==============

void setup()
{
  LOG_NL("Initing engine pins...");
  init_engines();

  LOG_NL("Adding events...");
  parser.AddEvent(Command::FORWARD_L, forward_left);
  parser.AddEvent(Command::FORWARD_R, forward_right);
  parser.AddEvent(Command::FORWARD, forward);

  parser.AddEvent(Command::BACKWARD_L, backward_left);
  parser.AddEvent(Command::BACKWARD_R, backward_right);
  parser.AddEvent(Command::BACKWARD, backward);

  parser.AddEvent(Command::STOP_L, stop_left);
  parser.AddEvent(Command::STOP_R, stop_right);
  parser.AddEvent(Command::STOP, stop);

  parser.AddEvent(Command::FASTER_L, faster_left);
  parser.AddEvent(Command::FASTER_R, faster_right);
  parser.AddEvent(Command::FASTER, faster);

  parser.AddEvent(Command::SLOWER_L, slower_left);
  parser.AddEvent(Command::SLOWER_R, slower_right);
  parser.AddEvent(Command::SLOWER, slower);

  parser.AddEvent(Command::STEADY_L, steady_left);
  parser.AddEvent(Command::STEADY_R, steady_right);
  parser.AddEvent(Command::STEADY, steady);

  Serial.begin(115200);

  LOG("Is event queue full: ");
  LOG_NL(parser.IsFull());

  LOG_NL("Creating WiFi...");
  init_entire_web();
}

void loop()
{
  dns.processNextRequest();
  web_socket.cleanupClients();
  // JUST FOR DEBUG
  if (parser.ReadStream(Serial))
    parser.ExecuteBuffer();
}

#endif // UNIT_TEST
#endif // defined(NODEMCUS32) || defined(NODEMCUV2)