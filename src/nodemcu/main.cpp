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

// ================
// ENGINE PINS
// ================

constexpr int RIGHT_FRONT = D0;
constexpr int RIGHT_BACK = D1;
constexpr int LEFT_FRONT = D3;
constexpr int LEFT_BACK = D2;

// ================
// COMMANDS
// ================

constexpr char *FORWARD = "FORWARD";
constexpr char *BACKWARD = "BACKWARD";
constexpr char *LEFT = "LEFT";
constexpr char *RIGHT = "RIGHT";
constexpr char *STOP = "STOP";
constexpr char *MP3_COMMAND = "MP3_COMMAND";

// ================
// VARIABLES
// ================

StaticEngine left(LEFT_FRONT, LEFT_BACK);
StaticEngine right(RIGHT_FRONT, RIGHT_BACK);
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
  pinMode(LEFT_FRONT, OUTPUT);
  pinMode(LEFT_BACK, OUTPUT);
}

// ==============
// TYPICAL ARDUINO
// ==============

auto ForwardFun = [](const CommandBuffer &b) { left.Forward(); right.Forward(); };
auto BackwardFun = [](const CommandBuffer &b) {left.Backward(); right.Backward(); };
auto LeftFun = [](const CommandBuffer &b) { Engine::TurnLeft(left, right); };
auto RightFun = [](const CommandBuffer &b) { Engine::TurnRight(left, right); };
auto StopFun = [](const CommandBuffer &b) {left.Stop(); right.Stop(); };

void callback(const char *topic, byte *payload, unsigned int length)
{
  for (size_t i = 0; i < length; i++)
    parser.GetBuff().PushBack(payload[i]);

  parser.ExecuteMessege();
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect("ESP8266Client"))
      client.subscribe(Network::SUBSCRIBE_TOPIC);
    delay(5000);
  }
  Serial.println("Connected to broker");
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
  Serial.println("Connected to wifi");

  parser.AddEvents(FORWARD, ForwardFun);
  parser.AddEvents(BACKWARD, BackwardFun);
  parser.AddEvents(LEFT, LeftFun);
  parser.AddEvents(RIGHT, RightFun);
  parser.AddEvents(STOP, StopFun);
}

void loop()
{
  client.loop();
  if (!client.connected())
    reconnect();
}

#endif // UNIT_TEST
#endif // NODEMCU