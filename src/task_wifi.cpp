#include "task_wifi.h"

void startAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(String(SSID_AP), String(PASS_AP));
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
}

void startSTA()
{
    if (WIFI_SSID.isEmpty())
    {
        Serial.println("SSID empty -> start AP");
        startAP();
        return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());

    Serial.println("Connecting to WiFi...");

    int retry = 0;
    const int max_retry = 50;   // 5 seconds timeout (50 * 100ms)

    while (WiFi.status() != WL_CONNECTED && retry < max_retry)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        retry++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi Connected");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("WiFi Failed -> Start AP mode");
        WiFi.disconnect(true);
        startAP();
    }
}


bool Wifi_reconnect()
{
    static bool wifiWasConnected = false; 

    if (WiFi.status() == WL_CONNECTED)
    {
        if (!wifiWasConnected)
        {
            Serial.println("WiFi Connected!");
            Serial.println(WiFi.localIP());
            wifiWasConnected = true;
        }
        return true;
    }
    else
    {
        wifiWasConnected = false;
        return false;
    }
}