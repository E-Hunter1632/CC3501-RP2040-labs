#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"

#include "drivers/leds.h" //---

// COULD ALSO HAVE boardPins.h TO BE ABLE TO CHANGE PINS EASILY FOR THE BOARD, AS WELL AS NUM OF LEDS
// AND ADD INCLUDE.H 

// setting the colours and led state here for now - then control stuff and functionality goes in led.cpp


int main()
{
    stdio_init_all();

    leds_init(); 


    for (;;) {
        // Test the log system
        log(LogLevel::INFORMATION, "Hello world");

        // // // TESTING THE USER INTERFACE TO ASK FOR COLOUR AND LED STATES: 
        // // cout << "Enter colour values for first LED - (R, G, B): ";
        leds_set_rgb(0, 255, 255, 0); 
        leds_set_rgb(1, 0, 255, 255); 
        leds_set_rgb(2, 255, 0, 255); 
        leds_set_rgb(6, 255, 255, 255);
        leds_set_rgb(3, 255, 0, 0);
        leds_set_rgb(4, 0, 255, 0);
        leds_set_rgb(5, 0, 0, 255);
        leds_set_rgb(7, 255, 100, 0);
        leds_set_rgb(8, 100, 0, 255);
        leds_set_rgb(9, 255, 0, 100);
        leds_set_rgb(10, 0, 255, 100);
        leds_set_rgb(11, 100, 255, 0);

        update_leds(); // Update the LEDs with the current data

        sleep_ms(1000); // Wait for 1 second

        leds_off(); // Turn off all LEDs
        sleep_ms(500); // Wait for 0.5 seconds

       
    }

    return 0;
}
