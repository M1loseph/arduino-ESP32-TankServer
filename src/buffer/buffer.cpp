#include <Arduino.h>
#include "buffer.h"
#include "debug.h"

#if BUFFER_DEBUG

#define LOG_BUFFER(message) LOG(message)
#define LOG_BUFFER_NL(message) LOG_NL(message)

#else

#define LOG_BUFFER(message)
#define LOG_BUFFER_NL(message)

#endif

bool CommandBuffer::push_back(char c)
{
    if (m_current_length < MAX_LENGTH)
    {
        // remove spaces -> change them to NULL_CHAR
        if (c == ' ')
            c = DEFAULT_FILL;
        m_buffer[m_current_length++] = c;
        LOG_BUFFER("New char: ");
        LOG_BUFFER_NL(m_buffer[m_current_length - 1]);
        return true;
    }
    return false;
}

bool CommandBuffer::push_back(const char *string, size_t length)
{
    if (string)
    {
        bool if_added = true;
        length = length ? length : strlen(string);
        // iterate over antire string
        // if char can't be pushed -> break the loop
        // return if last char was added successfully
        for (size_t i = 0; i < length && if_added; i++)
            if_added = push_back(string[i]);

        return if_added;
    }
    return false;
}

void CommandBuffer::clear()
{
    memset(m_buffer, DEFAULT_FILL, MAX_LENGTH);
    m_current_length = 0;
    LOG_BUFFER_NL("Cleared buffer");
}

Value CommandBuffer::int_at(size_t seeked_index) const
{
    const char *number_ptr = word_at(seeked_index);

    if (number_ptr)
    {
        // index of the ptr that has been returned
        size_t index = number_ptr - m_buffer;

        bool at_least_one_digit = false;
        bool valid_number = true;

        // check if number starts with -
        // if so, then it's negative
        // simple increment by one, atoi will handle the rest
        if (*number_ptr == '-')
            index++;

        // check if all chars are digits
        while (index < length() && m_buffer[index] != DEFAULT_FILL && valid_number)
            // as long we read digits -> keep going
            if (isdigit(m_buffer[index]))
            {
                at_least_one_digit = true;
                ++index;
            }
            else
            {
                valid_number = false;
            }

        // debub only
        LOG_BUFFER("Pointer: ");
        LOG_BUFFER_NL(number_ptr);
        LOG_BUFFER("Is valid: ");
        LOG_BUFFER_NL(valid_number);
        LOG_BUFFER("Is the last index null: ");
        LOG_BUFFER_NL(m_buffer[index] == DEFAULT_FILL);

        // number has to have at least one digit
        // it has to end with null (index 100 doesnt count)
        // it has to be numeric
        if (at_least_one_digit && m_buffer[index] == DEFAULT_FILL && valid_number)
        {
            LOG_BUFFER("Value: ");
            LOG_BUFFER_NL(atoi(number_ptr));

            return Value(atoi(number_ptr), true);
        }
    }
    return {NOT_FOUND, false};
}

Value CommandBuffer::float_at(size_t seeked_index) const
{
    const char *number_ptr = word_at(seeked_index);
    if (number_ptr)
    {
        // index of the ptr that has been returned
        size_t index = number_ptr - m_buffer;

        // check if number starts with -
        // if so, then it's negative
        if (*number_ptr == '-')
            index++;

        bool valid_number = true;
        bool at_least_one_digit = false;
        bool found_separator = false;
        // now look for a dot
        // at laest one digit must be found before we get to the dot
        // all characters up to the dot must be a digit
        // we can't go over the buffer length
        while (index < length() && m_buffer[index] != DEFAULT_FILL && valid_number && !found_separator)
            if (isDigit(m_buffer[index]))
            {
                at_least_one_digit = true;
                index++;
            }
            else if (m_buffer[index] == CommandBuffer::FLOAT_SEPARATOR)
            {
                found_separator = true;
                index++;
            }
            else
            {
                // character we didnt want
                valid_number = false;
            }

        if (valid_number && at_least_one_digit && found_separator)
        {
            at_least_one_digit = false;
            //iterate over remaining digits
            // has to be at LEAST one
            while (index < length() && m_buffer[index] != DEFAULT_FILL && valid_number)
                // as long we read digits -> keep going
                if (isdigit(m_buffer[index]))
                {
                    at_least_one_digit = true;
                    ++index;
                }
                else
                {
                    valid_number = false;
                }

            // check if everything is ok
            if (at_least_one_digit && m_buffer[index] == DEFAULT_FILL && valid_number)
                return Value((float)atof(number_ptr), true);
        }
    }
    // in case if failure return error
    return {NOT_FOUND, false};
}

const char *CommandBuffer::word_at(size_t seeked_index) const
{
    size_t i = 0;
    for (size_t word = 0; word < seeked_index && i < length(); word++)
    {
        // jump over NULLs
        while (i < length() && m_buffer[i] == DEFAULT_FILL)
            ++i;

        // jump over normal string
        while (i < length() && m_buffer[i] != DEFAULT_FILL)
            ++i;
    }

    // jump over NULLs
    while (i < length() && m_buffer[i] == DEFAULT_FILL)
        ++i;

    // make sure we are not at the end
    if (m_buffer[i] != DEFAULT_FILL)
    {
        return m_buffer + i;
    }
    return nullptr;
}

const char *CommandBuffer::command() const
{
    return word_at(0U);
}

size_t CommandBuffer::length() const
{
    return m_current_length;
}

bool CommandBuffer::is_full() const
{
    return m_current_length == MAX_LENGTH;
}

const char *CommandBuffer::c_ptr() const
{
    return m_buffer;
}

size_t CommandBuffer::read_stream(Stream &stream)
{
    if (stream.available())
    {
        unsigned long start = millis();
        bool foundEnd = false;
        // when null or \r or \n is found, we end the loop
        while (!foundEnd && !is_full() && millis() - start < TIMEOUT)
        {
            if (stream.available())
            {
                char c = (char)stream.read();
                if (c == '\0' || c == '\r' || c == '\n')
                    foundEnd = true;
                else
                    push_back(c);
            }
        }
        LOG_BUFFER_NL(' ');
        LOG_BUFFER("Read characters: ");
        LOG_BUFFER_NL(length());
        LOG_BUFFER_NL(c_ptr());
        return length();
    }
    return 0;
}
