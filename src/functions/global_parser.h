#ifndef __GLOBAL_PARSER_H__
#define __GLOBAL_PARSER_H__

#include <Arduino.h>
#include "parser/parser.h"

extern Parser global_parser;
extern xSemaphoreHandle global_parser_semaphore;

void init_global_semaphore();
 
#endif // __GLOBAL_PARSER_H__