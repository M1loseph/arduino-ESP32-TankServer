#ifdef NODEMCU
#ifndef UNIT_TEST

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#include "parser/parser.h"
#include "engine/engine.h"
#include "debug.h"
#include "network.h"
#include "commands.h"

// ================
// ENGINE PINS
// ================

// https://www.youtube.com/watch?v=7h2bE2vNoaY

constexpr int PIN_RIGHT_FRONT = D0;
constexpr int PIN_RIGHT_BACK = D1;
constexpr int PIN_LEFT_FRONT = D3;
constexpr int PIN_LEFT_BACK = D2;

constexpr int PIN_TANK_SPEED = D4;

constexpr uint DEF_SPEED = 500;

// ================
// VARIABLES
// ================

Engine left(PIN_LEFT_FRONT, PIN_LEFT_BACK, PIN_TANK_SPEED, DEF_SPEED);
Engine right(PIN_RIGHT_FRONT, PIN_RIGHT_BACK, PIN_TANK_SPEED, DEF_SPEED);
Parser parser;

WiFiClient espClient;
PubSubClient client(espClient);

// ================
// SETTING UP PINS
// ================

static void InitEnginePins()
{
  pinMode(PIN_RIGHT_FRONT, OUTPUT);
  pinMode(PIN_RIGHT_BACK, OUTPUT);
  pinMode(PIN_TANK_SPEED, OUTPUT);
  pinMode(PIN_LEFT_FRONT, OUTPUT);
  pinMode(PIN_LEFT_BACK, OUTPUT);
}

// ==============
// FUNCTIONS TO EVENTS
// ==============

void SpeedFun(const CommandBuffer &buffer)
{
  Number number = buffer.FindNumber(1);

  if (number.success)
  {
    if (number.value >= 0 && number.value <= 1023)
    {
      left.ChangeSpeed(number.value);
      right.ChangeSpeed(number.value);
      LOG_NL(number.value);
    }
    else
      LOG_NL("Speed is too large or too small");
  }
  else
    LOG_NL("Speed not found");
}

void Move(const CommandBuffer &b)
{
  const char *commandToSend = b.Word(1);
  if (commandToSend)
  {
    // index of the command
    size_t index = commandToSend - b.C_Ptr();

    // send everything from this index till the very end
    for (size_t i = index; i < b.Length(); i++)
      Serial.write(b.C_Ptr()[i]);
  }
}

auto ForwardFun = [](const CommandBuffer &b) { left.Forward(); right.Forward(); };
auto BackwardFun = [](const CommandBuffer &b) {left.Backward(); right.Backward(); };
auto StopFun = [](const CommandBuffer &b) {left.Stop(); right.Stop(); };

auto LeftFun = [](const CommandBuffer &b) { Engine::TurnLeft(left, right); };
auto RightFun = [](const CommandBuffer &b) { Engine::TurnRight(left, right); };

// ==============
// WIFI
// ==============

void callback(const char *topic, byte *payload, unsigned int length)
{
  for (size_t i = 0; i < length; i++)
    parser.GetBuff().PushBack(payload[i]);

  parser.ExecuteMessege();

#if DEBUG
  client.publish(Network::CALLBACK_TOPIC, "Messege received!");
#endif
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect(Network::ID))
      client.subscribe(Network::SUBSCRIBE_TOPIC);
    delay(5000);
  }
  client.publish(Network::CALLBACK_TOPIC, "Tank has been connected!");

  LOG_NL("Connected to broker");
}

// ==============
// SETUP AND LOOP
// ==============

void setup()
{
  InitEnginePins();
  client.setServer(Network::MQTT_SERVER, Network::PORT);
  client.setCallback(callback);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(Network::SSID, Network::PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  LOG_NL("Connected to wifi");
  // Tank
  parser.AddEvents(Command::Mcu::TANK_FORWARD, ForwardFun);
  parser.AddEvents(Command::Mcu::TANK_BACKWARD, BackwardFun);
  parser.AddEvents(Command::Mcu::TANK_LEFT, LeftFun);
  parser.AddEvents(Command::Mcu::TANK_RIGHT, RightFun);
  parser.AddEvents(Command::Mcu::TANK_STOP, StopFun);
  parser.AddEvents(Command::Mcu::TANK_SPEED, SpeedFun);

  parser.AddEvents(Command::Mcu::MOVE, Move);
}

void loop()
{
  client.loop();
  if (!client.connected())
  {
    left.Stop();
    right.Stop();
    reconnect();
  }
}

#endif // UNIT_TEST
#endif // NODEMCU