#include "led_blinky.h"

void led_blinky(void *pvParameters) {
    pinMode(LED_GPIO, OUTPUT);

    while (1) {
        float temp = glob_temperature;

        // Sensor chưa sẵn sàng
        if (temp < 0) {
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        // Nhiệt độ thấp → nháy chậm
        if (temp < 25.0) {
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(1000));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        // Nhiệt độ trung bình → nháy nhanh
        else if (temp < 35.0) {
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(300));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(300));
        }
        // Nhiệt độ cao → LED ON liên tục
        else {
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}
