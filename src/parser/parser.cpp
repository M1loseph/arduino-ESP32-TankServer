#include <Arduino.h>
#include "parser.h"
#include "debug.h"

#if PARSER_DEBUG

#define LOG_PARSER(message) LOG(message)
#define LOG_PARSER_NL(message) LOG_NL(message)

#else

#define LOG_PARSER(message)
#define LOG_PARSER_NL(message)

#endif

unsigned Parser::ReadStream(Stream &stream)
{
    if (stream.available())
    {
        unsigned long start = millis();
        bool foundEnd = false;
        // when null or \r or \n is found, we end the loop
        while (!foundEnd && !m_Buffer.is_full() && millis() - start < TIMEOUT)
        {
            if (stream.available())
            {
                char c = (char)stream.read();
                if (c == '\0' || c == '\r' || c == '\n')
                    foundEnd = true;
                else
                    m_Buffer.push_back(c);
            }
        }
        LOG_PARSER_NL(' ');
        LOG_PARSER("Read characters: ");
        LOG_PARSER_NL(m_Buffer.length());
        LOG_PARSER_NL(m_Buffer.c_ptr());
        return m_Buffer.length();
    }
    return 0;
}

void Parser::ExecuteBuffer()
{
    for (size_t i = 0; i < m_CurrentEvents; i++)
    {
        const char *command = m_Buffer.command();
        if (strcmp(command, m_Events[i].name) == 0)
        {
            LOG_PARSER("Executing ");
            LOG_PARSER_NL(m_Events[i].name);
            m_Events[i].event(m_Buffer);
            break;
        }
    }
    m_Buffer.clear();
}

bool Parser::AddEvent(const char *Command, Event event, unsigned interval)
{
    if (Command && event && !IsFull())
    {
        m_Events[m_CurrentEvents].name = Command;
        m_Events[m_CurrentEvents].event = event;
        m_Events[m_CurrentEvents].interval = interval;
        ++m_CurrentEvents;
        return true;
    }
    return false;
}

CommandBuffer &Parser::GetBuff()
{
    return m_Buffer;
}

void Parser::ExecuteIntervals()
{
    for (size_t i = 0; i < m_CurrentEvents; i++)
    {
        // first check if interval is active -> 0 means it is inactive
        // if interval is active, check when it was last used
        // if the interval has passed, update last time and execute command
        EventData &interval = m_Events[i];
        if (interval.interval != 0 && millis() - interval.lastUpdate > interval.interval)
        {
            m_Events[i].event(m_Buffer);
            interval.lastUpdate = millis();
        }
    }
}

bool Parser::SetInterval(const char *command, size_t interval)
{
    // check if command is valid
    if (command)
    {
        for (size_t i = 0; i < m_CurrentEvents; i++)
        {
            if (strcmp(command, m_Events[i].name) == 0)
            {
                m_Events[i].interval = interval;
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
            if (strcmp(command, m_Events[i].name) == 0)
                return m_Events[i].interval;
    }
    return Parser::INTERVAL_NOT_FOUND;
}

bool Parser::IsFull()
{
    // it should never be bigger, but made that just in case
    return m_CurrentEvents >= MAX_EVENTS;
}