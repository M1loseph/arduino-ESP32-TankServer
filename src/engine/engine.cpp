#include <Arduino.h>
#include "debug.h"
#include "engine.h"

Engine::Engine(uchar forward, uchar backward, uchar speed, uchar defSpeed) : m_ForwardPin(forward),
                                                                             m_BackwardPin(backward),
                                                                             m_SpeedPin(speed),
                                                                             m_CurrentSpeed(defSpeed)
{
}

void Engine::Forward()
{
  digitalWrite(m_SpeedPin, LOW);
  digitalWrite(m_ForwardPin, HIGH);
  digitalWrite(m_BackwardPin, LOW);
  analogWrite(m_SpeedPin, m_CurrentSpeed);
  LOG_NL("Driving forward");
}

void Engine::Backward()
{
  digitalWrite(m_SpeedPin, LOW);
  digitalWrite(m_ForwardPin, LOW);
  digitalWrite(m_BackwardPin, HIGH);
  analogWrite(m_SpeedPin, m_CurrentSpeed);
  LOG_NL("Driving backward");
}

void Engine::Stop()
{
  digitalWrite(m_SpeedPin, LOW);
  digitalWrite(m_ForwardPin, LOW);
  digitalWrite(m_BackwardPin, LOW);
  LOG_NL("Stopped");
}

void Engine::ChangeSpeed(uchar newSpeed)
{
  m_CurrentSpeed = newSpeed;
  analogWrite(m_SpeedPin, m_CurrentSpeed);
  LOG("Changed speed to: ");
  LOG_NL(newSpeed);
}
