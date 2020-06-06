#ifndef NODEMCU
#ifndef __ARM_H__
#define __ARM_H__

#include <Arduino.h>
#include <Servo.h>

typedef unsigned char uchar;
typedef unsigned int size_t;

class Arm
{
public:
  Arm(const uchar *pins, const uchar *minConstrains, const uchar *maxConstrains, const uchar *defAngles);
  ~Arm();

  void Attach();
  bool Parse(uchar motor, uchar angle);

  void Update();

  static constexpr size_t SERVOS = 6U;
  static constexpr size_t TIME_BETWEEN_UPDATES = 10U;

private:
  Servo _servos[SERVOS];       // engines
  uchar _pins[SERVOS];         // pin
  uchar _minConstrain[SERVOS]; // minimun angle a servo can take
  uchar _maxConstrain[SERVOS]; // maximum angle
  uchar _current[SERVOS];      // current
  uchar _destination[SERVOS];  // destination
};

#endif // __ARM_H__
#endif // NODEMCU