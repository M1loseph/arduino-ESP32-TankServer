#include <Arduino.h>
#include "parser.h"

int Parser::ReadStream(Stream &stream)
{
    if (stream.available())
    {
        unsigned long start = millis();
        bool foundEnd = false;
        // when null or \r or \n is found, we end the loop
        while (!foundEnd && !m_Buffer.IsFull() && millis() - start < TIMEOUT)
        {
            if (stream.available())
            {
                char c = (char)stream.read();
                if (c == '\0' || c == '\r' || c == '\n')
                    foundEnd = true;
                else
                    m_Buffer.PushBack(c);
            }
        }
        LOG_NL(' ');
        LOG("Read characters: ");
        LOG_NL(m_Buffer.Length());
        LOG_NL(m_Buffer.C_Ptr());
        return m_Buffer.Length();
    }
    return 0;
}

void Parser::ExecuteMessege()
{
    for (size_t i = 0; i < m_CurrentEvents; i++)
    {
        const char *command = m_Buffer.Command();
        if (strcmp(command, m_Events[i]) == 0)
        {
            LOG("Executing ");
            LOG_NL(m_Events[i]);
            m_Functions[i](m_Buffer);
            break;
        }
    }
    m_Buffer.Clear();
}

void Parser::AddEvent(const char *Command, void (*callbacFun)(const CommandBuffer &buffer))
{
    if (Command && callbacFun && m_CurrentEvents < MAX_EVENTS)
    {
        m_Events[m_CurrentEvents] = Command;
        m_Functions[m_CurrentEvents] = callbacFun;
        ++m_CurrentEvents;
    }
}

CommandBuffer &Parser::GetBuff()
{
    return m_Buffer;
}

void Parser::ExecuteInterval()
{
    for (size_t i = 0; i < m_CurrentEvents; i++)
    {
        // first check if interval is active -> 0 means it is inactive
        // if interval is active, check when it was last used
        // if the interval has passed, update last time and execute command
        IntervalInfo &interval = m_Intervals[i];
        if (interval.interval != 0 && millis() - interval.lastUpdate > interval.interval)
        {
            m_Functions[i](m_Buffer);
            interval.lastUpdate = millis();
        }
    }
}

bool Parser::SetInterval(const char *command, size_t interval)
{
    if (command)
    {
        for (size_t i = 0; i < m_CurrentEvents; i++)
        {
            if (strcmp(command, m_Events[i]) == 0)
            {
                m_Intervals[i].interval = interval;
                return true;
            }
        }
    }
    return false;
}

int Parser::GetInterval(const char *command)
{
    if (command)
    {
        for (size_t i = 0; i < m_CurrentEvents; i++)
            if (strcmp(command, m_Events[i]) == 0)
                return m_Intervals[i].interval;
    }
    return Parser::INTERVAL_NOT_FOUND;
}