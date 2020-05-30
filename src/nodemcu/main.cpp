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

constexpr int PIN_TURRET_FRONT = D6;
constexpr int PIN_TURRET_BACK = D7;
constexpr int PIN_TURRET_SPEED = 10;

constexpr int PIN_GUN_VERTICAL = D5;

constexpr uint TURRET_DEF_SPEED = 300;

// ================
// VARIABLES
// ================

Engine left(PIN_LEFT_FRONT, PIN_LEFT_BACK, PIN_TANK_SPEED, DEF_SPEED);
Engine right(PIN_RIGHT_FRONT, PIN_RIGHT_BACK, PIN_TANK_SPEED, DEF_SPEED);
Engine turret(PIN_TURRET_FRONT, PIN_TURRET_BACK, PIN_TURRET_SPEED, TURRET_DEF_SPEED);
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
  pinMode(PIN_TURRET_FRONT, OUTPUT);
  pinMode(PIN_TURRET_BACK, OUTPUT);
  pinMode(PIN_TURRET_SPEED, OUTPUT);
  pinMode(PIN_GUN_VERTICAL, OUTPUT);
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
auto StopFun = [](const CommandBuffer &b) {left.Stop(); right.Stop(); };

auto LeftFun = [](const CommandBuffer &b) { Engine::TurnLeft(left, right); };
auto RightFun = [](const CommandBuffer &b) { Engine::TurnRight(left, right); };

auto TurretStopFun = [](const CommandBuffer &b) { turret.Stop(); };
auto TurretRightFun = [](const CommandBuffer &b) { turret.Forward(); };
auto TurretLeftFun = [](const CommandBuffer &b) { turret.Backward(); };

auto GunMoveFun = [](const CommandBuffer &b) { digitalWrite(PIN_GUN_VERTICAL, LOW); };
auto GunStopFun = [](const CommandBuffer &b) { digitalWrite(PIN_GUN_VERTICAL, HIGH); };


void callback(const char *topic, byte *payload, unsigned int length)
{
  for (size_t i = 0; i < length; i++)
    parser.GetBuff().PushBack(payload[i]);

  parser.ExecuteMessege();

#if DEBUG
  client.publish(Network::SUBSCRIBE_TOPIC, (char *)payload, length);
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
  LOG_NL("Connected to broker");
}

void setup()
{
  InitEnginePins();
  // stop gun from moving
  digitalWrite(PIN_GUN_VERTICAL, HIGH);
  client.setServer(Network::MQTT_SERVER, Network::PORT);
  client.setCallback(callback);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(Network::SSID, Network::PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  LOG_NL("Connected to wifi");
  // Tank
  parser.AddEvents(Command::TANK_FORWARD, ForwardFun);
  parser.AddEvents(Command::TANK_BACKWARD, BackwardFun);
  parser.AddEvents(Command::TANK_LEFT, LeftFun);
  parser.AddEvents(Command::TANK_RIGHT, RightFun);
  parser.AddEvents(Command::TANK_STOP, StopFun);
  parser.AddEvents(Command::TANK_SPEED, SpeedFun);
  // Turret
  parser.AddEvents(Command::TURRET_FORWARD, TurretRightFun);
  parser.AddEvents(Command::TURRET_BACKWARD, TurretLeftFun);
  parser.AddEvents(Command::TURRET_STOP, TurretStopFun);
  // Gun
  parser.AddEvents(Command::GUN_MOVE, GunMoveFun);
  parser.AddEvents(Command::GUN_STOP, GunStopFun);
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