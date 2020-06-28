#ifndef __COMMON_FUN_H__
#define __COMMON_FUN_H__

#include <Arduino.h>
#include "buffer/buffer.h"

void MoveOverSerial(const CommandBuffer &b)
{
  const char *commandToSend = b.WordAt(1);
  if (commandToSend)
  {
    // get index of message we want to send
    size_t index = (size_t)(commandToSend - b.C_Ptr());
    // send entire messege
    for (size_t i = index; i < b.Length(); i++)
    {
      char toSend = b.C_Ptr()[i];
      if (toSend == b.NULL_CHAR)
        Serial.write(' ');
      else
        Serial.write(toSend);
    }
    // to stop communation
    Serial.write('\n');
  }
}
#endif //__COMMON_FUN_H__
