#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// extern float glob_temperature;
// extern float glob_humidity;
//Task3: Use system state insted of global variables
#define STATE_NORMAL 0
#define STATE_ATTENTION 1
#define STATE_WARNING 2
#define STATE_CRITICAL 3
#define STATE_SIZE 4
extern String WIFI_SSID;
extern String WIFI_PASS;
extern String CORE_IOT_TOKEN;
extern String CORE_IOT_SERVER;
extern String CORE_IOT_PORT;
extern String wifi_ssid;
extern String wifi_password;
extern String ssid;
extern String password;
extern boolean isWifiConnected;
extern SemaphoreHandle_t xBinarySemaphoreInternet;
extern QueueHandle_t sensorDataQueue;
// extern SemaphoreHandle_t xBinarySemaphoreTemp;
// extern SemaphoreHandle_t xBinarySemaphoreHumi;
extern SemaphoreHandle_t xBinarySemaphoreNEOState[STATE_SIZE];
extern SemaphoreHandle_t xBinarySemaphoreLEDState[STATE_SIZE];
typedef struct {
    float temperature;
    float humidity;
} SensorData_t;
#endif