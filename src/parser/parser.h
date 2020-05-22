#ifndef __PARSER_H
#define __PARSER_H

#include "buffer/buffer.h"

#define TIMEOUT 20
#define MAX_EVENTS 20

using Buffer = CommandBuffer;

class Parser
{
public:
    // Returns how many characters have benn read
    int ReadToBuffer();
    void ExecuteMessege();
    void AddEvents(const char *command, void (*callbacFun)(const CommandBuffer &buffer));
    Buffer &GetBuff();

private:
    CommandBuffer m_Buffer;

    int currentEvents = 0;
    void (*m_Functions[MAX_EVENTS])(const CommandBuffer &buffer);
    const char *m_Events[MAX_EVENTS];
};

#endif // __PARSER_H