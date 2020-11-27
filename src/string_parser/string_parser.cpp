#include <Arduino.h>
#include "string_parser.hpp"
#include "debug.h"

#if STRING_PARSER_DEBUG

#define LOG_STRING_PARSER(message) LOG(message)
#define LOG_STRING_PARSER_NL(message) LOG_NL(message)

#else

#define LOG_STRING_PARSER(message)
#define LOG_STRING_PARSER_NL(message)

#endif

namespace string_parser
{
    bool string_parser::exec_buffer(const command_buffer *buffer, size_t command_pos)
    {
        if (buffer)
        {
            bool if_executed = false;
            const char *command = buffer->word_at(command_pos);
            // check if there was any command
            if (command)
            {
                auto *event = get_event(command);
                if (event)
                {
                    LOG_STRING_PARSER("Executing ")
                    LOG_STRING_PARSER_NL(event->command)
                    event->fun(buffer);
                    if_executed = true;
                }
            }
            return if_executed;
        }
        return false;
    }

    bool string_parser::add_event(const char *command, event fun, unsigned interval)
    {
        // check if all parameters are ok
        if (command && fun && !is_full())
        {
            auto *exising_event = get_event(command);
            // add only if exent with the same name doesnt't exist
            if (!exising_event)
            {
                // otherwise add it and return true
                m_events[m_current_events].command = command;
                m_events[m_current_events].fun = fun;
                m_events[m_current_events].interval = interval;
                ++m_current_events;
                return true;
            }
        }
        return false;
    }

    void string_parser::exec_intervals()
    {
        for (size_t i = 0; i < m_current_events; i++)
        {
            // first check if interval is active -> 0 means it is inactive
            // if interval is active, check when it was last used
            // if the interval has passed, update last time and execute command
            auto &event = m_events[i];
            if (event.interval != 0 && millis() - event.lastUpdate > event.interval)
            {
                event.fun(nullptr);
                event.lastUpdate = millis();
            }
        }
    }

    bool string_parser::set_interval(const char *command, size_t interval)
    {
        // check if command is valid
        if (command)
        {
            auto *event = get_event(command);
            if (event)
            {
                event->interval = interval;
                return true;
            }
        }
        return false;
    }

    string_parser::event_data *string_parser::get_event(const char *command)
    {
        if (command)
        {
            for (size_t i = 0; i < m_current_events; i++)
                if (strcmp(command, m_events[i].command) == 0)
                    return &(m_events[i]);
        }
        return nullptr;
    }

    bool string_parser::is_full()
    {
        // it should never be bigger, but made that just in case
        return m_current_events >= MAX_EVENTS;
    }
} // namespace string_parser