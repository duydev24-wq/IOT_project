#include "led_blinky.h"

void led_blinky(void *pvParameters) {
    pinMode(LED_GPIO, OUTPUT);
    Serial.println("LED Blinky Task Started");
    while (1) {
        if (xSemaphoreTake(xBinarySemaphoreLEDState[STATE_NORMAL], 0) == pdTRUE) {
            // State NORMAL -> blink led once every 2 seconds
            Serial.println("LED NORMAL\n");
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(2000));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(2000));
            
        }
        else if (xSemaphoreTake(xBinarySemaphoreLEDState[STATE_ATTENTION], 0) == pdTRUE) {
            // State ATTENTION -> blink led once every 1 second
            Serial.println("LED ATTENTION\n");
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(1000));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(1000));
            
        }
        else if (xSemaphoreTake(xBinarySemaphoreLEDState[STATE_WARNING], 0) == pdTRUE) {
            // State WARNING -> blink led once every 0.5 second
            Serial.println("LED WARNING\n");
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(500));
            
        }
        else if (xSemaphoreTake(xBinarySemaphoreLEDState[STATE_CRITICAL], 0) == pdTRUE) {
            // State CRITICAL -> LED ON continuously
            Serial.println("LED CRITICAL\n");
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(2000));
            
        }
        else{
            // No signal -> turn off LED
            Serial.println("LED OFF");
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}
