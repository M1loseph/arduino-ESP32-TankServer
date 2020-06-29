// main file, don't compile with unit tests
#ifndef UNIT_TEST
#ifdef MEGA

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <SPI.h>
#include <SD.h>

#include "main/commonFun.h"
#include "arm.h"
#include "parser/parser.h"
#include "commands.h"
#include "mp3.h"

typedef unsigned char uchar;

// ================
// VARIABLES
// ================

Parser parser;

constexpr size_t TEMPERATURE_TIMEOUT = 1000; // timeout in milliseconds
constexpr uchar ONE_WIRE_BUS = 13;
constexpr size_t TEMPERATURE_RESOLUTION = 11;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress inside, outside;

bool awaitingForState = false;
// needs to be init in setup
unsigned long lastTemperatureCheck;

// gyro and acc sensor
MPU6050 accelgyro;

// distance sensor
constexpr uchar TRIG = 31;
constexpr uchar ECHO = 33;

// SD card Slave Select
constexpr int8_t SD_CARD_SLAVE_SELECT = 53;
constexpr size_t FILE_NAME_BUFFER_LENGTH = 30;
bool CardConnected = false;
bool OpenNewFile = false;
char FileNameBuffer[FILE_NAME_BUFFER_LENGTH];
size_t NextCommandDelay = 0;

// ==============
// MP3 FUNCTIONS
// ==============

void ReadMP3FromBuffer(const CommandBuffer &buffer)
{
  static constexpr size_t NUMBERS = 3U;
  Integer numbers[NUMBERS];
  for (size_t i = 0; i < NUMBERS; i++)
    numbers[i] = buffer.IntAt(i + 1);

  // check the success of each number
  // later check if each of them is 0 <= value <= 255
  for (size_t i = 0; i < NUMBERS; i++)
    if (!numbers[i].success || numbers[i].value < 0 || numbers[i].value > 255)
      return;

  MP3command(Serial3, numbers[0].value, numbers[1].value, numbers[2].value);
}

// ==============
//  SERVOS
// ==============

void PrepareForSDExecute(const CommandBuffer &b)
{
  if (CardConnected)
  {
    const char *fileName = b.WordAt(1);
    if (fileName)
    {
      OpenNewFile = true;
      // to make sure the null is also included
      size_t fNameLength = strlen(fileName) + 1;
      for (size_t i = 0; i < FILE_NAME_BUFFER_LENGTH && i < fNameLength; i++)
        FileNameBuffer[i] = fileName[i];
      LOG_NL("Ready to start execution");
    }
  }
}

void ExecuteSD()
{
  static File file;
  static unsigned long lastExecution = millis();
  // by defult we dont wait, sice nothing has been executed yet
  // we do anything only if the card has been connected
  if (CardConnected)
  {
    // to open a new file we need to close a previous one
    if (OpenNewFile)
    {
      file.close();
      LOG("Opening file: ");
      LOG_NL(FileNameBuffer);
      file = SD.open(FileNameBuffer);
      // to prevent old script blocking new one
      NextCommandDelay = 0;
      // to not open the same file again
      OpenNewFile = false;
    }

    // if the file is opened and waiting for next execution has ended
    if (file && millis() - lastExecution > NextCommandDelay)
    {
      // reset next command delay
      NextCommandDelay = 0;
      if (file.available())
      {
        // keep reading one line, when \n sign is found, stop reading
        while (file.available())
        {
          char c = file.read();
          if (c == '\n' || c == '\r')
            break;
          parser.GetBuff().PushBack(c);
        }
        if (parser.GetBuff().Length() > 0)
          parser.ExecuteMessege();
        lastExecution = millis();
      }
      else
      {
        LOG_NL("Closing file");
        // when there is nothing more in the file -> close it
        file.close();
      }
    }
  }
}

void SetExecutionDelay(const CommandBuffer &b)
{
  Integer newDelay = b.IntAt(1);
  if (newDelay.success && newDelay.value >= 0)
  {
    NextCommandDelay = newDelay.value;
    LOG("New delay: ");
    LOG_NL(NextCommandDelay);
  }
  else
  {
    LOG("Failed new delay: ");
    LOG_NL(newDelay.value);
  }
}

size_t ReadDistance()
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long duration = pulseIn(ECHO, HIGH);
  return (size_t)(duration * 0.034 / 2);
}

void RequestState(const CommandBuffer &buffer)
{
  // to make sure we dont get flooded with requests
  if (!awaitingForState)
  {
    // we need to ask termometers for temperature
    sensors.requestTemperatures();
    awaitingForState = true;
    lastTemperatureCheck = millis();
  }
}

void SendState()
{
  static const size_t stateLength = strlen(Command::Common::STATE);

  // if ${TEMPERATURE_TIMEOUT} has passed and we want a timeout
  if (awaitingForState && millis() - lastTemperatureCheck > TEMPERATURE_TIMEOUT)
  {
    float tempOutside = sensors.getTempC(outside);
    float tempInside = sensors.getTempC(inside);

    int16_t resoults[6];
    size_t distance = ReadDistance();
    accelgyro.getMotion6(resoults, resoults + 1, resoults + 2, resoults + 3, resoults + 4, resoults + 5);

    // we dont want a null character
    for (size_t i = 0; i < stateLength; i++)
      Serial.write(Command::Common::STATE[i]);

    Serial.write(' ');
    // time for sending current state
    Serial.print(tempOutside);
    Serial.write(' ');
    Serial.print(tempInside);
    Serial.print(' ');

    // sevo positions
    for (size_t i = 0; i < SERVOS; i++)
    {
      Serial.print(currentAngles[i]);
      Serial.write(' ');
    }

    for (size_t i = 0; i < 6; i++)
    {
      Serial.print(resoults[i]);
      Serial.write(' ');
    }

    Serial.print(distance);
    // to ensure the communication ends here
    Serial.write('\n');
    awaitingForState = false;
  }
}

void SendDistance(const CommandBuffer &b)
{
  static const size_t distanceLength = strlen(Command::Common::DISTANCE);
  size_t distance = ReadDistance();

  for (size_t i = 0; i < distanceLength; i++)
    Serial.write(Command::Common::DISTANCE[i]);

  Serial.write(' ');

  Serial.print(distance);
  // to ensure the communication ends here
  Serial.write('\n');
}

void SetIntervalFromOutside(const CommandBuffer &b)
{
  const char *command = b.WordAt(1);
  Integer interval = b.IntAt(2);
  if (command && interval.success && interval.value >= 0)
    parser.SetInterval(command, interval.value);
}

// ==============
// Parser functions
// ==============

auto BASE_FUN = [](const CommandBuffer &b) { ChangeServoDirection(BASE_PIN, b.IntAt(1)); };
auto SHOULDER_FUN = [](const CommandBuffer &b) { ChangeServoDirection(SHOULDER_PIN, b.IntAt(1)); };
auto ELBOW_1_FUN = [](const CommandBuffer &b) { ChangeServoDirection(ELBOW_1_PIN, b.IntAt(1)); };
auto ELBOW_2_FUN = [](const CommandBuffer &b) { ChangeServoDirection(ELBOW_2_PIN, b.IntAt(1)); };
auto WRIST_FUN = [](const CommandBuffer &b) { ChangeServoDirection(WRIST_PIN, b.IntAt(1)); };
auto CLAW_FUN = [](const CommandBuffer &b) { ChangeServoDirection(CLAW_PIN, b.IntAt(1)); };

void setup()
{
  Serial.begin(115200);
  Serial3.begin(9600);
  MP3command(Serial3, 6, 0, 15);

  sensors.begin();
  sensors.getAddress(outside, 0);
  sensors.getAddress(inside, 1);
  // resolution and NO waiting for conversion
  sensors.setResolution(TEMPERATURE_RESOLUTION);
  sensors.setWaitForConversion(false);
  lastTemperatureCheck = millis();

  LOG("Devices count: ");
  LOG_NL(sensors.getDeviceCount());
  LOG("Current resolution: ");
  LOG_NL(sensors.getResolution(outside));

  Wire.begin();
  accelgyro.initialize();

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates

  for (size_t i = 0; i < SERVOS; i++)
  {
    pwm.writeMicroseconds(i, map(DEF_ANGLES[i], 0, 180, PULSE_MS_MIN, PULSE_MS_MAX));
    currentAngles[i] = DEF_ANGLES[i];
  }

  parser.AddEvent(Command::Mega::INTERVAL, SetIntervalFromOutside);
  parser.AddEvent(Command::Mega::BASE, BASE_FUN);
  parser.AddEvent(Command::Mega::SHOULDER, SHOULDER_FUN);
  parser.AddEvent(Command::Mega::ELBOW_1, ELBOW_1_FUN);
  parser.AddEvent(Command::Mega::ELBOW_2, ELBOW_2_FUN);
  parser.AddEvent(Command::Mega::WRIST, WRIST_FUN);
  parser.AddEvent(Command::Mega::CLAW, CLAW_FUN);
  parser.AddEvent(Command::Common::STATE, RequestState);
  parser.AddEvent(Command::Common::DISTANCE, SendDistance);
  parser.AddEvent(Command::Common::MOVE, MoveOverSerial);
  parser.AddEvent(Command::Mega::EXECUTE, PrepareForSDExecute);
  parser.AddEvent(Command::Mega::DELAY, SetExecutionDelay);
  parser.AddEvent(Command::Mega::MP3_COMMAND, ReadMP3FromBuffer);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIG, LOW);

  // to make sure the card was connected
  if (SD.begin())
    CardConnected = true;

  LOG("Connecting SD card: ");
  LOG_NL(CardConnected);
}

void loop()
{
  if (parser.ReadStream(Serial))
    parser.ExecuteMessege();
  parser.ExecuteInterval();
  UpdateServos();
  SendState();
  ExecuteSD();
}

#endif // MEGA
#endif // UNIT_TEST