#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "debug.h"

struct Integer
{
    int value;
    bool success;
};

struct Float
{
    float value;
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
    Integer IntAt(size_t seekedIndex) const;
    Float FloatAt(size_t seekedIndex) const;
    // when no command is found, returns nullptr
    const char *WordAt(size_t seekedIndex) const;
    const char *Command() const;
    size_t Length() const;
    bool IsFull() const;
    // pointer to the beggining of the buffer
    const char *C_Ptr() const;

    static constexpr char NULL_CHAR = '\0';
    static constexpr char FLOAT_SEPARATOR = '.';
    static constexpr size_t BUFFER_MAX_LENGTH = 100U;
    static constexpr int NOT_FOUND = 0;

private:
    size_t m_CurrentLength = 0;
    // one additional index that is ALWAYS null (to make sure atoi and friends dont mess up)
    char m_Buffer[BUFFER_MAX_LENGTH + 1] = {NULL_CHAR};
};

#endif
