// bunch of macros
#ifndef __DEBUG_H__
#define __DEBUG_H__

#if SMART_TANK_DEBUG

#define INIT_LOG Serial.begin(115200);
#define LOG_NL(messege) Serial.println(messege);
#define LOG(messege) Serial.print(messege);
#define LOG_F(...) Serial.printf(__VA_ARGS__);
#define LOG_JSON_PRETTY(json)                         \
    WriteBufferingStream buffered_serial(Serial, 64); \
    serializeJsonPretty(json, buffered_serial);       \
    buffered_serial.flush();                          \
    LOG('\n');

#else

#define INIT_LOG
#define LOG_NL(messege)
#define LOG(messege)
#define LOG_F(...)
#define LOG_JSON_PRETTY(json)

#endif // SMART_TANK_DEBUG

#endif // __DEBUG
