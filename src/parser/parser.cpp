#include <Arduino.h>
#include "parser.h"

int Parser::ReadStream(Stream *stream)
{
    if (stream->available())
    {
        unsigned long start = millis();
        bool foundEnd = false;
        // when null or \r or \n is found, we end the loop
        while (!foundEnd && !m_Buffer.IsFull() && millis() - start < TIMEOUT)
        {
            if (stream->available())
            {
                char c = (char)stream->read();
                if (c == '\0' || c == '\r' || c == '\n')
                    foundEnd = true;
                else
                    m_Buffer.PushBack(c);
            }
        }

        LOG("Read characters: ");
        LOG_NL(m_Buffer.Length());
        LOG_NL(m_Buffer.C_Ptr());
        return m_Buffer.Length();
    }
    return 0;
}

void Parser::ExecuteMessege()
{
    for (int i = 0; i < currentEvents; i++)
    {
        const char *command = m_Buffer.Command();
        if (strcmp(command, m_Events[i]) == 0)
        {
            m_Functions[i](m_Buffer);
            break;
        }
    }
    m_Buffer.Clear();
}

void Parser::AddEvents(const char *Command, void (*callbacFun)(const CommandBuffer &buffer))
{
    if (Command && callbacFun && currentEvents < MAX_EVENTS)
    {
        m_Events[currentEvents] = Command;
        m_Functions[currentEvents] = callbacFun;
        ++currentEvents;
    }
}

CommandBuffer &Parser::GetBuff()
{
    return m_Buffer;
}