#ifndef __MP3_H__
#define __MP3_H__

#include <Arduino.h>
#include "buffer/buffer.h"

void MP3command(Stream &stream, int8_t command, int8_t data_fst, int8_t data_snd);

#endif //__MP3_H__