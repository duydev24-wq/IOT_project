
#include "neo_blinky.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// Declare the semaphore handle externally or define it here if not already defined elsewhere



void neo_blinky(void *pvParameters){

    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    // Set all pixels to off to start
    strip.clear();
    strip.show();

    while(1) {
        // if (xSemaphoreTake(xBinarySemaphoreHumi, portMAX_DELAY) == pdTRUE) {
        //     float humi = glob_humidity;
        //     if (humi < 0) {
        //         // Sensor not ready
        //         strip.clear();
        //         strip.show();
        //         continue;
        //     }
        //     if (humi < 40.0) {
        //         // Low humidity -> Blue
        //         strip.setPixelColor(0, strip.Color(0, 0, 255));
        //     }
        //     else if (humi < 70.0) {
        //         // Medium humidity -> Green
        //         strip.setPixelColor(0, strip.Color(0, 255, 0));
        //     }
        //     else {
        //         // High humidity -> Red
        //         strip.setPixelColor(0, strip.Color(255, 0, 0));
        //     }
        //     strip.show(); // Update the strip
        // }
        if (xSemaphoreTake(xBinarySemaphoreNEOState[STATE_NORMAL], 0) == pdTRUE) {
            Serial.println("NEO NORMAL");
            strip.setPixelColor(0, strip.Color(0, 255, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else if (xSemaphoreTake(xBinarySemaphoreNEOState[STATE_ATTENTION], 0) == pdTRUE) {
            Serial.println("NEO ATTENTION");
            strip.setPixelColor(0, strip.Color(255, 255, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else if (xSemaphoreTake(xBinarySemaphoreNEOState[STATE_WARNING], 0) == pdTRUE) {
            Serial.println("NEO WARNING");
            strip.setPixelColor(0, strip.Color(255, 165, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else if (xSemaphoreTake(xBinarySemaphoreNEOState[STATE_CRITICAL], 0) == pdTRUE) {
            Serial.println("NEO CRITICAL");
            strip.setPixelColor(0, strip.Color(255, 0, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else{
            // If no state is set, turn off the LED
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
        }
        // Wait for another 500 milliseconds
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}