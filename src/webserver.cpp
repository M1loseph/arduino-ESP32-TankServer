#include <ArduinoJson.h>
#include "webserver.hpp"
#include "debug.hpp"
#include "global_queue.hpp"

#if WEB_SERVER_DEBUG

#define LOG_WEBSERVER(message) LOG(message)
#define LOG_WEBSERVER_NL(message) LOG_NL(message)
#define LOG_WEBSERVER_F(...) LOG_F(__VA_ARGS__)
#define LOG_WEBSERVER_PRETTY(...) serializeJsonPretty(__VA_ARGS__); 

#else

#define LOG_WEBSERVER(message)
#define LOG_WEBSERVER_NL(message)
#define LOG_WEBSERVER_F(...)
#define LOG_WEBSERVER_PRETTY(...) 

#endif

AsyncWebServer webserver::web_server(HTTP_PORT);
AsyncWebSocket webserver::web_socket(WEB_SOCKET_ROOT);
DNSServer webserver::dns;

void webserver::init_entire_web()
{
    LOG_NL("[webserver] initing file system...")
    if (SPIFFS.begin())
    {
        LOG_NL("[webserver] initing access points...")
        init_access_point();
        LOG_NL("[webserver] initing web server...")
        init_web_server();
        LOG_NL("[webserver] initing web socket...")
        init_web_socket();
        LOG_NL("[webserver] initing DNS...")
        init_dns();
        LOG_NL("[webserver] starting server...")
        web_server.begin();
        LOG_NL("[webserver] good to go!")
    }
    else
    {
        LOG_NL("[webserver] error occured while initing SPIFFS")
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

    web_server.on("/js/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/main.js", "text/javascript");
    });

    web_server.on("/styles/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/styles/styles.css", "text/css");
    });

    web_server.on("/js/configs.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/configs.js", "text/javascript");
    });

    web_server.on("/js/gamepad_processing.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/gamepad_processing.js", "text/javascript");
    });

    web_server.on("/js/tank_commands.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/tank_commands.js", "text/javascript");
    });

    web_server.on("/js/websocket.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/websocket.js", "text/javascript");
    });

    web_server.on("/images/controller.svg", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/images/controller.svg", "image/svg+xml");
    });

    web_server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/favicon.ico", "image/png");
    });

    web_server.on("/js/jquery-3.5.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/jquery-3.5.1.min.js");
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
                    LOG_WEBSERVER_PRETTY(*json, Serial)
                    LOG_WEBSERVER_NL(' ');
                }

                if(error || !global_queue::queue.push(&json))
                {
                    delete json;
                }
            }
            // else
            // {
            //     //message is comprised of multiple frames or the frame is split into multiple packets
            //     if (frame->index == 0)
            //     {
            //         if (frame->num == 0)
            //             Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (frame->message_opcode == WS_TEXT) ? "text" : "binary");
            //         Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), frame->num, frame->len);
            //     }

            //     Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), frame->num, (frame->message_opcode == WS_TEXT) ? "text" : "binary", frame->index, frame->index + len);

            //     if ((frame->index + len) == frame->len)
            //     {
            //         Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), frame->num, frame->len);
            //         if (frame->final)
            //         {
            //             Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (frame->message_opcode == WS_TEXT) ? "text" : "binary");
            //         }
            //     }
            // }
        }
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        LOG_WEBSERVER_F("ws[%u] disconnect\n", client->id());
        size_t clients = server->getClients().length();
        // if (!clients)
        //     json_parser::stop();
    }
#if WEB_SERVER_DEBUG
    else if (type == WS_EVT_CONNECT)
    {
        LOG_WEBSERVER_F("ws[%u] connect\n", client->id());
        client->ping();
    }
    else if (type == WS_EVT_ERROR)
    {
        LOG_WEBSERVER_F("ws[%u] error(%u): %s\n", client->id(), *((uint16_t *)arg), (char *)data);
    }
    else if (type == WS_EVT_PONG)
    {
        LOG_WEBSERVER_F("ws[%u] pong[%u]: %s\n", client->id(), len, (len) ? (char *)data : "");
    }
#endif // WEB_SERVER_DEBUG
}

void webserver::init_dns()
{
    dns.start(53, "*", WiFi.softAPIP());
}
