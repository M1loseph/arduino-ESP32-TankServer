#include <ArduinoJson.h>
#include <StreamUtils.h>
#include "webserver.hpp"
#include "debug.hpp"
#include "global_queue.hpp"

#if WEB_SERVER_DEBUG

#define LOG_WEBSERVER(message) LOG(message)
#define LOG_WEBSERVER_NL(message) LOG_WEBSERVER_G(message)
#define LOG_WEBSERVER_F(...) LOG_F(__VA_ARGS__)
#define LOG_WEBSERVER_JSON_PRETTY(json) LOG_JSON_PRETTY(json)

#else

#define LOG_WEBSERVER(message)
#define LOG_WEBSERVER_NL(message)
#define LOG_WEBSERVER_F(...)
#define LOG_WEBSERVER_JSON_PRETTY(json) 

#endif

AsyncWebServer webserver::web_server(HTTP_PORT);
AsyncWebSocket webserver::web_socket(WEB_SOCKET_ROOT);
DNSServer webserver::dns;

void webserver::init_entire_web()
{
    LOG_WEBSERVER_F("[%s] initing file system...\n", SSID)
    if (SPIFFS.begin())
    {
        LOG_WEBSERVER_F("[%s] initing access points...\n", SSID)
        init_access_point();
        LOG_WEBSERVER_F("[%s] initing web server...\n", SSID);
        init_web_server();
        LOG_WEBSERVER_F("[%s] initing web socket...\n", SSID)
        init_web_socket();
        LOG_WEBSERVER_F("[%s] initing DNS...\n", SSID)
        init_dns();
        LOG_WEBSERVER_F("[%s] starting server...\n", SSID)
        web_server.begin();
        LOG_WEBSERVER_F("[%s] good to go!\n", SSID)
    }
    else
    {
        LOG_WEBSERVER_F("[%s] error occured while initing SPIFFS\n", SSID)
    }
}

void webserver::process_web()
{
    dns.processNextRequest();
    web_socket.cleanupClients();
}

void webserver::init_access_point()
{
    WiFi.softAP(SSID, PASSWORD);
}

void webserver::init_web_server()
{
    web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    web_server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    web_server.on("/js/index.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/index.js", "text/javascript");
    });

    web_server.on("/styles/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/styles/styles.css", "text/css");
    });

    web_server.on("/styles/mobile.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/styles/mobile.css", "text/css");
    });

    web_server.on("/js/configs.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/configs.js", "text/javascript");
    });

    web_server.on("/js/gamepad_processing.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/gamepad_processing.js", "text/javascript");
    });

    web_server.on("/js/websocket.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/websocket.js", "text/javascript");
    });

    web_server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico", "image/png");
    });
}

void webserver::init_web_socket()
{
    web_socket.onEvent(handle_web_socket);
    web_server.addHandler(&web_socket);
}

void webserver::handle_web_socket(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *frame = (AwsFrameInfo *)arg;
        if (frame->opcode == WS_TEXT)
        {
            // 1st case -> entire message was sent in a single frame
            if (frame->final && frame->index == 0 && frame->len == len)
            {
                DynamicJsonDocument* json = new DynamicJsonDocument(256);
                auto error = deserializeJson(*json, (const char*) data, len);
                if(!error)
                {
                    LOG_WEBSERVER_JSON_PRETTY(*json)
                }

                if(error || !global_queue::queue.push(&json))
                {
                    delete json;
                }
            }
        }
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        LOG_WEBSERVER_F("[%s] ws[%u] disconnect\n", SSID, client->id());
        size_t clients = server->getClients().length();
        if (!clients)
        {
            DynamicJsonDocument* json = new DynamicJsonDocument(256);
            (*json)["controller"] = "engines";
            (*json)["command"] = "STOP";
            (*json)["engines"] = "both";
            if(!global_queue::queue.push(&json))
            {
                delete json;
            }
        }
    }
#if WEB_SERVER_DEBUG
    else if (type == WS_EVT_CONNECT)
    {
        LOG_WEBSERVER_F("[%s] ws[%u] connect\n", SSID, client->id());
        client->ping();
    }
    else if (type == WS_EVT_ERROR)
    {
        LOG_WEBSERVER_F("[%s] ws[%u] error(%u): %s\n", SSID, client->id(), *((uint16_t *)arg), (char *)data);
    }
    else if (type == WS_EVT_PONG)
    {
        LOG_WEBSERVER_F("[%s] ws[%u] pong[%u]: %s\n", SSID, client->id(), len, (len) ? (char *)data : "");
    }
#endif // WEB_SERVER_DEBUG
}

void webserver::init_dns()
{
    dns.start(53, "*", WiFi.softAPIP());
}

void webserver::send_ws(const DynamicJsonDocument &json)
{
    String buffer;
    serializeJson(json, buffer);
    LOG_WEBSERVER_F("[%s] string size: %d\n", SSID, buffer.length());
    web_socket.textAll(buffer);
}