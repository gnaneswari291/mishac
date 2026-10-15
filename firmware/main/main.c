#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_GPIO GPIO_NUM_4
#define WALK_LED_GPIO GPIO_NUM_2
#define DEBOUNCE_US 50000LL
#define WALK_DURATION_US 10000000LL
#define BLINK_HALF_PERIOD_US 500000LL
#define SAMPLE_PERIOD_MS 10

static const char *TAG = "crosswalk";

typedef enum {
    BEACON_IDLE = 0,
    BEACON_WALK_ACTIVE,
} beacon_state_t;

static void configure_gpio(void)
{
    const gpio_config_t button_config = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&button_config));

    const gpio_config_t led_config = {
        .pin_bit_mask = 1ULL << WALK_LED_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&led_config));
    ESP_ERROR_CHECK(gpio_set_level(WALK_LED_GPIO, 0));
}

void app_main(void)
{
    configure_gpio();

    const int64_t boot_time_us = esp_timer_get_time();
    bool raw_pressed = false;
    bool stable_pressed = false;
    int64_t raw_change_time_us = boot_time_us;
    int64_t walk_start_time_us = 0;
    beacon_state_t state = BEACON_IDLE;

    ESP_LOGI(TAG, "BOOT: button_gpio=%d led_gpio=%d uart_baud=115200",
             BUTTON_GPIO, WALK_LED_GPIO);
    ESP_LOGI(TAG, "STATE=IDLE LED=OFF");

    while (true) {
        const int64_t now_us = esp_timer_get_time();
        const bool sampled_pressed = (gpio_get_level(BUTTON_GPIO) == 0);

        if (sampled_pressed != raw_pressed) {
            raw_pressed = sampled_pressed;
            raw_change_time_us = now_us;
        }

        if ((raw_pressed != stable_pressed) &&
            ((now_us - raw_change_time_us) >= DEBOUNCE_US)) {
            stable_pressed = raw_pressed;
            if (stable_pressed) {
                if (state == BEACON_IDLE) {
                    state = BEACON_WALK_ACTIVE;
                    walk_start_time_us = now_us;
                    ESP_LOGI(TAG, "REQUEST_ACCEPTED");
                    ESP_LOGI(TAG, "STATE=WALK_ACTIVE duration_ms=10000");
                } else {
                    ESP_LOGI(TAG, "REQUEST_IGNORED_BUSY");
                }
            }
        }

        if (state == BEACON_WALK_ACTIVE) {
            const int64_t elapsed_us = now_us - walk_start_time_us;
            if (elapsed_us >= WALK_DURATION_US) {
                state = BEACON_IDLE;
                ESP_ERROR_CHECK(gpio_set_level(WALK_LED_GPIO, 0));
                ESP_LOGI(TAG, "WALK_TIMER_COMPLETE");
                ESP_LOGI(TAG, "STATE=IDLE LED=OFF");
            } else {
                const bool led_on = ((elapsed_us / BLINK_HALF_PERIOD_US) % 2) == 0;
                ESP_ERROR_CHECK(gpio_set_level(WALK_LED_GPIO, led_on ? 1 : 0));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(SAMPLE_PERIOD_MS));
    }
}
