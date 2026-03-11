#include "task_webserver.h"
#include "task_wifi.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

bool webserver_isrunning = false;

void Webserver_sendata(String data)
{
    if (ws.count() > 0)
    {
        ws.textAll(data); // Send to all connected clients
        Serial.println("📤 Data sent via WebSocket: " + data);
    }
    else
    {
        Serial.println("⚠️ No WebSocket clients are currently connected!");
    }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
    }
    else if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;

        if (info->opcode == WS_TEXT)
        {
            String message;
            message += String((char *)data).substring(0, len);
            // parseJson(message, true);
            handleWebSocketMessage(message);
        }
    }
}
void connnectWSV()
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);

    // Serve index
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(LittleFS, "/index.html", "text/html");
    });

    // Serve static files
    server.serveStatic("/", LittleFS, "/");

    server.begin();

    ElegantOTA.begin(&server);

    webserver_isrunning = true;
}

void Webserver_stop()
{
    ws.closeAll();
    server.end();
    webserver_isrunning = false;
    Serial.println("WebServer stopped");
}

void Webserver_reconnect()
{
    if (!webserver_isrunning)
    {
        connnectWSV();
    }
    ElegantOTA.loop();
}

/*Task WebTask used to handle web server communication*/ 
void WebTask(void *pvParameters)
{
    SensorData_t data;
    wifi_mode_t mode = WiFi.getMode();
    
    while (1)
    {
        if (mode == WIFI_MODE_AP)
        {
            if (xQueueReceive(webserverQueue, &data, pdMS_TO_TICKS(100)) == pdTRUE)
            {
                String json = "{\"temperature\":" + String(data.temperature) +
                        ",\"humidity\":" + String(data.humidity) + "}";

                Webserver_sendata(json); // Send data to WebSocket clients
            }

            ElegantOTA.loop(); // Handle OTA updates in AP mode
            ws.cleanupClients(); // Cleanup clients to handle disconnections
            vTaskDelay(pdMS_TO_TICKS(100));  // Delay to avoid busy loop
            }
    }
}
