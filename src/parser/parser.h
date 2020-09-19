#ifndef __PARSER_H__
#define __PARSER_H__

#include <Arduino.h>
#include "buffer/buffer.h"

class Parser
{
public:
    typedef void (*Event)(const CommandBuffer &);

    // Returns how many characters have benn read
    size_t ReadStream(Stream &stream);

    bool AddEvent(const char *command, Event event, size_t interval = Parser::IDLE_INTERVAL);
    bool SetInterval(const char *command, size_t interval);

    void ExecuteBuffer();
    void ExecuteIntervals();

    CommandBuffer &GetBuff();
    int GetInterval(const char *command);

    bool IsFull();

    static constexpr int TIMEOUT = 20;
    static constexpr int MAX_EVENTS = 20;
    static constexpr int INTERVAL_NOT_FOUND = -1;
    static constexpr size_t IDLE_INTERVAL = 0U;

private:

    typedef struct EventData
    {
        const char *name;
        Event event;
        // used fot the interval functionality
        unsigned long lastUpdate = 0;
        size_t interval;
    } EventData;

    CommandBuffer m_Buffer;
    // arrays of events and info about them
    size_t m_CurrentEvents = 0;
    EventData m_Events[MAX_EVENTS];
};

#endif // __PARSER_H
