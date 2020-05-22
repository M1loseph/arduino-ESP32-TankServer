#ifndef __BUFFER_H
#define __BUFFER_H

#include "debug.h"

#define BUFFER_LENGTH 100
#define NOT_FOUND -1

struct Number {
    int number;
    bool success;
};

class CommandBuffer
{
public:
    bool PushBack(char c);
    void Clear();

    //                                        0       1      2      3     ...
    // the commands are expected to be like: "command number number number..."
    // spaces don't matter (must be at least one)
    Number FindNumber(int numberIndex) const;
    // when no command is found, returns nullptr
    const char *Command() const;
    int Length() const;
    bool IsFull() const;
    // pointer to the beggining of the buffer
    const char *C_Ptr() const;

private:
    int m_CurrentLength = 0;
    // one additional index that is ALWAYS null (to make sure atoi and friends dont mess up)
    char m_Buffer[BUFFER_LENGTH + 1] = {'\0'}; 
};

#endif
