#include <Arduino.h>
#include "buffer/buffer.h"
#include "debug.h"

namespace sd_card
{
    namespace commands
    {
        extern const char *LOG_SD;
    }

    bool init_sd_card();
} // namespace sd_card