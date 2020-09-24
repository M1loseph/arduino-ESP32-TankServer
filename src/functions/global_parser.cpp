#include "global_parser.h"

namespace global_parser
{
    Parser parser;
    xSemaphoreHandle semaphore;

    void init_global_semaphore()
    {
        semaphore = xSemaphoreCreateMutex();
    }

} // namespace global_parser