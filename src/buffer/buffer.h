#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "debug.h"

struct Number
{
    int value;
    bool success;
};

typedef unsigned int size_t;

class CommandBuffer
{
public:
    bool PushBack(char c);
    void Clear();

    //                                        0       1      2      3     ...
    // the commands are expected to be like: "command number number number..."
    // spaces don't matter (must be at least one)
    Number FindNumber(size_t numberIndex) const;
    // when no command is found, returns nullptr
    const char *Word(size_t wordIndex) const;
    const char *Command() const;
    size_t Length() const;
    bool IsFull() const;
    // pointer to the beggining of the buffer
    const char *C_Ptr() const;

    static constexpr char NULL_CHAR = '\0';
    static constexpr size_t ST_BUFFER_LENGTH = 100U;
    static constexpr int NOT_FOUND = -1;

private:
    size_t m_CurrentLength = 0;
    // one additional index that is ALWAYS null (to make sure atoi and friends dont mess up)
    char m_Buffer[ST_BUFFER_LENGTH + 1] = {NULL_CHAR};
};

#endif
