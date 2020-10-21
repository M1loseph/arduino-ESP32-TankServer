#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <AsyncWebSocket.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>

namespace webserver
{
    extern AsyncWebServer web_server;
    extern AsyncWebSocket web_socket;
    extern DNSServer dns;

    constexpr uint8_t HTTP_PORT = 80;

    extern const char *SSID;
    extern const char *PASSWORD;

    void init_access_point();
    void init_web_server();
    void init_web_socket();
    void init_dns();

    void init_entire_web();

    void handle_web_socket(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

} // namespace webserver
#endif // __WEBSERVER_H__