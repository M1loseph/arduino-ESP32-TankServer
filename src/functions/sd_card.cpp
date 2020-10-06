#include "sd_card.h"
#include <SD.h>

#if SD_DEBUG

#define LOG_SD(message) LOG(message)
#define LOG_SD_NL(message) LOG_NL(message)
#define LOG_SD_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_SD(message)
#define LOG_SD_NL(message)
#define LOG_SD_F(...)

#endif // SD_DEBUG

namespace sd_card
{

    constexpr uint8_t CHIP_SELECT = 5;
    // file for parser commands
    const char *script_file = "/script.txt";
    // this one is for logging only
    const char *log_file = "/logs.txt";

    File file;

    bool init_sd_card()
    {
        return SD.begin(CHIP_SELECT);
    }

} // namespace sd_card