#ifdef NODEMCU
#ifndef UNIT_TEST

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "parser/parser.h"
#include "engine/engine.h"
#include "debug.h"
#include "network.h"
#include "commands.h"

// ================
// ENGINE PINS
// ================

// https://www.youtube.com/watch?v=7h2bE2vNoaY

constexpr uchar PIN_RIGHT_FRONT = D0;
constexpr uchar PIN_RIGHT_BACK = D1;
constexpr uchar PIN_LEFT_FRONT = D3;
constexpr uchar PIN_LEFT_BACK = D2;

constexpr uchar PIN_LEFT_SPEED = D4;
constexpr uchar PIN_RIGHT_SPEED = D5;

constexpr uint DEF_SPEED = 500U;

// ================
// VARIABLES
// ================

Engine left(PIN_LEFT_FRONT, PIN_LEFT_BACK, PIN_LEFT_SPEED, DEF_SPEED);
Engine right(PIN_RIGHT_FRONT, PIN_RIGHT_BACK, PIN_RIGHT_SPEED, DEF_SPEED);
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
  pinMode(PIN_LEFT_SPEED, OUTPUT);
  pinMode(PIN_LEFT_FRONT, OUTPUT);
  pinMode(PIN_LEFT_BACK, OUTPUT);
  pinMode(PIN_RIGHT_SPEED, OUTPUT);
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

void MoveToNano(const CommandBuffer &b)
{
  const char *commandToSend = b.Word(1);
  if (commandToSend)
  {
    // get index of message we want to send
    size_t index = (size_t)(commandToSend - b.C_Ptr());
    // send entire messege
    for (size_t i = index; i < b.Length(); i++)
    {
      char toSend = b.C_Ptr()[i];
      if (toSend == b.NULL_CHAR)
        Serial.write(' ');
      else
        Serial.write(toSend);
    }
    // to stop communation
    Serial.write('\n');
  }
}

auto ForwardLeftFun = [](const CommandBuffer &b) { left.Forward(); };
auto ForwardRightFun = [](const CommandBuffer &b) { right.Forward(); };

auto BackwardLeftFun = [](const CommandBuffer &b) { left.Backward(); };
auto BackwardRightFun = [](const CommandBuffer &b) { right.Backward(); };

auto StopLeftFun = [](const CommandBuffer &b) { left.Stop(); };
auto StopRightFun = [](const CommandBuffer &b) { right.Stop(); };

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
  WiFi.mode(WIFI_STA);
  WiFi.begin(Network::SSID, Network::PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);
  // Tank
  // move command to nano
  parser.AddEvents(Command::Mcu::MOVE, MoveToNano);
  parser.AddEvents(Command::Mcu::TANK_FORWARD_L, ForwardLeftFun);
  parser.AddEvents(Command::Mcu::TANK_FORWARD_R, ForwardRightFun);
  parser.AddEvents(Command::Mcu::TANK_BACKWARD_L, BackwardLeftFun);
  parser.AddEvents(Command::Mcu::TANK_BACKWARD_R, BackwardRightFun);
  parser.AddEvents(Command::Mcu::TANK_STOP_L, StopLeftFun);
  parser.AddEvents(Command::Mcu::TANK_STOP_R, StopRightFun);

  parser.AddEvents(Command::Mcu::TANK_SPEED, SpeedFun);

  Serial.begin(115200);
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