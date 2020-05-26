// main file, don't compile with unit tests
#ifndef UNIT_TEST
#ifdef NANO

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "parser/parser.h"
#include "engine/engine.h"
#include "debug.h"

// ================
// ENGINE PINS
// ================

#define RIGHT_FRONT 2
#define RIGHT_BACK 3
#define LEFT_FRONT 4
#define LEFT_BACK 5
#define SIDE_FRONT A2
#define SIDE_BACK A1

#define RIGHT_SPEED 9
#define LEFT_SPEED 10
#define SIDE_SPEED A0

#define MP3_RX 11
#define MP3_TX 12

// ================
// DIODES
// ================

#define LEFT_DIODE 6
#define RIGHT_DIODE 7

// ================
// DEFAULT VALUES
// ================

#define DEF_SPEED 120
#define DEF_SIDE_SPEED 255

#define EVENTS 20
#define BUFFER_LENGTH 100

#define TURN_MULTIPLIER 0.7
#define BLINK_INTERVAL 1000

// ================
// COMMANDS
// ================

const char *FORWARD = "FORWARD";
const char *BACKWARD = "BACKWARD";
const char *LEFT = "LEFT";
const char *RIGHT = "RIGHT";
const char *LEFT_WHILE_DRIVING = "LEFT_WHILE_DRIVING";
const char *RIGHT_WHILE_DRIVING = "RIGHT_WHILE_DRIVING";
const char *STOP = "STOP";
const char *SPEED = "SPEED";
const char *MP3_COMMAND = "MP3_COMMAND";
const char *SIDE_UP = "SIDE_UP";
const char *SIDE_DOWN = "SIDE_DOWN";

// ================
// VARIABLES
// ================

Engine left(LEFT_FRONT, LEFT_BACK, LEFT_SPEED, DEF_SPEED);
Engine right(RIGHT_FRONT, RIGHT_BACK, RIGHT_SPEED, DEF_SPEED);
Engine side(SIDE_FRONT, SIDE_BACK, SIDE_SPEED, DEF_SIDE_SPEED);
Parser parser;
SoftwareSerial mp3(MP3_RX, MP3_TX);

// ================
// SETTING UP PINS
// ================

static void InitEnginePins()
{
  pinMode(RIGHT_FRONT, OUTPUT);
  pinMode(RIGHT_BACK, OUTPUT);
  pinMode(LEFT_FRONT, OUTPUT);
  pinMode(LEFT_BACK, OUTPUT);
  pinMode(SIDE_FRONT, OUTPUT);
  pinMode(SIDE_BACK, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT);
  pinMode(LEFT_SPEED, OUTPUT);
  pinMode(SIDE_SPEED, OUTPUT);
}

static void InitDiodes()
{
  pinMode(LEFT_DIODE, OUTPUT);
  pinMode(RIGHT_DIODE, OUTPUT);
}

// ==============
// MP3 FUNCTIONS
// ==============

void MP3command(int8_t command, int8_t dat1, int8_t dat2)
{
  int8_t Send_buf[8];
  Send_buf[0] = 0x7e;             //starting byte
  Send_buf[1] = 0xff;             //version
  Send_buf[2] = 0x06;             //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command;          //
  Send_buf[4] = 0x00;             //0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = dat1;             //datah
  Send_buf[6] = dat2;             //datal
  Send_buf[7] = 0xef;             //ending byte
  for (uint8_t i = 0; i < 8; i++) //
    mp3.write(Send_buf[i]);
}

void readMP3FromBuffer(const CommandBuffer &buffer)
{
  Number n1 = buffer.FindNumber(1);
  Number n2 = buffer.FindNumber(2);
  Number n3 = buffer.FindNumber(3);

  if (n1.success && n2.success && n3.success)
  {
    LOG("Correct mp3 command: ");
    LOG(n1.number);
    LOG(" ");
    LOG(n2.number);
    LOG(" ");
    LOG(n3.number);
    MP3command(n1.number, n2.number, n3.number);
  }
  else
  {
    LOG_NL("===============================");
    LOG_NL(n1.success);
    LOG_NL(n2.success);
    LOG_NL(n3.success);
    LOG_NL("===============================");
  }
}

// ==============
// ENGINES
// ==============

void SpeedFun(const CommandBuffer &buffer)
{
  Number number = buffer.FindNumber(1);

  if (number.success)
  {
    if (number.number >= 0 && number.number <= 255)
    {
      left.ChangeSpeed((uchar)number.number);
      right.ChangeSpeed((uchar)number.number);
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

void Blink()
{
  static bool state = false;
  static unsigned long lastChange = millis();

  if (millis() - lastChange > BLINK_INTERVAL)
  {
    digitalWrite(LEFT_DIODE, state);
    digitalWrite(RIGHT_DIODE, !state);
    state = !state;
    lastChange = millis();
  }
}

auto Forward_Fun =  [](const CommandBuffer &b) { left.Forward(); right.Forward(); };
auto BackwardFun = [](const CommandBuffer &b) {left.Backward(); left.Backward(); };
auto LeftFun = [](const CommandBuffer &b) {left.Backward(); right.Forward(); };
auto RightFun = [](const CommandBuffer &b) {left.Forward(); right.Backward(); };
auto SideUpFun = [](const CommandBuffer &b) { side.Forward(); };
auto SideDownFun =  [](const CommandBuffer &b) { side.Backward(); };

void setup()
{
  Serial.begin(115200);
  mp3.begin(9600);

  InitEnginePins();
  InitDiodes();

  parser.AddEvents(FORWARD, Forward_Fun);
  parser.AddEvents(BACKWARD, BackwardFun);
  parser.AddEvents(SPEED, SpeedFun);
  parser.AddEvents(LEFT, LeftFun);
  parser.AddEvents(RIGHT, RightFun);
  parser.AddEvents(SIDE_UP, SideUpFun);
  parser.AddEvents(SIDE_DOWN, SideDownFun);
  parser.AddEvents(MP3_COMMAND, readMP3FromBuffer);

  MP3command(6, 0, 15);
}

void loop()
{
  if (parser.ReadToBuffer())
    parser.ExecuteMessege();
  Blink();
}


#endif // NANOA
#endif // UNIT_TEST