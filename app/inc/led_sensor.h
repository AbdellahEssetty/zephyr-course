#ifndef LED_SENSOR_H
#define LED_SENSOR_H

#include <stdint.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int led_sensor_set_sample_count(const struct device *dev, uint32_t count);

#ifdef __cplusplus
}
#endif

#endif /* LED_SENSOR_H */