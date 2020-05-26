#include <Arduino.h>
#include "buffer.h"

bool CommandBuffer::PushBack(char c)
{
    if (m_CurrentLength < BUFFER_LENGTH)
    {
        // remove spaces -> change them to NULL_CHAR
        if (c == ' ')
            c = NULL_CHAR;
        m_Buffer[m_CurrentLength++] = c;
        LOG(m_Buffer[m_CurrentLength - 1]);
        return true;
    }
    return false;
}

void CommandBuffer::Clear()
{
    memset(m_Buffer, NULL_CHAR, BUFFER_LENGTH);
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
                while (index < Length() && m_Buffer[index] == NULL_CHAR)
                    ++index;

                // jump over normal string
                while (index < Length() && m_Buffer[index] != NULL_CHAR)
                    ++index;
            }
            else
            {
                // jump over NULLs
                while (index < Length() && m_Buffer[index] == NULL_CHAR)
                    ++index;

                // make sure we are not at the end
                if (m_Buffer[index] != NULL_CHAR)
                {
                    const char *numberPtr = m_Buffer + index;
                    bool validNumber = true;

                    // check if all char are digits
                    while (index < Length() && m_Buffer[index] != NULL_CHAR && validNumber)
                    {
                        if (!isdigit(m_Buffer[index]))
                            validNumber = false;

                        ++index;
                    }

                    // number had to have at least one digit
                    // it has to end with null (index 100 doesnt count)
                    // it has to be numeric
                    if ((*numberPtr) != NULL_CHAR && m_Buffer[index] == NULL_CHAR && validNumber)
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
        if (m_Buffer[i] != NULL_CHAR)
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
