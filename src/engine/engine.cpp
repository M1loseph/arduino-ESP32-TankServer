#include <Arduino.h>
#include "debug.h"
#include "engine.h"

StaticEngine::StaticEngine(uchar forward, uchar backward) : m_ForwardPin(forward),
                                                            m_BackwardPin(backward)
{
}

void StaticEngine::Forward()
{
  digitalWrite(m_ForwardPin, HIGH);
  digitalWrite(m_BackwardPin, LOW);
  LOG_NL("Driving forward");
}

void StaticEngine::Backward()
{
  digitalWrite(m_ForwardPin, LOW);
  digitalWrite(m_BackwardPin, HIGH);
  LOG_NL("Driving backward");
}

void StaticEngine::Stop()
{
  digitalWrite(m_ForwardPin, LOW);
  digitalWrite(m_BackwardPin, LOW);
  LOG_NL("Stopped");
}

void StaticEngine::TurnLeft(StaticEngine &left, StaticEngine &right)
{
  left.Backward();
  right.Forward();
}

void StaticEngine::TurnRight(StaticEngine &left, StaticEngine &right)
{
  left.Forward();
  right.Backward();
}

Engine::Engine(uchar forward, uchar backward, uchar speed, uchar defSpeed) : StaticEngine(forward, backward),
                                                                             m_SpeedPin(speed),
                                                                             m_CurrentSpeed(defSpeed)
{
}

void Engine::Forward()
{
  digitalWrite(m_SpeedPin, LOW);
  StaticEngine::Forward();
  analogWrite(m_SpeedPin, m_CurrentSpeed);
}

void Engine::Backward()
{
  digitalWrite(m_SpeedPin, LOW);
  StaticEngine::Backward();
  analogWrite(m_SpeedPin, m_CurrentSpeed);
}

void Engine::Stop()
{
  digitalWrite(m_SpeedPin, LOW);
  StaticEngine::Stop();
}

void Engine::ChangeSpeed(uchar newSpeed)
{
  m_CurrentSpeed = newSpeed;
  analogWrite(m_SpeedPin, m_CurrentSpeed);
  LOG("Changed speed to: ");
  LOG_NL(newSpeed);
}