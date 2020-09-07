#if 0
#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "typedefs.h"

class StaticEngine
{
public:
  explicit StaticEngine(uchar forward, uchar backward);
  ~StaticEngine() = default;

  // not declared as virtual to save computation time
  virtual void Forward();
  virtual void Backward();
  virtual void Stop();

  static void TurnLeft(StaticEngine &left, StaticEngine &right);
  static void TurnRight(StaticEngine &left, StaticEngine &right);

  int Direction() const;

  static constexpr int MOVEMENT_BACKWARD = -1;
  static constexpr int MOVEMENT_STOP = 0;
  static constexpr int MOVEMENT_FORWARD = 1;

protected:
  int m_Direction = MOVEMENT_STOP;
  uchar m_ForwardPin;
  uchar m_BackwardPin;
};

class Engine : public StaticEngine
{
public:
  explicit Engine(uchar forward, uchar backward, uchar speed, size_t defSpeed);
  ~Engine() = default;

  void Forward() override;
  void Backward() override;
  void Stop() override;
  void ChangeSpeed(size_t newSpeed);
  size_t CurrentSpeed();

  static constexpr int E_MAX_SPEED = 1023;

private:
  uchar m_SpeedPin;
  size_t m_CurrentSpeed;
};

#endif // __ENGINE_H

#endif