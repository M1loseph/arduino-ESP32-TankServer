#ifndef __ENGINE_H
#define __ENGINE_H

using uchar = unsigned char;

class Engine
{
public:
  explicit Engine(uchar forward, uchar backward, uchar speed, uchar defSpeed);
  ~Engine() = default;

  void Forward();
  void Backward();
  void Stop();
  void ChangeSpeed(uchar newSpeed);

private:
  uchar m_ForwardPin;
  uchar m_BackwardPin;
  uchar m_SpeedPin;
  uchar m_CurrentSpeed;
};

void TurnLeft(Engine& left, Engine& right);
void TurnRight(Engine& left, Engine& right);
void TurnLeftWhileDriving(Engine& left, Engine& right);

#endif // __ENGINE_H
