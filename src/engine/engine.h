#ifndef __ENGINE_H
#define __ENGINE_H

using uchar = unsigned char;

class StaticEngine
{
public:
  explicit StaticEngine(uchar forward, uchar backward);
  ~StaticEngine() = default;

  // not declared as virtual to save computation time
  void Forward();
  void Backward();
  void Stop();

  static void TurnLeft(StaticEngine &left, StaticEngine &right);
  static void TurnRight(StaticEngine &left, StaticEngine &right);

protected:
  uchar m_ForwardPin;
  uchar m_BackwardPin;
};

class Engine : public StaticEngine
{
public:
  explicit Engine(uchar forward, uchar backward, uchar speed, uchar defSpeed);
  ~Engine() = default;

  void Forward();
  void Backward();
  void Stop();
  void ChangeSpeed(uchar newSpeed);

private:
  uchar m_SpeedPin;
  uchar m_CurrentSpeed;
};


#endif // __ENGINE_H
