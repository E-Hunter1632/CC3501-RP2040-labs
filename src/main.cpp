#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "boardPins.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"

#include "drivers/leds.h" //---
#include "drivers/lis3dh.h" // Include the accelerometer driver

#include "accelerometer_task.h" // Include the accelerometer task file for functionality
#include "leds_task.h"

// Define the button callback function
void button_callback(uint gpio, uint32_t events) {
    // Add your button handling code here, or leave empty if not needed yet
    // increment the option so the function later can use it to switch tasks. 
    static int option = 0; // Static variable to keep track of the current task option
    option = (option + 1) % 3; // Increment the option and wrap around after 2
}


int main()
{
    stdio_init_all();

    // accelerometer_task(); // Call the accelerometer task to initialize and run the accelerometer functionality

    // leds_task(); // Call the LED task to initialize and run the LED functionality

    printf("Starting main task...\n");
    printf("Press the button to switch between tasks.\n");

    // switch / case statement to switch between accelerometer and LED tasks using button press: 
    gpio_init(SW1); // Initialize the switch GPIO pin
    gpio_set_dir(SW1, GPIO_IN); // Set the switch pin as input
    gpio_set_irq_enabled_with_callback(SW1, GPIO_IRQ_EDGE_FALL, true, &button_callback); // Set up an interrupt for the switch pin
    int option = 1; // Variable to keep track of the current task option
    if (option == 0) {
        // Run the LED task
        leds_task();
    } else if (option == 1) {
        // Run the accelerometer task
        accelerometer_task();
    } else if (option == 2) {
        // Run the microphone task (if implemented)
        printf("Microphone task is not implemented yet.\n"); // just to see if it works. 
        // microphone_task(); // Uncomment this line when the microphone task is implemented
    }
    if (option > 2) {
        option = 0; // Reset the option if it exceeds the number of tasks & to reset task selection (ie, go back to leds after mic.)
    }

    // NEW CODE FOr MIC BELOW HERE - DELETE COMMENTED CODE LATER ONCE CONFIRMED FULLY WORKING. 


   

    return 0;
}
