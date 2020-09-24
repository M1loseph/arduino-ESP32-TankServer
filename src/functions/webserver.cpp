#include "webserver.h"
#include "debug.h"
#include "global_parser.h"

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
    AsyncWebServer web_server(HTTP_PORT);
    AsyncWebSocket web_socket(WEB_SOCKET_ROOT);
    DNSServer dns;

    const char *WEB_SOCKET_ROOT = "/ws";
    const char *SSID = "TankWiFi";
    const char *PASSWORD = "eurobeat";

    void init_access_point()
    {
        WiFi.softAP(SSID, PASSWORD);
    }

    void init_web_server()
    {
        web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/index.html");
        });

        web_server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/index.html");
        });

        web_server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/main.js");
        });

        web_server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/styles.css");
        });

        web_server.on("/configs.js", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/configs.js");
        });

        web_server.on("/gamepad_processing.js", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/gamepad_processing.js");
        });

        web_server.on("/tank_commands.js", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(SPIFFS, "/tank_commands.js");
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
                    // wait untill we get the resourse
                    while (xSemaphoreTake(global_parser::semaphore, (TickType_t)100) != pdTRUE)
                        ;
                    // we care only about text data
                    for (size_t i = 0; i < len; i++)
                        global_parser::parser.get_buff().push_back((char)data[i]);

                    global_parser::parser.exec_buffer();
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
        else if (type == WS_EVT_DISCONNECT)
        {
            LOG_WEBSERVER_F("ws[%u] disconnect\n", client->id());
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

    void init_dns()
    {
        // TODO => doesn't always work
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