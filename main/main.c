#include "motor.h"
#include "wifi.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "webserver.h"

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());

    wifi_init_softap();

    start_webserver();

    motor_init();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}