#include "motor.h"
#include "wifi.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    wifi_init_softap();

    motor_init();

    while (1)
    {
        rear_forward(50);
        vTaskDelay(pdMS_TO_TICKS(2000));

        rear_stop();
        vTaskDelay(pdMS_TO_TICKS(1000));

        rear_backward(50);
        vTaskDelay(pdMS_TO_TICKS(2000));

        rear_stop();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}