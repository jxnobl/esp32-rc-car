#include "battery.h"

#include <stdbool.h>

#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"

#define BATTERY_ADC     ADC_CHANNEL_6
#define BATTERY_ATTEN   ADC_ATTEN_DB_12

#define VOLTAGE_DIVIDER_RATIO 3.2f

static adc_oneshot_unit_handle_t adc_handle;
static bool initialized = false;

static void battery_init(void)
{
    if (initialized)
        return;

    adc_oneshot_unit_init_cfg_t init = {
        .unit_id = ADC_UNIT_1
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init, &adc_handle));

    adc_oneshot_chan_cfg_t cfg = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = BATTERY_ATTEN
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(
        adc_handle,
        BATTERY_ADC,
        &cfg));

    initialized = true;
}

float battery_voltage(void)
{
    battery_init();

    int raw = 0;

    ESP_ERROR_CHECK(adc_oneshot_read(
        adc_handle,
        BATTERY_ADC,
        &raw));

    float adc_voltage = ((float)raw / 4095.0f) * 3.3f;

    return adc_voltage * VOLTAGE_DIVIDER_RATIO;
}

int battery_percent(void)
{
    float v = battery_voltage();

    if (v >= 8.4f)
        return 100;

    if (v <= 6.0f)
        return 0;

    return (int)(((v - 6.0f) / (8.4f - 6.0f)) * 100.0f);
}