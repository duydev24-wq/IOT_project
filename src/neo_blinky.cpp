
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
        if (xSemaphoreTake(xBinarySemaphoreNEOState[STATE_NORMAL], 0) == pdTRUE) {
            // State NORMAL -> blink green once every 2 seconds
            Serial.println("NEO NORMAL");
            strip.setPixelColor(0, strip.Color(0, 255, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else if (xSemaphoreTake(xBinarySemaphoreNEOState[STATE_ATTENTION], 0) == pdTRUE) {
            // State ATTENTION -> blink yellow once every 2 seconds
            Serial.println("NEO ATTENTION");
            strip.setPixelColor(0, strip.Color(255, 255, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else if (xSemaphoreTake(xBinarySemaphoreNEOState[STATE_WARNING], 0) == pdTRUE) {
            // State WARNING -> blink orange once every 2 seconds
            Serial.println("NEO WARNING");
            strip.setPixelColor(0, strip.Color(255, 165, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else if (xSemaphoreTake(xBinarySemaphoreNEOState[STATE_CRITICAL], 0) == pdTRUE) {
            // State CRITICAL -> LED ON continuously red
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