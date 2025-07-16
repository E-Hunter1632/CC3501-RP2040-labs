#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "drivers/leds.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "leds_task.h"
#include "boardPins.h"


void leds_task(void)
{
    stdio_init_all();

    leds_init(); 
    leds_off(); // Turn off all LEDs initially in case some haven't reset from prior task. 

    // implement interupt for when the button is pressed to stop this task and run another. 
    // Break out of loop and function when button is pressed. 
    gpio_init(SW1); // Initialize the switch GPIO pin
    gpio_set_dir(SW1, GPIO_IN); // Set the switch pin as input

    // for (;;) {
        // Cycle colours of leds like a rainbow: 
        for (int i = 1; i < 12; i++) {
            // Set the color of each LED in the chain
            leds_set_rgb(0, 255, 255, 255); // Set first LED to white
            leds_set_rgb(i, (i * 100) % 256, (i * 50) % 256, (i * 10) % 256);
            update_leds(); // Update the LEDs with the current data
            sleep_ms(50);
        }
        for (int i = 1; i < 12; i++) {
            // Set the color of each LED in the chain
            leds_set_rgb(0, 255, 255, 255); // Set first LED to white
            leds_set_rgb(i, (i * 10) % 256, (i * 20) % 256, (i * 30) % 256);
            update_leds(); // Update the LEDs with the current data
            sleep_ms(50);
        }
        for (int i = 1; i < 12; i++) {
            // Set the color of each LED in the chain
            leds_set_rgb(0, 255, 255, 255); // Set first LED to white
            leds_set_rgb(i, (i * 20) % 256, (i * 40) % 256, (i * 60) % 256);
            update_leds(); // Update the LEDs with the current data
            sleep_ms(50);
        }
        for (int i = 1; i < 12; i++) {
            leds_set_rgb(0, 255, 255, 255); // Set first LED to white
            leds_set_rgb(i, (i * 50) % 256, (i * 100) % 256, (i * 20) % 256);
            update_leds(); // Update the LEDs with the current data
            sleep_ms(50);
        }
        for (int i = 1; i < 12; i++) {
            leds_set_rgb(0, 255, 255, 255); // Set first LED to white
            leds_set_rgb(i, (i * 60) % 256, (i * 80) % 256, (i * 100) % 256);
            update_leds(); // Update the LEDs with the current data
            sleep_ms(50);
        }
            for (int i = 1; i < 12; i++) {
            leds_set_rgb(0, 255, 255, 255); // Set first LED to white
            leds_set_rgb(i, (i * 80) % 256, (i * 100) % 256, (i * 120) % 256);
            update_leds(); // Update the LEDs with the current data
            sleep_ms(50);
        }

    // }
    // return 0;
}