#include <Arduino.h>
#include "command_buffer.hpp"
#include "debug.hpp"

#if BUFFER_DEBUG

#define LOG_BUFFER(message) LOG(message)
#define LOG_BUFFER_NL(message) LOG_NL(message)
#define LOG_BUFFER_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_BUFFER(message)
#define LOG_BUFFER_NL(message)
#define LOG_BUFFER_F(...)

#endif

namespace string_parser
{
    bool command_buffer::push_back(char c)
    {
        if (m_current_length < MAX_LENGTH)
        {
            // remove spaces -> change them to NULL_CHAR
            if (c == ' ')
                c = DEFAULT_FILL;
            m_buffer[m_current_length++] = c;
            LOG_BUFFER_F("[string_parser] New char: %c\n", m_buffer[m_current_length - 1])
            return true;
        }
        return false;
    }

    bool command_buffer::push_back(const char *string, uint32_t length)
    {
        if (string)
        {
            bool if_added = true;
            length = length ? length : strlen(string);
            // iterate over antire string
            // if char can't be pushed -> break the loop
            // return if last char was added successfully
            for (uint32_t i = 0; i < length && if_added; i++)
                if_added = push_back(string[i]);

            return if_added;
        }
        return false;
    }

    void command_buffer::clear()
    {
        memset(m_buffer, DEFAULT_FILL, MAX_LENGTH);
        m_current_length = 0;
        LOG_BUFFER_NL("[string_parser] cleared buffer")
    }

    value command_buffer::int_at(uint32_t seeked_index) const
    {
        const char *number_ptr = word_at(seeked_index);

        if (number_ptr)
        {
            // index of the ptr that has been returned
            uint32_t index = number_ptr - m_buffer;

            bool at_least_one_digit = false;
            bool valid_number = true;

            // check if number starts with -
            // if so, then it's negative
            // simple increment by one, atoi will handle the rest
            if (*number_ptr == '-')
                index++;

            // check if all chars are digits
            while (index < length() && m_buffer[index] != DEFAULT_FILL && valid_number)
            {
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
            }

            // number has to have at least one digit
            // it has to end with null (index 100 doesnt count)
            // it has to be numeric
            if (at_least_one_digit && m_buffer[index] == DEFAULT_FILL && valid_number)
            {
                int seeked_number = atoi(number_ptr);
                LOG_BUFFER_F("[string_parser] found %d at index %d\n", seeked_number, seeked_index);

                return value(seeked_number, true);
            }
        }
        LOG_BUFFER_F("[string_parser] didnt find int at index %d\n", seeked_index)
        return {NOT_FOUND, false};
    }

    value command_buffer::float_at(uint32_t seeked_index) const
    {
        const char *number_ptr = word_at(seeked_index);
        if (number_ptr)
        {
            // index of the ptr that has been returned
            uint32_t index = number_ptr - m_buffer;

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
                else if (m_buffer[index] == command_buffer::FLOAT_SEPARATOR)
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
                {
                    float seeked_float = static_cast<float>(atof(number_ptr));
                    LOG_BUFFER_F("[string_parser] found %f at index %d\n", seeked_float, seeked_index)
                    return value(seeked_float, true);
                }
            }
        }
        LOG_BUFFER_F("[string_parser] didn't find float at index %d\n", seeked_index)
        return {NOT_FOUND, false};
    }

    const char *command_buffer::word_at(uint32_t seeked_index) const
    {
        uint32_t i = 0;
        for (uint32_t word = 0; word < seeked_index && i < length(); word++)
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

    const char *command_buffer::command() const
    {
        return word_at(0U);
    }

    uint32_t command_buffer::length() const
    {
        return m_current_length;
    }

    bool command_buffer::is_full() const
    {
        return m_current_length == MAX_LENGTH;
    }

    const char *command_buffer::c_ptr() const
    {
        return m_buffer;
    }

    uint32_t command_buffer::read_stream(Stream &stream)
    {
        if (stream.available())
        {
            unsigned long start = millis();
            bool found_end = false;
            // when null or \r or \n is found, we end the loop
            while (!found_end && !is_full() && millis() - start < TIMEOUT)
            {
                if (stream.available())
                {
                    char c = static_cast<char>(stream.read());
                    if (c == '\0' || c == '\r' || c == '\n')
                    {
                        found_end = true;
                    }
                    else
                    {
                        push_back(c);
                    }
                }
            }
            LOG_BUFFER_F("Read characters: %d", length())
            LOG_BUFFER_F("Read string: %s", c_ptr())
            return length();
        }
        return 0;
    }
} // namespace string_parser