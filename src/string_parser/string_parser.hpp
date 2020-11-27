#ifndef __PARSER_H__
#define __PARSER_H__

#include <Arduino.h>
#include "command_buffer/command_buffer.hpp"

namespace string_parser
{
    class string_parser
    {
    public:
        typedef void (*event)(const command_buffer *);
        typedef struct
        {
            const char *command;
            event fun;
            // used fot the interval functionality
            unsigned long lastUpdate = 0;
            size_t interval;
        } event_data;

        bool add_event(const char *command, event function, size_t interval = string_parser::IDLE_INTERVAL);
        bool set_interval(const char *command, size_t interval);

        bool exec_buffer(const command_buffer *buffer, size_t command_pos = 0);
        void exec_intervals();

        event_data *get_event(const char *command);

        bool is_full();

        static constexpr size_t MAX_EVENTS = 100U;
        static constexpr size_t IDLE_INTERVAL = 0U;

    private:
        // arrays of events and info about them
        size_t m_current_events = 0;
        event_data m_events[MAX_EVENTS];
    };
} // namespace string_parser

#endif // __PARSER_H
