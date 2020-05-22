// bunch of macros
#ifndef __DEBUG
#define __DEBUG

// 0 - no debug
// 1 - debug messages over serial com
#define DEBUG_LEVEL 0

#if DEBUG_LEVEL

#define LOG_NL(messege) Serial.println(messege)
#define LOG(messege) Serial.print(messege)

#else

#define LOG_NL(messege)
#define LOG(messege)

#endif // DEBUG_LEVEL

#endif // __DEBUG
