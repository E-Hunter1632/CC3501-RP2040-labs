// Create LED class to control the LED chain
// #ifndef LEDS_H
// #define LEDS_H
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "leds.h"
#include "boardPins.h"


// void leds_init(uint led_pin, uint32_t frequency) {
    // // Initialize PIO0 to control the LED chain
    // pio_program_offset = pio_add_program(pio0, &ws2812_program);
    // ws2812_program_init(pio0, 0, pio_program_offset, led_pin, frequency, false);

uint32_t led_data[12]; // Array to hold LED data for 12 LEDs - WILL LATER BE PUT INTO BoardPins.h AND INCLUDE

// Definition of leds_init() should only be present in one source file.
// Remove this definition if it is also defined in another file (e.g., main.cpp).
// If you want to keep it here, ensure only a declaration exists in the header file.
void leds_init() {
    // Initialise PIO0 to control the LED chain
    printf("Initializing LEDs...\n");
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
}

void leds_set_rgb(int Num_LED, uint8_t r, uint8_t g, uint8_t b) {
    led_data[Num_LED] = (r << 24) | (g << 16) | (b << 8);
    // pio_sm_put_blocking(pio0, 0, led_data[Num_LED]);
}


void update_leds() {
    // This function is called to update the LEDs with the current data
    for (int i = 0; i < 12; i++) {
        pio_sm_put_blocking(pio0, 0, led_data[i]);
    }
    sleep_ms(0.3); // Delay to allow the LEDs to update - Too long - Delay found from datasheet (300uS)
}

void leds_off() {
    // Turn off all LEDs by setting their data to 0
    for (int i = 0; i < 12; i++) {
        led_data[i] = 0;
    }
   update_leds(); // Update the LEDs to turn them off
    printf("All LEDs turned off.\n");
}


// #endif // LEDS_H
