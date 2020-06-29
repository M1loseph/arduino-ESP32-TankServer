#ifndef __MP3_H__
#define __MP3_H__
#include <Arduino.h>
#include "buffer/buffer.h"

void MP3command(Stream &stream, int8_t command, int8_t data_one, int8_t data_two)
{
    int8_t Send_buf[8];
    Send_buf[0] = 0x7e;             //starting byte
    Send_buf[1] = 0xff;             //version
    Send_buf[2] = 0x06;             //the number of bytes of the command without starting byte and ending byte
    Send_buf[3] = command;          //
    Send_buf[4] = 0x00;             //0x00 = no feedback, 0x01 = feedback
    Send_buf[5] = data_one;         //datah
    Send_buf[6] = data_two;         //datal
    Send_buf[7] = 0xef;             //ending byte
    for (uint8_t i = 0; i < 8; i++) //
        stream.write(Send_buf[i]);
}

#endif //__MP3_H__