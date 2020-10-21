#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <Arduino.h>

// value extreacted from buffer -> contains error code
struct Value
{
    Value(int i, bool success) : i(i),
                                 success(success)
    {
    }

    Value(float f, bool success) : f(f),
                                   success(success)
    {
    }

    union
    {
        const float f;
        const int i;
    };
    const bool success;
};

class CommandBuffer
{
public:
    bool push_back(char c);
    // return true if every char in the string could fit into buffer
    // false if at least one could not fit
    bool push_back(const char *string, uint32_t length = 0);
    void clear();
    //                               0       1      2      3
    // expected format of commands: "command number number number..."
    // number of spaces doesn't matter (but must be at least one)
    Value int_at(uint32_t seeked_index) const;
    Value float_at(uint32_t seeked_index) const;
    // when no command is found, returns nullptr
    const char *word_at(uint32_t seeked_index) const;
    const char *command() const;
    uint32_t length() const;
    bool is_full() const;
    // pointer to the beggining of the buffer
    const char *c_ptr() const;
    uint32_t read_stream(Stream &stream);

    static constexpr char DEFAULT_FILL = '\0';
    static constexpr char FLOAT_SEPARATOR = '.';
    static constexpr uint32_t MAX_LENGTH = 200U;
    static constexpr int NOT_FOUND = 0;
    static constexpr uint32_t TIMEOUT = 20;

private:
    uint32_t m_current_length = 0;
    // one additional index that is ALWAYS null (to make sure atoi and friends dont mess up)
    char m_buffer[MAX_LENGTH + 1] = {DEFAULT_FILL};
};

#endif
