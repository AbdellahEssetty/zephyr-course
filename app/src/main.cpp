#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
#include <zephyr/logging/log.h>

#ifndef CONFIG_LED_SLEEP_TIME_MS
#define CONFIG_LED_SLEEP_TIME_MS 1000
#endif

#ifndef CONFIG_LED_BRIGHTNESS
#define CONFIG_LED_BRIGHTNESS 50
#endif

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

#define PWM_LED_NODE DT_NODELABEL(pwm_led0)
static const struct led_dt_spec led_pwm = LED_DT_SPEC_GET(LED_NODE);


LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;
    // led_set_brightness_dt(&led_pwm, CONFIG_LED_BRIGHTNESS);



    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_LED_SLEEP_TIME_MS);
    }
    return 0;
}
