#include "global.h"

#include "led_blinky.h"
#include "neo_blinky.h"
#include "temp_humi_monitor.h"
// #include "mainserver.h"
#include "tinyml.h"
#include "coreiot.h"

// include task
#include "task_check_info.h"
#include "task_toogle_boot.h"
#include "task_wifi.h"
#include "task_webserver.h"
#include "task_core_iot.h"

void setup()
{
    Serial.begin(921600);
    Wire.begin(11, 12);

    WiFi.mode(WIFI_AP_STA); // Set WiFi mode to both AP and STA

    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    if (check_info_File(0))   // 0 = check file, 1 = return file content
    {
        startSTA();           
    }
    else // 1 = start AP mode
    {
        startAP();
    }
    connnectWSV();
    for (int i = 0; i < STATE_SIZE; i++) {
    xBinarySemaphoreLEDState[i] = xSemaphoreCreateBinary();
    xBinarySemaphoreNEOState[i] = xSemaphoreCreateBinary();
    }

    cloudQueue = xQueueCreate(5, sizeof(SensorData_t));
    webserverQueue = xQueueCreate(5, sizeof(SensorData_t));
    tinymlQueue = xQueueCreate(5, sizeof(SensorData_t));

    xTaskCreate(led_blinky, "Task LED Blink", 4096, NULL, 2, NULL);// Create LED Blinky task
    xTaskCreate(neo_blinky, "Task NEO Blink", 2048, NULL, 2, NULL);// Create NEO Blinky task
    xTaskCreate(temp_humi_monitor, "Task TEMP HUMI Monitor", 4096, NULL, 2, NULL);// Create TEMP HUMI Monitor task
    xTaskCreate(CloudTask, "Task Cloud", 4096, NULL, 2, NULL);
    xTaskCreate(WebTask, "Task Webserver", 4096, NULL, 2, NULL);
    xTaskCreate(Task_Toogle_BOOT, "Task Toogle Boot", 4096, NULL, 2, NULL);// Create Toogle Boot task
    xTaskCreate(tiny_ml_task, "Tiny ML Task" ,2048  ,NULL  ,2 , NULL);// Create Tiny ML task
}

void loop()
{
    //  Do everything in task, so we can leave it empty
}