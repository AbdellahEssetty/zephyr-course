#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
#include <zephyr/logging/log.h>

#include <zephyr/sys/printk.h>

#include "led_sensor.h"


#ifndef CONFIG_LED_SLEEP_TIME_MS
#define CONFIG_LED_SLEEP_TIME_MS 1000
#endif

#ifndef CONFIG_LED_BRIGHTNESS
#define CONFIG_LED_BRIGHTNESS 50
#endif

#ifndef CONFIG_APP_HEARTBEAT_PERIOD_MS
#define CONFIG_APP_HEARTBEAT_PERIOD_MS 500
#endif

// #define APP_LED DT_ALIAS(app_led)
// static const struct gpio_dt_spec app_led = GPIO_DT_SPEC_GET(APP_LED, gpios);

// #define LED_NODE DT_ALIAS(led0)
// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

// #define PWM_LED_NODE DT_NODELABEL(pwm_led0)
// static const struct led_dt_spec led_pwm = LED_DT_SPEC_GET(LED_NODE);




LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);



int main(void)
{
    // bool led_state = true;

    // if (!gpio_is_ready_dt(&app_led)) return 0;

    // if (gpio_pin_configure_dt(&app_led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    // while (1) {
    //     if (gpio_pin_toggle_dt(&app_led) < 0) return 0;

    //     led_state = !led_state;
    //     LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
    //     k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    // }
    // LOG_INF("Hello, Zephyr!");

    const struct device *led_sensor_dev = DEVICE_DT_GET(DT_NODELABEL(led_sensor));

    struct sensor_value val;
    int ret;

    if (!device_is_ready(led_sensor_dev))
    {
        LOG_ERR("LED sensor device not ready");
        return 0;
    }

    ret = led_sensor_set_sample_count(led_sensor_dev, 10U);

    if (ret < 0) {
        LOG_ERR("Failed to update sample count: %d", ret);
        return 0;
    }

    while(1)
    {
        ret = sensor_sample_fetch(led_sensor_dev);
        if (ret < 0)
        {
            LOG_ERR("Failed to fetch sample: %d", ret);
            return 0;
        }

        ret = sensor_channel_get(led_sensor_dev, SENSOR_CHAN_ALL, &val);
        if (ret < 0)
        {
            LOG_ERR("Failed to get channel data: %d", ret);
            return 0;
        }

        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
}
