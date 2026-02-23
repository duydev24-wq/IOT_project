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
        float temperature = dht20.getTemperature();
        // Reading humidity
        float humidity = dht20.getHumidity();
        data.temperature = temperature;
        data.humidity    = humidity;

        //xQueueSend(sensorDataQueue, &data, portMAX_DELAY);

        

        // Check if any reads failed and exit early
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
            //return;
        }

        //Update global variables for temperature and humidity
        // glob_temperature = temperature;
        // glob_humidity = humidity;

        /*Use system state instead of global variables*/
        if (temperature < 20 || temperature > 36) {
            temp_state = STATE_CRITICAL;
        } else if (temperature > 33 && temperature <= 36) {
            temp_state = STATE_WARNING;
        } else if ( temperature >= 29 && temperature <= 33.0) {
            temp_state = STATE_ATTENTION;
        } else {
            temp_state = STATE_NORMAL;
        }

        if (humidity < 40 || humidity > 90) {
            humi_state = STATE_CRITICAL;
        } else if (humidity >= 85 && humidity <= 90) {
            humi_state = STATE_WARNING;
        } else if (humidity >= 75 && humidity < 85) {
            humi_state = STATE_ATTENTION;
        } else {
            humi_state = STATE_NORMAL;
        }
        system_state = max(temp_state, humi_state);
        // Print the results
        
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        Serial.print("System State: ");
        Serial.println(system_state);
        // xSemaphoreGive(xBinarySemaphoreTemp);
        // xSemaphoreGive(xBinarySemaphoreHumi);
        xSemaphoreGive(xBinarySemaphoreLEDState[temp_state]);
        xSemaphoreGive(xBinarySemaphoreNEOState[humi_state]);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    
}