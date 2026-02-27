#include "global.h"

#include "led_blinky.h"
#include "neo_blinky.h"
#include "temp_humi_monitor.h"
// #include "mainserver.h"
// #include "tinyml.h"
#include "coreiot.h"

// include task
#include "task_check_info.h"
#include "task_toogle_boot.h"
#include "task_wifi.h"
#include "task_webserver.h"
#include "task_core_iot.h"

void setup()
{
  Serial.begin(115200);
  //check_info_File(0);
  WiFi.mode(WIFI_AP_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  if (check_info_File(0))   // 0 = load file
  {
      startSTA();           // ✅ gọi ở đây
  }


  for (int i = 0; i < STATE_SIZE; i++) {
      xBinarySemaphoreLEDState[i] = xSemaphoreCreateBinary();
      xBinarySemaphoreNEOState[i] = xSemaphoreCreateBinary();
  }


  xTaskCreate(led_blinky, "Task LED Blink", 4096, NULL, 2, NULL);
  xTaskCreate(neo_blinky, "Task NEO Blink", 2048, NULL, 2, NULL);
  xTaskCreate(temp_humi_monitor, "Task TEMP HUMI Monitor", 4096, NULL, 2, NULL);

  // xTaskCreate(main_server_task, "Task Main Server" ,8192  ,NULL  ,2 , NULL);
  // xTaskCreate( tiny_ml_task, "Tiny ML Task" ,2048  ,NULL  ,2 , NULL);
  //xTaskCreate(coreiot_task, "CoreIOT Task" ,4096  ,NULL  ,2 , NULL);
  // xTaskCreate(Task_Toogle_BOOT, "Task_Toogle_BOOT", 4096, NULL, 2, NULL);

      // xTaskCreate(led_blinky,"LED Task",2048, NULL,1,NULL);

    //xTaskCreate(temp_humi_monitor,"TempHumi Task",4096,NULL,1,NULL);
}
void loop()
{
    if (check_info_File(1))
    {
        Wifi_reconnect();          // luôn kiểm tra

        if (WiFi.status() == WL_CONNECTED)
        {
            CORE_IOT_reconnect();  // chỉ reconnect khi có WiFi
        }

        tb.loop();                 // ⚠️ luôn chạy, không đặt trong if

        static unsigned long lastSend = 0;

        if (millis() - lastSend > 5000)
        {
            lastSend = millis();

            if (WiFi.status() == WL_CONNECTED && tb.connected())
            {
                CORE_IOT_sendata("telemetry", "temperature", String(glob_temperature));
                CORE_IOT_sendata("telemetry", "humidity", String(glob_humidity));
                String json = "{\"temperature\":" + String(glob_temperature) + 
                  ",\"humidity\":" + String(glob_humidity) + "}";
                Webserver_sendata(json);
            }
        }
    }
    ws.cleanupClients();   
    Webserver_reconnect();
    delay(1);
}