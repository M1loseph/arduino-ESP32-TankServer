#ifndef __GLOBAL_PARSER_H__
#define __GLOBAL_PARSER_H__

#include <Arduino.h>
#include "parser/parser.h"

namespace global_parser
{

    extern Parser parser;
    extern xSemaphoreHandle semaphore;

    void init_global_semaphore();

} // namespace global_parser

#endif // __GLOBAL_PARSER_H__