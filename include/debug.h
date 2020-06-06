// bunch of macros
#ifndef __DEBUG_H__
#define __DEBUG_H__

#if SMART_TANK_DEBUG

#define LOG_NL(messege) Serial.println(messege)
#define LOG(messege) Serial.print(messege)

#else

#define LOG_NL(messege)
#define LOG(messege)

#endif // SMART_TANK_DEBUG

#endif // __DEBUG
