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
    // return true if every char in the string could fit into buffer
    // false if at least one could not fit
    bool push_back(const char* string, size_t length = 0);
    void clear();
    //                               0       1      2      3     
    // expected format of commands: "command number number number..."
    // number of spaces doesn't matter (but must be at least one)
    Integer int_at(size_t seeked_index) const;
    Float float_at(size_t seeked_index) const;
    // when no command is found, returns nullptr
    const char *word_at(size_t seeked_index) const;
    const char *command() const;
    size_t length() const;
    bool is_full() const;
    // pointer to the beggining of the buffer
    const char *c_ptr() const;

    static constexpr char DEFAULT_FILL = '\0';
    static constexpr char FLOAT_SEPARATOR = '.';
    static constexpr size_t MAX_LENGTH = 100U;
    static constexpr int NOT_FOUND = 0;

private:
    size_t m_current_length = 0;
    // one additional index that is ALWAYS null (to make sure atoi and friends dont mess up)
    char m_buffer[MAX_LENGTH + 1] = {DEFAULT_FILL};
};

#endif
