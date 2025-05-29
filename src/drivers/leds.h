// Header file for LED control functions.
#pragma once

#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file



struct RGBColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};  

// Initialize the LED
void leds_init();

// // Set the color of LED (LED Number, RGB values: 0-255 (red, green, blue))
void leds_set_rgb(int Num_LED, uint8_t r, uint8_t g, uint8_t b);

void update_leds();

void leds_off();


