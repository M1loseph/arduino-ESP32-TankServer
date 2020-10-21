#include "webserver.h"
#include "debug.h"
#include "global_parser.h"
#include "engines.h"

#if WEB_SERVER_DEBUG

#define LOG_WEBSERVER(message) LOG(message)
#define LOG_WEBSERVER_NL(message) LOG_NL(message)
#define LOG_WEBSERVER_F(...) LOG_F(__VA_ARGS__)

#else

#define LOG_ENGINE(message)
#define LOG_ENGINE_NL(message)
#define LOG_WEBSERVER_F(...)

#endif

namespace webserver
{

    // ================
    // VARIABLES
    // ================

    const char *WEB_SOCKET_ROOT = "/ws";
    const char *SSID = "TankWiFi";
    const char *PASSWORD = "eurobeat";

    AsyncWebServer web_server(HTTP_PORT);
    AsyncWebSocket web_socket(WEB_SOCKET_ROOT);
    DNSServer dns;

    void init_access_point()
    {
        WiFi.softAP(SSID, PASSWORD);
    }

    void init_web_server()
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

        web_server.on("/js/jquery-3.5.1.min.js", HTTP_GET, [](AsyncWebServerRequest* request) {
            request->send(SPIFFS, "/js/jquery-3.5.1.min.js");
        });
    }

    void init_web_socket()
    {
        web_socket.onEvent(handle_web_socket);
        web_server.addHandler(&web_socket);
    }

    void handle_web_socket(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
    {
        if (type == WS_EVT_DATA)
        {
            AwsFrameInfo *frame = (AwsFrameInfo *)arg;
            if (frame->opcode == WS_TEXT)
            {
                // 1st case -> entire message was sent in a single frame
                if (frame->final && frame->index == 0 && frame->len == len)
                {
                    CommandBuffer ws_buffer;
                    // we care only about text data
                    for (size_t i = 0; i < len; i++)
                        ws_buffer.push_back((char)data[i]);

                    // wait for semaphore
                    while (xSemaphoreTake(global_parser::semaphore, (TickType_t)10U) != pdTRUE)
                        ;
                    global_parser::parser.exec_buffer(&ws_buffer);
                    xSemaphoreGive(global_parser::semaphore);
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
        else if (type == WS_EVT_DISCONNECT)
        {
            LOG_WEBSERVER_F("ws[%u] disconnect\n", client->id());
            size_t clients = server->getClients().length();
            // if client count is 0 -> stop the tank
            if (!clients)
                engines::stop();
        }
    }

    void init_dns()
    {
        dns.start(53, "*", WiFi.softAPIP());
    }

    void init_entire_web()
    {
        LOG_NL("Initing file system...");
        if (SPIFFS.begin())
        {
            LOG_NL("Initing access points...");
            init_access_point();
            LOG_NL("Initing web server...");
            init_web_server();
            LOG_NL("Initing web socket...");
            init_web_socket();
            LOG_NL("Initing DNS...");
            init_dns();
            LOG_NL("Starting server...");
            web_server.begin();
            LOG_NL("Good to go!");
        }
        else
        {
            LOG_NL("Error occured while initing SPIFFS");
        }
    }
} // namespace webserver