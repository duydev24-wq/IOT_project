#include "temp_humi_monitor.h"
DHT20 dht20;
LiquidCrystal_I2C lcd(33,16,2);


void display_system_state(uint8_t system_state){
    if (system_state == 0){
        Serial.println("System is in NORMAL state.");
    }
    else if (system_state == 1)
    {
        Serial.println("System is in ATTENTION state.");
    }
    else if (system_state == 2)
    {
        Serial.println("System is in WARNING state.");
    }
    else if (system_state == 3)
    {
        Serial.println("System is in CRITICAL state.");
    }
}

void temp_humi_monitor(void *pvParameters){
    char line[50];
    uint8_t anomaly = 0;

    dht20.begin();
    uint8_t temp_state = STATE_NORMAL;
    uint8_t humi_state = STATE_NORMAL;
    uint8_t system_state = STATE_NORMAL;
    SensorData_t data;

    while (1){
        /* code */
        
        dht20.read();
        // Reading temperature in Celsius
        data.temperature = dht20.getTemperature();
        // Reading humidity
        data.humidity = dht20.getHumidity();

        // Check if any reads failed and exit early
        if (isnan(data.temperature) || isnan(data.humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            data.temperature = data.humidity =  -1;
            return;
        }
        else {
            Serial.println("DHT20 sensor read successfully!");
            /*Use system state instead of global variables*/
            if (data.temperature < 20 || data.temperature > 36) {
                temp_state = STATE_CRITICAL;
            } else if (data.temperature > 33 && data.temperature <= 36) {
                temp_state = STATE_WARNING;
            } else if ( data.temperature >= 29 && data.temperature <= 33.0) {
                temp_state = STATE_ATTENTION;
            } else {
                temp_state = STATE_NORMAL;
            }

            if (data.humidity < 40 || data.humidity > 90) {
                humi_state = STATE_CRITICAL;
            } else if (data.humidity >= 80 && data.humidity <= 90) {
                humi_state = STATE_WARNING;
            } else if (data.humidity >= 70 && data.humidity < 80) {
                humi_state = STATE_ATTENTION;
            } else {
                humi_state = STATE_NORMAL;
            }
            system_state = max(temp_state, humi_state);
            if (system_state == STATE_WARNING || system_state == STATE_CRITICAL){
                anomaly = 1;
            }else {
                anomaly = 0;
            }
            snprintf(line, sizeof(line), "%.2f,%.2f,%d",
            data.temperature,
            data.humidity,
            anomaly);

            Serial.println(line);
            //Serial.println("°C");
            display_system_state(system_state);
            // Send data to queues
            xQueueSend(webcloudQueue, &data, pdMS_TO_TICKS(100));
            xQueueSend(tinymlQueue, &data, pdMS_TO_TICKS(100));
            // Signal other tasks based on state
            xSemaphoreGive(xBinarySemaphoreLEDState[temp_state]);
            xSemaphoreGive(xBinarySemaphoreNEOState[humi_state]);
    }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}
