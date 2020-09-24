// bunch of macros
#ifndef __DEBUG_H__
#define __DEBUG_H__

#if SMART_TANK_DEBUG

#define LOG_NL(messege) Serial.println(messege)
#define LOG(messege) Serial.print(messege)
#define LOG_F(...) Serial.printf(__VA_ARGS__)

#else

#define LOG_NL(messege)
#define LOG(messege)
#define LOG_F(...)

#endif // SMART_TANK_DEBUG

#endif // __DEBUG
