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

constexpr int RIGHT_FRONT = D0;
constexpr int RIGHT_BACK = D1;
constexpr int LEFT_FRONT = D3;
constexpr int LEFT_BACK = D2;

constexpr int RIGHT_SPEED = D4;
constexpr int LEFT_SPEED = D5;

constexpr uint DEF_SPEED = 500;

constexpr int TURRET_FRONT = D6;
constexpr int TURRET_BACK = D7;
constexpr int TURRET_SPEED = 10;

constexpr uint TURRET_DEF_SPEED = 400;

// ================
// VARIABLES
// ================

Engine left(LEFT_FRONT, LEFT_BACK, LEFT_SPEED, DEF_SPEED);
Engine right(RIGHT_FRONT, RIGHT_BACK, RIGHT_SPEED, DEF_SPEED);
Engine turret(TURRET_FRONT, TURRET_BACK, TURRET_SPEED, TURRET_DEF_SPEED);
Parser parser;

WiFiClient espClient;
PubSubClient client(espClient);

// ================
// SETTING UP PINS
// ================

static void InitEnginePins()
{
  pinMode(RIGHT_FRONT, OUTPUT);
  pinMode(RIGHT_BACK, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT);
  pinMode(LEFT_FRONT, OUTPUT);
  pinMode(LEFT_BACK, OUTPUT);
  pinMode(LEFT_SPEED, OUTPUT);
  pinMode(TURRET_FRONT, OUTPUT);
  pinMode(TURRET_BACK, OUTPUT);
  pinMode(TURRET_SPEED, OUTPUT);
}

// ==============
// ENGINES
// ==============

void SpeedFun(const CommandBuffer &buffer)
{
  Number number = buffer.FindNumber(1);

  if (number.success)
  {
    if (number.number >= 0 && number.number <= 1023)
    {
      left.ChangeSpeed(number.number);
      right.ChangeSpeed(number.number);
      LOG_NL(number.number);
    }
    else
    {
      LOG_NL("Speed is too large or too small");
    }
  }
  else
  {
    LOG_NL("Speed not found");
  }
}

// ==============
// TYPICAL ARDUINO
// ==============

auto ForwardFun = [](const CommandBuffer &b) { left.Forward(); right.Forward(); };
auto BackwardFun = [](const CommandBuffer &b) {left.Backward(); right.Backward(); };
auto LeftFun = [](const CommandBuffer &b) { Engine::TurnLeft(left, right); };
auto RightFun = [](const CommandBuffer &b) { Engine::TurnRight(left, right); };
auto StopFun = [](const CommandBuffer &b) {left.Stop(); right.Stop(); };
auto TurretForwardFun = [](const CommandBuffer &b) { turret.Forward(); };
auto TurretBackwardFun = [](const CommandBuffer &b) { turret.Backward(); };
auto TurretStopFun = [](const CommandBuffer &b) { turret.Stop(); };

void callback(const char *topic, byte *payload, unsigned int length)
{
#if DEBUG
  client.publish(Network::SUBSCRIBE_TOPIC, (char *)payload, length);
#endif
  for (size_t i = 0; i < length; i++)
    parser.GetBuff().PushBack(payload[i]);

  parser.ExecuteMessege();
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect(Network::ID))
      client.subscribe(Network::SUBSCRIBE_TOPIC);
    delay(5000);
  }
  LOG_NL("Connected to broker");
}

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
  parser.AddEvents(Command::FORWARD, ForwardFun);
  parser.AddEvents(Command::BACKWARD, BackwardFun);
  parser.AddEvents(Command::LEFT, LeftFun);
  parser.AddEvents(Command::RIGHT, RightFun);
  parser.AddEvents(Command::STOP, StopFun);
  parser.AddEvents(Command::SPEED, SpeedFun);
  parser.AddEvents(Command::TURRET_FORWARD, TurretForwardFun);
  parser.AddEvents(Command::TURRET_BACKWARD, TurretBackwardFun);
  parser.AddEvents(Command::TURRET_STOP, TurretStopFun);
}

void loop()
{
  client.loop();
  if (!client.connected())
  {
    left.Stop();
    right.Stop();
    turret.Stop();
    reconnect();
  }
}

#endif // UNIT_TEST
#endif // NODEMCU