#ifndef __MP3_H__
#define __MP3_H__

#include <Arduino.h>
#include "buffer/buffer.h"

void MP3command(Stream &stream, int8_t command, int8_t data_one, int8_t data_two);

#endif //__MP3_H__