/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "training_led" alias. */
#define LED0_NODE DT_ALIAS(training_led)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static void my_thread_handler(void *dummy1, void *dummy2, void *dummy3) {
	int ret;

	if (!device_is_ready(led.port)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	while(true) {
		ret = gpio_pin_toggle_dt(&led);
		if(ret < 0) {
			return;
		}

		k_sleep(K_SECONDS(1));
	}
}

K_THREAD_DEFINE(my_thread, 1024,
				my_thread_handler, NULL, NULL, NULL,
	 			K_LOWEST_APPLICATION_THREAD_PRIO, 0, 0);


int main(void)
{
	k_thread_start(my_thread);

	while(true) {
		k_msleep(SLEEP_TIME_MS);
	}
}
