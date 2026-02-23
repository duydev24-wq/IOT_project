#include "led_blinky.h"

void led_blinky(void *pvParameters) {
    pinMode(LED_GPIO, OUTPUT);
    Serial.println("LED Blinky Task Started");
    while (1) {
        // Chờ tín hiệu từ task đo nhiệt độ và độ ẩm
        // if (xSemaphoreTake(xBinarySemaphoreTemp, portMAX_DELAY) == pdTRUE) {
        //     // Lấy giá trị nhiệt độ hiện tại
        //     float temp = glob_temperature;

        //     // Sensor chưa sẵn sàng
        //     if (temp < 0) {
        //         digitalWrite(LED_GPIO, LOW);
        //         vTaskDelay(pdMS_TO_TICKS(1000));
        //         continue;
        //     }

        //     // Nhiệt độ thấp → nháy chậm
        //     if (temp < 25.0) {
        //         digitalWrite(LED_GPIO, HIGH);
        //         vTaskDelay(pdMS_TO_TICKS(1000));
        //         digitalWrite(LED_GPIO, LOW);
        //         vTaskDelay(pdMS_TO_TICKS(1000));
        //     }
        //     // Nhiệt độ trung bình → nháy nhanh
        //     else if (temp < 35.0) {
        //         digitalWrite(LED_GPIO, HIGH);
        //         vTaskDelay(pdMS_TO_TICKS(300));
        //         digitalWrite(LED_GPIO, LOW);
        //         vTaskDelay(pdMS_TO_TICKS(300));
        //     }
        //     // Nhiệt độ cao → LED ON liên tục
        //     else {
        //         digitalWrite(LED_GPIO, HIGH);
        //         vTaskDelay(pdMS_TO_TICKS(1000));
        //     }
        //}
        if (xSemaphoreTake(xBinarySemaphoreLEDState[STATE_NORMAL], 0) == pdTRUE) {
            // Trạng thái bình thường → nháy xanh lá
            Serial.println("LED NORMAL\n");
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(2000));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(2000));
            
        }
        else if (xSemaphoreTake(xBinarySemaphoreLEDState[STATE_ATTENTION], 0) == pdTRUE) {
            // Trạng thái chú ý → nháy vàng
            Serial.println("LED ATTENTION\n");
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(2000));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(2000));
            
        }
        else if (xSemaphoreTake(xBinarySemaphoreLEDState[STATE_WARNING], 0) == pdTRUE) {
            // Trạng thái cảnh báo → nháy cam
            Serial.println("LED WARNING\n");
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(2000));
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(2000));
            
        }
        else if (xSemaphoreTake(xBinarySemaphoreLEDState[STATE_CRITICAL], 0) == pdTRUE) {
            // Trạng thái nguy cấp → LED ON liên tục đỏ
            Serial.println("LED CRITICAL\n");
            digitalWrite(LED_GPIO, HIGH);
            vTaskDelay(pdMS_TO_TICKS(2000));
            
        }
        else{
            // Không có tín hiệu → tắt LED
            Serial.println("LED OFF");
            digitalWrite(LED_GPIO, LOW);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}
