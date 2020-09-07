#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "typedefs.h"

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


class CommandBuffer
{
public:
    bool push_back(char c);
    void clear();

    //                                        0       1      2      3     ...
    // the commands are expected to be like: "command number number number..."
    // spaces don't matter (must be at least one)
    Integer int_at(size_t seeked_index) const;
    Float float_at(size_t seeked_index) const;
    // when no command is found, returns nullptr
    const char *word_at(size_t seeked_index) const;
    const char *command() const;
    size_t length() const;
    bool is_full() const;
    // pointer to the beggining of the buffer
    const char *c_ptr() const;

    static constexpr char NULL_CHAR = '\0';
    static constexpr char FLOAT_SEPARATOR = '.';
    static constexpr size_t BUFFER_MAX_LENGTH = 100U;
    static constexpr int NOT_FOUND = 0;

private:
    size_t m_current_length = 0;
    // one additional index that is ALWAYS null (to make sure atoi and friends dont mess up)
    char m_buffer[BUFFER_MAX_LENGTH + 1] = {NULL_CHAR};
};

#endif
