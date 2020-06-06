#ifndef NODEMCU
#include "arm.h"
#include <Arduino.h>

Arm::Arm(const uchar *pins, const uchar *minConstrains, const uchar *maxConstrains, const uchar *defAngles)
{
  for (size_t i = 0; i < Arm::SERVOS; i++)
  {
    _pins[i] = pins[i];
    _minConstrain[i] = minConstrains[i];
    _maxConstrain[i] = maxConstrains[i];
    _current[i] = defAngles[i];
    _destination[i] = defAngles[i];
  }
}

void Arm::Attach()
{
  for (size_t i = 0; i < Arm::SERVOS; i++)
  {
    _servos[i].attach(_pins[i]);
    _servos[i].write(_current[i]);
  }
}

Arm::~Arm()
{
  for (size_t i = 0; i < Arm::SERVOS; i++)
    _servos[i].detach();
}

bool Arm::Parse(uchar index, uchar angle)
{
  if (index >= 0 && index < Arm::SERVOS)
  {
    if (angle <= _maxConstrain[index] && angle >= _minConstrain[index])
    {
      _destination[index] = angle;
      return true;
    }
  }
  return false;
}

void Arm::Update()
{
  static unsigned long lastUpdate = millis();
  if (millis() - lastUpdate > TIME_BETWEEN_UPDATES)
  {
    for (size_t i = 0; i < Arm::SERVOS; i++)
    {
      if (_current[i] != _destination[i])
      {
        if (_destination[i] > _current[i])
          _current[i]++;
        else
          _current[i]--;
        _servos[i].write(_current[i]);
      }
    }
  }
}

#endif // NODEMCU