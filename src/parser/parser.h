#ifndef __PARSER_H__
#define __PARSER_H__

#include "buffer/buffer.h"

class Parser
{
public:
    // Returns how many characters have benn read
    int ReadStream(Stream* stream);
    void ExecuteMessege();
    void AddEvents(const char *command, void (*callbacFun)(const CommandBuffer &buffer));
    CommandBuffer &GetBuff();

    static constexpr int TIMEOUT = 20;
    static constexpr int MAX_EVENTS = 20;

private:
    CommandBuffer m_Buffer;

    int currentEvents = 0;
    void (*m_Functions[MAX_EVENTS])(const CommandBuffer &buffer);
    const char *m_Events[MAX_EVENTS];
};

#endif // __PARSER_H