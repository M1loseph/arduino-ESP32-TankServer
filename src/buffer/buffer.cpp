#include <Arduino.h>
#include "buffer.h"

bool CommandBuffer::PushBack(char c)
{
    if (m_CurrentLength < ST_BUFFER_LENGTH)
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
    memset(m_Buffer, NULL_CHAR, ST_BUFFER_LENGTH);
    m_CurrentLength = 0;
    LOG_NL("Cleared memory");
}

Number CommandBuffer::NumberAt(size_t numberIndex) const
{
    const char *numberPtr = WordAt(numberIndex);

    if (numberPtr)
    {
        // index of the ptr that has been returned
        size_t index = numberPtr - m_Buffer;

        bool validNumber = true;
        bool isNegative = false;
        // check if all char are digits

        // check if number starts with -
        // if so, then it's negative
        if (*numberPtr == '-')
        {
            isNegative = true;
            index++;
            numberPtr++;
        }

        while (index < Length() && m_Buffer[index] != NULL_CHAR && validNumber)
            // as long we read digits -> keep going
            if (!isdigit(m_Buffer[index]))
                validNumber = false;
            else
                ++index;

        // number had to have at least one digit
        // it has to end with null (index 100 doesnt count)
        // it has to be numeric
        if ((*numberPtr) != NULL_CHAR && m_Buffer[index] == NULL_CHAR && validNumber)
        {
            // if it is negative, move one step back
            // rest is handled by atoi
            if (isNegative)
                --numberPtr;
            return {atoi(numberPtr), true};
        }
    }
    return {NOT_FOUND, false};
}

const char *CommandBuffer::WordAt(size_t wordIndex) const
{
    size_t i = 0;
    for (size_t word = 0; word < wordIndex && i < Length(); word++)
    {
        // jump over NULLs
        while (i < Length() && m_Buffer[i] == NULL_CHAR)
            ++i;

        // jump over normal string
        while (i < Length() && m_Buffer[i] != NULL_CHAR)
            ++i;
    }

    // jump over NULLs
    while (i < Length() && m_Buffer[i] == NULL_CHAR)
        ++i;

    // make sure we are not at the end
    if (m_Buffer[i] != NULL_CHAR)
    {
        return m_Buffer + i;
    }

    return nullptr;
}

const char *CommandBuffer::Command() const
{
    return WordAt(0U);
}

size_t CommandBuffer::Length() const
{
    return m_CurrentLength;
}

bool CommandBuffer::IsFull() const
{
    return m_CurrentLength == ST_BUFFER_LENGTH;
}

const char *CommandBuffer::C_Ptr() const
{
    return m_Buffer;
}
