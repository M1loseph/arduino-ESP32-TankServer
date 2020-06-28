#ifndef __PARSER_H__
#define __PARSER_H__

#include "buffer/buffer.h"

class Parser
{
public:
    // Returns how many characters have benn read
    int ReadStream(Stream *stream);

    void AddEvents(const char *command, void (*callbacFun)(const CommandBuffer &buffer));
    bool SetInterval(const char *command, size_t interval);

    void ExecuteMessege();
    void ExecuteInterval();

    CommandBuffer &GetBuff();
    int GetInterval(const char* command);

    static constexpr int TIMEOUT = 20;
    static constexpr int MAX_EVENTS = 20;
    static constexpr int INTERVAL_NOT_FOUND = -1; 

private:
    static constexpr int IDLE_INTERVAL = 0;

    struct IntervalInfo
    {
        unsigned long lastUpdate = 0;
        size_t interval = Parser::IDLE_INTERVAL;
    };

    CommandBuffer m_Buffer;
    size_t m_CurrentEvents = 0;
    void (*m_Functions[MAX_EVENTS])(const CommandBuffer &buffer);
    const char *m_Events[MAX_EVENTS];
    IntervalInfo m_Intervals[MAX_EVENTS];
};

#endif // __PARSER_H
