#include <Arduino.h>
#include "parser.h"

int Parser::ReadStream(Stream* stream)
{
    if (stream->available())
    {
        unsigned long start = millis();
        bool foundNull = false;
        // when null is found, we end the loop
        // each space is replaced by null
        // each \r and \n is ignored
        while (!foundNull && !m_Buffer.IsFull() && millis() - start < TIMEOUT)
        {
            if (stream->available())
            {
                char c = (char)stream->read();
                if (c == '\0')
                    foundNull = true;
                if (c == ' ')
                    c = '\0';
                if (c != '\r' && c != '\n') // remove both these signs, we dont want them
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
        if (strcmp(m_Buffer.C_Ptr(), m_Events[i]) == 0)
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