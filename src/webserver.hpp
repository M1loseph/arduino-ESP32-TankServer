#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <AsyncWebSocket.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <DNSServer.h>

class webserver {
public:
    static void init_entire_web();
    static void process_web();

private:
    static void send_or_delete(DynamicJsonDocument *json, const char* data, size_t len);
    static void handle_web_socket(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    static void init_access_point();
    static void init_web_server();
    static void init_web_socket();
    static void init_dns();

    static constexpr const char *WEB_SOCKET_ROOT = "/ws";
    static constexpr const char *SSID = "TankWiFi";
    static constexpr const char *PASSWORD = "eurobeat";
    static constexpr uint8_t HTTP_PORT = 80;

    static AsyncWebServer web_server;
    static AsyncWebSocket web_socket;
    static DNSServer dns;
};

#endif // __WEBSERVER_H__