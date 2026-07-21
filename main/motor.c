#include "motor.h"

#include "driver/gpio.h"
#include "driver/ledc.h"

/* Rear Motor */
#define PWMA_GPIO   25
#define AIN1_GPIO   26
#define AIN2_GPIO   27

/* Steering Motor */
#define PWMB_GPIO   14
#define BIN1_GPIO   18
#define BIN2_GPIO   19

/* Enable */
#define STBY_GPIO   33

/* PWM Configuration */
#define PWM_FREQ        1000
#define PWM_RESOLUTION  LEDC_TIMER_8_BIT
#define PWM_MODE        LEDC_LOW_SPEED_MODE
#define PWM_TIMER       LEDC_TIMER_0
#define PWM_CHANNEL     LEDC_CHANNEL_0

/*----------------------------------------------------------
    Set PWM Speed (0 - 100%)
----------------------------------------------------------*/
static void set_speed(uint8_t speed)
{
    if (speed > 100)
        speed = 100;

    uint32_t duty = (speed * 255) / 100;

    ledc_set_duty(PWM_MODE, PWM_CHANNEL, duty);
    ledc_update_duty(PWM_MODE, PWM_CHANNEL);
}

/*----------------------------------------------------------
    Initialization
----------------------------------------------------------*/
void motor_init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask =
            (1ULL << PWMA_GPIO) |
            (1ULL << AIN1_GPIO) |
            (1ULL << AIN2_GPIO) |
            (1ULL << PWMB_GPIO) |
            (1ULL << BIN1_GPIO) |
            (1ULL << BIN2_GPIO) |
            (1ULL << STBY_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);

    /* Configure PWM Timer */
    ledc_timer_config_t timer = {
        .speed_mode = PWM_MODE,
        .timer_num = PWM_TIMER,
        .duty_resolution = PWM_RESOLUTION,
        .freq_hz = PWM_FREQ,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&timer);

    /* Configure PWM Channel */
    ledc_channel_config_t channel = {
        .gpio_num = PWMA_GPIO,
        .speed_mode = PWM_MODE,
        .channel = PWM_CHANNEL,
        .timer_sel = PWM_TIMER,
        .duty = 0,
        .hpoint = 0
    };

    ledc_channel_config(&channel);

    /* Enable Motor Driver */
    gpio_set_level(STBY_GPIO, 1);

    /* Steering Enable Pin */
    gpio_set_level(PWMB_GPIO, 1);

    rear_stop();
    steer_stop();
}

/*==========================================================
                    Rear Motor
==========================================================*/

void rear_forward(uint8_t speed)
{
    gpio_set_level(AIN1_GPIO, 0);
    gpio_set_level(AIN2_GPIO, 1);
    set_speed(speed);
}

void rear_backward(uint8_t speed)
{
    gpio_set_level(AIN1_GPIO, 1);
    gpio_set_level(AIN2_GPIO, 0);
    set_speed(speed);
}

void rear_stop(void)
{
    set_speed(0);

    gpio_set_level(AIN1_GPIO, 0);
    gpio_set_level(AIN2_GPIO, 0);
}

/*==========================================================
                  Steering Motor
==========================================================*/

void steer_left(void)
{
    gpio_set_level(BIN1_GPIO, 1);
    gpio_set_level(BIN2_GPIO, 0);
}

void steer_right(void)
{
    gpio_set_level(BIN1_GPIO, 0);
    gpio_set_level(BIN2_GPIO, 1);
}

void steer_stop(void)
{
    gpio_set_level(BIN1_GPIO, 0);
    gpio_set_level(BIN2_GPIO, 0);
}