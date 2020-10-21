#ifndef __PARSER_H__
#define __PARSER_H__

#include <Arduino.h>
#include "buffer/buffer.h"

class Parser
{
public:
    typedef void (*Event)(const CommandBuffer *);
    typedef struct
    {
        const char *command;
        Event fun;
        // used fot the interval functionality
        unsigned long lastUpdate = 0;
        size_t interval;
    } EventData;

    bool add_event(const char *command, Event function, size_t interval = Parser::IDLE_INTERVAL);
    bool set_interval(const char *command, size_t interval);

    bool exec_buffer(const CommandBuffer *buffer, size_t command_pos = 0);
    void exec_intervals();

    EventData *get_event(const char *command);

    bool is_full();

    static constexpr size_t MAX_EVENTS = 100U;
    static constexpr size_t IDLE_INTERVAL = 0U;

private:
    // arrays of events and info about them
    size_t m_current_events = 0;
    EventData m_events[MAX_EVENTS];
};

#endif // __PARSER_H
