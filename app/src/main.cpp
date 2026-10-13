#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
#include <zephyr/logging/log.h>

#include <zephyr/sys/printk.h>

#include <zephyr/device.h>
#include <zephyr/shell/shell.h>

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


static const struct device *led_sensor_dev = DEVICE_DT_GET(DT_NODELABEL(led_sensor));

static int cmd_sensor_fetch(const struct shell *shell, size_t argc, char **argv)
{
    struct sensor_value val;
    int ret;

    if (!device_is_ready(led_sensor_dev))
    {
        shell_error(shell, "LED sensor device not ready");
        return -ENODEV;
    }

    ret = sensor_sample_fetch(led_sensor_dev);
    if (ret < 0)
    {
        shell_error(shell, "Failed to fetch sample: %d", ret);
        return ret;
    }

    ret = sensor_channel_get(led_sensor_dev, SENSOR_CHAN_ALL, &val);
    if (ret < 0)
    {
        shell_error(shell, "Failed to get channel data: %d", ret);
        return ret;
    }

    shell_print(shell, "Sample count: %d", val.val1);

    return 0;
}

static int cmd_sensor_read(const struct shell *shell, size_t argc, char **argv)
{
    struct sensor_value val;
    int ret;

    if (!device_is_ready(led_sensor_dev))
    {
        shell_error(shell, "LED sensor device not ready");
        return -ENODEV;
    }

    ret = sensor_channel_get(led_sensor_dev, SENSOR_CHAN_ALL, &val);
    if (ret < 0)
    {
        shell_error(shell, "Failed to get channel data: %d", ret);
        return ret;
    }

    shell_print(shell, "Sample count: %d", val.val1);

    return 0;
}

static int cmd_sensor_info(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "LED Sensor Device: %s", led_sensor_dev->name);
    shell_print(shell, "Ready: %s", device_is_ready(led_sensor_dev) ? "Yes" : "No");
    return 0;
}

static int cmd_sensor_set(const struct shell *shell, size_t argc, char **argv)
{
    if (argc < 2)
    {
        shell_error(shell, "Usage: sensor set <count>");
        return -EINVAL;
    }

    uint32_t count = strtoul(argv[1], NULL, 10);
    int ret = led_sensor_set_sample_count(led_sensor_dev, count);
    if (ret < 0)
    {
        shell_error(shell, "Failed to set sample count: %d", ret);
        return ret;
    }

    shell_print(shell, "Sample count set to: %d", count);
    return 0;
}


SHELL_STATIC_SUBCMD_SET_CREATE(sensor_cmds,
    SHELL_CMD(fetch, NULL, "Fetch a new sample from the LED sensor", cmd_sensor_fetch),
    SHELL_CMD(read, NULL, "Read the last fetched sample from the LED sensor", cmd_sensor_read),
    SHELL_CMD(info, NULL, "Get information about the LED sensor device", cmd_sensor_info),
    SHELL_CMD(set, NULL, "Set the sample count for the LED sensor", cmd_sensor_set),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_cmds, "LED Sensor commands", NULL);

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

    // const struct device *led_sensor_dev = DEVICE_DT_GET(DT_NODELABEL(led_sensor));

    // struct sensor_value val;
    // int ret;

    // if (!device_is_ready(led_sensor_dev))
    // {
    //     LOG_ERR("LED sensor device not ready");
    //     return 0;
    // }

    // ret = led_sensor_set_sample_count(led_sensor_dev, 10U);

    // if (ret < 0) {
    //     LOG_ERR("Failed to update sample count: %d", ret);
    //     return 0;
    // }

    // while(1)
    // {
    //     ret = sensor_sample_fetch(led_sensor_dev);
    //     if (ret < 0)
    //     {
    //         LOG_ERR("Failed to fetch sample: %d", ret);
    //         return 0;
    //     }

    //     ret = sensor_channel_get(led_sensor_dev, SENSOR_CHAN_ALL, &val);
    //     if (ret < 0)
    //     {
    //         LOG_ERR("Failed to get channel data: %d", ret);
    //         return 0;
    //     }

    //     k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    // }
}
