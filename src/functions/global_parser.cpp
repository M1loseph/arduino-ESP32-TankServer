#include "global_parser.h"

Parser global_parser;
xSemaphoreHandle global_parser_semaphore;

void init_global_semaphore()
{
    global_parser_semaphore = xSemaphoreCreateMutex();
}