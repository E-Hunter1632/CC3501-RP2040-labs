#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"

#include "WS2812.pio.h"
#include "drivers/logging/logging.h"
#include "drivers/leds.h"
#include "drivers/lis3dh.h"

#define UART_TX_PIN 8
#define UART_RX_PIN 9

static bool accel_initialized = false;
static char buf[100];

void accelerometer_task_init() {
    if (accel_initialized) return;

    stdio_init_all();
    leds_init();
    leds_off();
    accel_init();

    uart_init(uart1, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_baudrate(uart1, 115200);

    uint8_t result = 0;
    accel_read_register(ACCEL_WHO_AM_I, &result, 1);
    printf("Accelerometer WHO_AM_I: %d\n", result);

    uint8_t config = 0b01110111;
    accel_write_register(CTRL_REG1, config);

    accel_read_register(CTRL_REG1, &result, 1);
    printf("Accelerometer CTRL_REG1: %d\n", result);

    accel_initialized = true;
}

void accelerometer_task_loop() {
    uint8_t raw_data[2];
    int M = 4;

    // X axis
    accel_read_register(OUT_X_L, &raw_data[0], 1);
    accel_read_register(OUT_X_H, &raw_data[1], 1);
    int16_t x_raw = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> M;
    float x_g = (float)x_raw * 0.001f;

    // Y axis
    accel_read_register(OUT_Y_L, &raw_data[0], 1);
    accel_read_register(OUT_Y_H, &raw_data[1], 1);
    int16_t y_raw = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> M;
    float y_g = (float)y_raw * 0.001f;

    // Z axis
    accel_read_register(OUT_Z_L, &raw_data[0], 1);
    accel_read_register(OUT_Z_H, &raw_data[1], 1);
    int16_t z_raw = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> M;
    float z_g = (float)z_raw * 0.001f;

    snprintf(buf, 100, "%f,%f,%f\n", x_g, y_g, z_g);
    uart_puts(uart1, buf);

    // LED logic same as your original
    if (x_g > 0.35f && x_g < 0.75f) {
        leds_set_rgb(5, 0, 0, 0);
        leds_set_rgb(6, 0, 255, 0);
        leds_set_rgb(7, 0, 0, 0);
    } else if (x_g > 0.75f) {
        leds_set_rgb(5, 0, 0, 0);
        leds_set_rgb(6, 0, 0, 0);
        leds_set_rgb(7, 255, 0, 0);
    } else if (x_g < -0.35f && x_g > -0.75f) {
        leds_set_rgb(4, 0, 0, 0);
        leds_set_rgb(5, 0, 255, 0);
        leds_set_rgb(6, 0, 0, 0);
    } else if (x_g < -0.75f) {
        leds_set_rgb(6, 0, 0, 0);
        leds_set_rgb(5, 0, 0, 0);
        leds_set_rgb(4, 255, 0, 0);
    } else {
        leds_set_rgb(4, 0, 0, 0);
        leds_set_rgb(5, 0, 0, 255);
        leds_set_rgb(6, 0, 0, 255);
        leds_set_rgb(7, 0, 0, 0);
    }

    if (y_g > 0.35f && y_g < 0.75f) {
        leds_set_rgb(1, 0, 0, 0);
        leds_set_rgb(2, 0, 255, 0);
        leds_set_rgb(3, 0, 0, 0);
    } else if (y_g > 0.75f) {
        leds_set_rgb(1, 0, 0, 0);
        leds_set_rgb(2, 0, 0, 0);
        leds_set_rgb(3, 255, 0, 0);
    } else if (y_g < -0.35f && y_g > -0.75f) {
        leds_set_rgb(0, 0, 0, 0);
        leds_set_rgb(1, 0, 255, 0);
        leds_set_rgb(2, 0, 0, 0);
    } else if (y_g < -0.75f) {
        leds_set_rgb(2, 0, 0, 0);
        leds_set_rgb(1, 0, 0, 0);
        leds_set_rgb(0, 255, 0, 0);
    } else {
        leds_set_rgb(0, 0, 0, 0);
        leds_set_rgb(1, 0, 0, 255);
        leds_set_rgb(2, 0, 0, 255);
        leds_set_rgb(3, 0, 0, 0);
    }

    update_leds();
    sleep_ms(100); // Keep things responsive
}
