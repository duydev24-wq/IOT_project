#include "temp_humi_monitor.h"
DHT20 dht20;
LiquidCrystal_I2C lcd(33,16,2);


void temp_humi_monitor(void *pvParameters){

    Wire.begin(11, 12);
    Serial.begin(115200);
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
            } else if (data.humidity >= 85 && data.humidity <= 90) {
                humi_state = STATE_WARNING;
            } else if (data.humidity >= 75 && data.humidity < 85) {
                humi_state = STATE_ATTENTION;
            } else {
                humi_state = STATE_NORMAL;
            }
            system_state = max(temp_state, humi_state);
            // Print the results
            
            Serial.print("Humidity: ");
            Serial.print(data.humidity);
            Serial.print("%  Temperature: ");
            Serial.print(data.temperature);
            Serial.println("°C");
            Serial.print("System State: ");
            Serial.println(system_state);
            // Send data to queues
            xQueueSend(cloudQueue, &data, pdMS_TO_TICKS(100));
            xQueueSend(webserverQueue, &data, pdMS_TO_TICKS(100));   
            // Signal other tasks based on state
            xSemaphoreGive(xBinarySemaphoreLEDState[temp_state]);
            xSemaphoreGive(xBinarySemaphoreNEOState[humi_state]);
    }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    
}