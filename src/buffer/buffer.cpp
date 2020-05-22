#include <Arduino.h>
#include "buffer.h"

bool CommandBuffer::PushBack(char c)
{
    if (m_CurrentLength < BUFFER_LENGTH)
    {
        if(c == ' ') c = '\0';
        m_Buffer[m_CurrentLength++] = c;
        LOG(m_Buffer[m_CurrentLength - 1]);
        return true;
    }
    return false;
}

void CommandBuffer::Clear()
{
    memset(m_Buffer, '\0', BUFFER_LENGTH);
    m_CurrentLength = 0;
    LOG_NL("Cleared memory");
}

Number CommandBuffer::FindNumber(int wordIndex) const
{
    if (0 <= wordIndex)
    {
        int index = 0;
        for (int word = 0; word <= wordIndex && index < Length(); word++)
        {
            if (word != wordIndex)
            {
                // jump over NULLs
                while (index < Length() && m_Buffer[index] == '\0')
                    ++index;

                // jump over normal string
                while (index < Length() && m_Buffer[index] != '\0')
                    ++index;
            }
            else
            {
                // jump over NULLs
                while (index < Length() && m_Buffer[index] == '\0')
                    ++index;

                // make sure we are not at the end
                if (m_Buffer[index] != '\0')
                {
                    const char *numberPtr = m_Buffer + index;
                    bool validNumber = true;

                    while (index < Length() && m_Buffer[index] != '\0' && validNumber)
                    {
                        if (!isdigit(m_Buffer[index]))
                            validNumber = false;

                        ++index;
                    }

                    if ((*numberPtr) != '\0' && m_Buffer[index] == '\0' && validNumber)
                        return {atoi(numberPtr), true};
                }
            }
        }
    }
    return {NOT_FOUND, false};
}

const char *CommandBuffer::Command() const
{
    for (int i = 0; i < Length(); i++)
        if (m_Buffer[i] != '\0')
            return m_Buffer + i;

    return nullptr;
}

int CommandBuffer::Length() const
{
    return m_CurrentLength;
}

bool CommandBuffer::IsFull() const
{
    return m_CurrentLength == BUFFER_LENGTH;
}

const char *CommandBuffer::C_Ptr() const
{
    return m_Buffer;
}
