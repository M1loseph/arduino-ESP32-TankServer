#ifdef ESP_32
#ifndef UNIT_TEST

// https://www.youtube.com/watch?v=7h2bE2vNoaY

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
#include "network.h"
#include "json_messages.h"
#include "commands.h"

#include "functions/engines.h"

// ================
// VARIABLES
// ================

Parser parser;
char JSON_BUFFER[300];

// void Reconnect()
// {
//   while (!client.connected())
//   {
//     if (client.connect(Network::ID))
//       client.subscribe(Network::SUBSCRIBE_TOPIC);
//     delay(5000);
//   }
//   client.publish(Network::DEBUG_TOPIC, "Tank has been connected!");

//   LOG_NL("Connected to broker");
// }

// void SendStateMQTT(const CommandBuffer &b)
// {

//   Float temperetures[TEMPERATURES];
//   Integer rest[REST];

//   for (size_t i = 0; i < TEMPERATURES + REST; i++)
//   {
//     if (i < TEMPERATURES)
//     {
//       // +1 to ignore the command
//       temperetures[i] = b.float_at(i + 1);
//     }
//     else
//     {
//       // -2 to offset temperatures
//       rest[i - TEMPERATURES] = b.int_at(i + 1);
//     }
//   }
//   // check if all numbers were succesfully read
//   bool validData = true;

//   for (size_t i = 0; i < TEMPERATURES; i++)
//     if (!temperetures[i].success)
//       validData = false;

//   for (size_t i = 0; i < REST; i++)
//     if (!rest[i].success)
//       validData = false;

//   if (validData)
//   {
//     // format message
//     sprintf(BUFFER, JSON_STATUS,
//             temperetures[0].value,
//             temperetures[1].value,
//             rest[0].value,
//             rest[1].value,
//             rest[2].value,
//             rest[3].value,
//             rest[4].value,
//             rest[5].value,
//             rest[6].value,
//             rest[7].value,
//             rest[8].value,
//             rest[9].value,
//             rest[10].value,
//             rest[11].value,
//             rest[12].value,
//             left.Direction(),
//             right.Direction(),
//             left.CurrentSpeed(),
//             right.CurrentSpeed());

//     // and finally send it over mqtt
//     client.publish(Network::STATUS_TOPIC, BUFFER);
//   }
// }

// void SendDistanceMQTT(const CommandBuffer &b)
// {
//   Integer dist = b.int_at(1);
//   if (dist.success)
//   {
//     sprintf(BUFFER, JSON_DISTANCE, dist.value);
//     client.publish(Network::DISTANCE_TOPIC, BUFFER);
//   }
// }

// ==============
// SETUP AND LOOP
// ==============

void setup()
{
  init_engines();

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
}

void loop()
{
  // client.loop();
  // if (!client.connected())
  // {
  //   left.Stop();
  //   right.Stop();
  //   Reconnect();
  // }
  // UpdateSpeed();

  if (parser.ReadStream(Serial))
    parser.ExecuteBuffer();
}

#endif // UNIT_TEST
#endif // NODEMCU