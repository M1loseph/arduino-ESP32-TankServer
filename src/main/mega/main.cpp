#ifndef UNIT_TEST
#ifdef MEGA

#include <Arduino.h>

#include "parser/parser.h"
#include "arm/arm.h"

static constexpr uchar PINS[] = {13, 12, 11, 10, 9, 8};
static constexpr uchar MIN[] = {0, 30, 0, 0, 0, 40};
static constexpr uchar MAX[] = {180, 180, 180, 180, 180, 60};
static constexpr uchar DEF_ANGLES[] = {50, 90, 90, 30, 90, 90};

Arm arm(PINS, MIN, MAX, DEF_ANGLES);
Parser parser;

void setup()
{
  arm.Attach();
  Serial.begin(115200);
}

void loop()
{
  if (parser.ReadSerial())
  {
    parser.ExecuteMessege();

    Serial.println(parser.GetBuff().C_Ptr());
    const char *command = parser.GetBuff().Command();
    if (command)
    {
      char c_command = *command;
      auto number = parser.GetBuff().FindNumber(1);

      if (number.success)
        arm.Parse(c_command, number.value);
    }
    parser.GetBuff().Clear();
  }
  arm.Update();
}

#endif // MEGA
#endif // UNIT_TEST
