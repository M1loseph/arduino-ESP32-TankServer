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

unsigned Parser::read_stream(Stream &stream)
{
    if (stream.available())
    {
        unsigned long start = millis();
        bool foundEnd = false;
        // when null or \r or \n is found, we end the loop
        while (!foundEnd && !m_buffer.is_full() && millis() - start < TIMEOUT)
        {
            if (stream.available())
            {
                char c = (char)stream.read();
                if (c == '\0' || c == '\r' || c == '\n')
                    foundEnd = true;
                else
                    m_buffer.push_back(c);
            }
        }
        LOG_PARSER_NL(' ');
        LOG_PARSER("Read characters: ");
        LOG_PARSER_NL(m_buffer.length());
        LOG_PARSER_NL(m_buffer.c_ptr());
        return m_buffer.length();
    }
    return 0;
}

bool Parser::exec_buffer()
{
    bool if_executed = false;
    const char *command = m_buffer.command();
    // check if there was any command
    if (command)
    {
        auto *event = get_event(command);
        if (event)
        {
            LOG_PARSER("Executing ");
            LOG_PARSER_NL(event->command);
            event->fun(m_buffer);
            if_executed = true;
        }
    }
    m_buffer.clear();
    return if_executed;
}

bool Parser::add_event(const char *command, Event fun, unsigned interval)
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

CommandBuffer &Parser::get_buff()
{
    return m_buffer;
}

void Parser::exec_intervals()
{
    for (size_t i = 0; i < m_current_events; i++)
    {
        // first check if interval is active -> 0 means it is inactive
        // if interval is active, check when it was last used
        // if the interval has passed, update last time and execute command
        auto &event = m_events[i];
        if (event.interval != 0 && millis() - event.lastUpdate > event.interval)
        {
            event.fun(m_buffer);
            event.lastUpdate = millis();
        }
    }
}

bool Parser::set_interval(const char *command, size_t interval)
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

Parser::EventData *Parser::get_event(const char *command)
{
    if (command)
    {
        for (size_t i = 0; i < m_current_events; i++)
            if (strcmp(command, m_events[i].command) == 0)
                return &(m_events[i]);
    }
    return nullptr;
}

bool Parser::is_full()
{
    // it should never be bigger, but made that just in case
    return m_current_events >= MAX_EVENTS;
}