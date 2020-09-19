#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <AsyncWebSocket.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include "typedefs.h"

extern AsyncWebServer web_server;
extern AsyncWebSocket web_socket;
extern DNSServer dns;

constexpr uchar HTTP_PORT = 80;

extern const char *WEB_SOCKET_ROOT;
extern const char *SSID;
extern const char *PASSWORD;

void init_access_point();
void init_web_server();
void init_web_socket();
void init_dns();

void init_entire_web();

void handle_web_socket(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

#endif // __WEBSERVER_H__