#include <Arduino.h>
#include "buffer.h"

bool CommandBuffer::PushBack(char c)
{
    if (m_CurrentLength < BUFFER_MAX_LENGTH)
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
    memset(m_Buffer, NULL_CHAR, BUFFER_MAX_LENGTH);
    m_CurrentLength = 0;
    LOG_NL("Cleared memory");
}

Integer CommandBuffer::IntAt(size_t seekedIndex) const
{
    const char *numberPtr = WordAt(seekedIndex);

    if (numberPtr)
    {
        // index of the ptr that has been returned
        size_t index = numberPtr - m_Buffer;

        bool atLeastOneDigit = false;
        bool validNumber = true;

        // check if number starts with -
        // if so, then it's negative
        // simple increment by one, atoi will handle the rest
        if (*numberPtr == '-')
            index++;

        // check if all chars are digits
        while (index < Length() && m_Buffer[index] != NULL_CHAR && validNumber)
            // as long we read digits -> keep going
            if (isdigit(m_Buffer[index]))
            {
                atLeastOneDigit = true;
                ++index;
            }
            else
            {
                validNumber = false;
            }
        
        // debub only
        LOG("Pointer: ");
        LOG_NL(numberPtr);
        LOG("Valid: ");
        LOG_NL(validNumber);
        LOG("Is last index null: ");
        LOG_NL(m_Buffer[index] == NULL_CHAR);

        // number has to have at least one digit
        // it has to end with null (index 100 doesnt count)
        // it has to be numeric
        if (atLeastOneDigit && m_Buffer[index] == NULL_CHAR && validNumber)
            return {atoi(numberPtr), true};
    }
    return {NOT_FOUND, false};
}

Float CommandBuffer::FloatAt(size_t seekedIndex) const
{
    const char *numberPtr = WordAt(seekedIndex);
    if (numberPtr)
    {
        // index of the ptr that has been returned
        size_t index = numberPtr - m_Buffer;

        // check if number starts with -
        // if so, then it's negative
        if (*numberPtr == '-')
            index++;

        bool validNumber = true;
        bool atLeastOneDigit = false;
        bool foundSeparator = false;
        // now look for a dot
        // at laest one digit must be found before we get to the dot
        // all characters up to the dot must be a digit
        // we can't go over the buffer length
        while (index < Length() && m_Buffer[index] != NULL_CHAR && validNumber && !foundSeparator)
            if (isDigit(m_Buffer[index]))
            {
                atLeastOneDigit = true;
                index++;
            }
            else if (m_Buffer[index] == CommandBuffer::FLOAT_SEPARATOR)
            {
                foundSeparator = true;
                index++;
            }
            else
            {
                // character we didnt want
                validNumber = false;
            }

        if (validNumber && atLeastOneDigit && foundSeparator)
        {
            atLeastOneDigit = false;
            //iterate over remaining digits
            // has to be at LEAST one
            while (index < Length() && m_Buffer[index] != NULL_CHAR && validNumber)
                // as long we read digits -> keep going
                if (isdigit(m_Buffer[index]))
                {
                    atLeastOneDigit = true;
                    ++index;
                }
                else
                {
                    validNumber = false;
                }

            // check if everything is ok
            if (atLeastOneDigit && m_Buffer[index] == NULL_CHAR && validNumber)
                return {(float)atof(numberPtr), true};
        }
    }
    // in case if failure return error
    return {NOT_FOUND, false};
}

const char *CommandBuffer::WordAt(size_t seekedIndex) const
{
    size_t i = 0;
    for (size_t word = 0; word < seekedIndex && i < Length(); word++)
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
    return m_CurrentLength == BUFFER_MAX_LENGTH;
}

const char *CommandBuffer::C_Ptr() const
{
    return m_Buffer;
}
