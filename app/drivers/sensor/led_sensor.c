
#define DT_DRV_COMPAT demo_led_sensor

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include "led_sensor.h"

LOG_MODULE_REGISTER(led_sensor, CONFIG_SENSOR_LOG_LEVEL);

struct led_sensor_config
{
    const struct gpio_dt_spec led;
};

struct led_sensor_data
{
    uint32_t sample_count;
};

static int led_sensor_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan)
{
    const struct led_sensor_config *config = dev->config;
    struct led_sensor_data *data = dev->data;
    int ret;

    ARG_UNUSED(chan);

    if (!gpio_is_ready_dt(&config->led))
    {
        return -ENODEV;
    }

    ret = gpio_pin_set_dt(&config->led, 1);
    if (ret < 0)
    {
        return ret;
    }
    data->sample_count++;

    LOG_INF("LED turned ON");
    return 0;
}

static int led_sensor_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val)
{
    const struct led_sensor_config *config = dev->config;
    struct led_sensor_data *data = dev->data;
    int ret;
    
    val->val1 = data->sample_count;
    val->val2 = 0;

    ARG_UNUSED(chan);

    ret = gpio_pin_set_dt(&config->led, 0);
    if (ret < 0)
    {
        return ret;
    }

    LOG_INF("LED turned OFF");
    return 0;
}

int led_sensor_set_sample_count(const struct device *dev, uint32_t count)
{
    struct led_sensor_data *data = dev->data;

    if (data == NULL) {
        return -EINVAL;
    }

    data->sample_count = count;

    LOG_INF("sample_count updated to %u", count);

    return 0;
}

static int led_sensor_init(const struct device *dev)
{
    const struct led_sensor_config *config = dev->config;
    int ret;

    if (!gpio_is_ready_dt(&config->led))
    {
        LOG_ERR("LED GPIO controller is not ready");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&config->led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0)
    {
        LOG_ERR("Failed to configure LED GPIO: %d", ret);
        return ret;
    }

    return 0;
}

static const struct sensor_driver_api led_sensor_api = {
    .sample_fetch = led_sensor_sample_fetch,
    .channel_get = led_sensor_channel_get,
};

#define LED_SENSOR_DEFINE(inst)                                        \
    static const struct led_sensor_config led_sensor_config_##inst = { \
        .led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios),                \
    };                                                                 \
    static struct led_sensor_data led_sensor_data_##inst;              \
    DEVICE_DT_INST_DEFINE(inst,                                        \
                          led_sensor_init,                             \
                          NULL,                                        \
                          &led_sensor_data_##inst,                     \
                          &led_sensor_config_##inst,                   \
                          POST_KERNEL,                                 \
                          CONFIG_SENSOR_INIT_PRIORITY,                 \
                          &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_DEFINE)