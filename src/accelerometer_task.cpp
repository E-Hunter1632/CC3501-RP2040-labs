#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"

#include "drivers/leds.h" //---
#include "drivers/lis3dh.h" // Include the accelerometer driver

#include "accelerometer_task.h" // Include the accelerometer task file for functionality

int accelerometer_task()
{
    stdio_init_all();

    leds_init(); 
    leds_off(); // Turn off all LEDs initially in case some haven't reset from prior task. 

    accel_init(); // Initialize the accelerometer

    // Testing to read the WHO_AM_I register - Works. 
    uint8_t deviceID = 0x33;
    uint8_t result = 0;
    accel_read_register(ACCEL_WHO_AM_I, &result, 1); // Read the WHO_AM_I register
    printf("Accelerometer WHO_AM_I: %d\n", result); // Print the device ID

    // Setting the control registers: 
    // CTRL_REG1 - write and set the correct mode (0111 - for sample mode):
    // byte to contain correct data: 01110111 
    // uint8_t ctrl_reg1 = 0b01110111; // Set to 01110111 for normal mode, 400Hz, and enable all axes
    uint8_t config = 0b01110111;
    uint8_t config_result;
    accel_write_register(CTRL_REG1, config); // Write to CTRL_REG1 to set the accelerometer configuration
    accel_read_register(CTRL_REG1, &config_result, 1); // Read back the CTRL_REG1 to verify
    printf("Accelerometer CTRL_REG1: %d\n", config_result); //

    for (;;) {
        
        // Now attempting to read the data - THIS WILL ALSO BE CLEANED UP LATER INTO SEPARATE FUNCTIONS SUCH AS READ_ACCELERATION(): 
        uint8_t raw_data[2];
        // least significant byte read into raw_data[0], most significant byte read into raw_data[1]
        // M be number of zeros padded by sensor
        // -> int16_t data = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> M; // Combine the two bytes into a single 16-bit value
        int M = 4; // Number of bits to shift right to get the correct value (depends on the sensitivity setting)
        // read x-axis data: 
        accel_read_register(OUT_X_L, &raw_data[0], 2);
        accel_read_register(OUT_X_H, &raw_data[1], 2);
        int16_t x_raw_data = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> M; // Combine the two bytes into a single 16-bit value
        // printf("Accelerometer x_raw_data: %d\n", x_raw_data); // Print the accelerometer data
        // convert raw data to g's:
        float x_g = (float)x_raw_data*0.001f; // Assuming 2g sensitivity, 16384 LSB/g
        printf("Accelerometer x_g: %f\n", x_g); // Print the accelerometer data in g's
        
        // read y-axis data:
        accel_read_register(OUT_Y_L, &raw_data[0], 2);
        accel_read_register(OUT_Y_H, &raw_data[1], 2);
        int16_t y_raw_data = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> M;
        // printf("Accelerometer y_raw_data: %d\n", y_raw_data);
        // convert raw data to g's:
        float y_g = (float)y_raw_data*0.001f; // Assuming 2g sensitivity, 16384 LSB/g
        printf("Accelerometer y_g: %f\n", y_g); // Print the accelerometer data in g's

        // read z-axis data:
        accel_read_register(OUT_Z_L, &raw_data[0], 2);
        accel_read_register(OUT_Z_H, &raw_data[1], 2);
        int16_t z_raw_data = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> M;
        // printf("Accelerometer z_raw_data: %d\n", z_raw_data);
        // convert raw data to g's:
        float z_g = (float)z_raw_data*0.001f; // Assuming 2g sensitivity, 16384 LSB/g
        printf("Accelerometer z_g: %f\n", z_g); // Print the accelerometer data in g's
       

        // Driving the LEDs based on accelerometer data as for the 'bubble'/'spirit' level: 
        // (For 'bubble'/'spirit' level, LEDs are opposide direction as 'bubble' 'floats' upwards of direction.)
        // THIS IS MESSY - JUST A PROOF OF CONCEPT - IT WILL BE ADDED TO IT'S OWN ACCELEROMETER_TASK.CPP FILE LATER.
        // For the X_Axis
        if (x_g > 0.35f & x_g < 0.75f) {
            leds_set_rgb(5, 0, 0, 0);
            leds_set_rgb(6, 0, 255, 0); 
            leds_set_rgb(7, 0, 0, 0);
            update_leds(); 
        } else if (x_g > 0.75f) {
            leds_set_rgb(5, 0, 0, 0);
            leds_set_rgb(6, 0, 0, 0); 
            leds_set_rgb(7, 255, 0, 0); 
            update_leds(); 
        } else if (x_g < -0.35f & x_g > -0.75f) {
            leds_set_rgb(4, 0, 0, 0);
            leds_set_rgb(5, 0, 255, 0); 
            leds_set_rgb(6, 0, 0, 0); 
            update_leds(); 
        } else if (x_g < -0.75f) {
            leds_set_rgb(6, 0, 0, 0); 
            leds_set_rgb(5, 0, 0, 0); 
            leds_set_rgb(4, 255, 0, 0); 
            update_leds(); 
        } else {
            leds_set_rgb(4, 0, 0, 0); 
            leds_set_rgb(5, 0, 0, 255); 
            leds_set_rgb(6, 0, 0, 255); 
            leds_set_rgb(7, 0, 0, 0); 
            update_leds(); 
        }
        // For the Y_Axis
        if (y_g > 0.35f & y_g < 0.75f) {
            leds_set_rgb(1, 0, 0, 0);
            leds_set_rgb(2, 0, 255, 0); 
            leds_set_rgb(3, 0, 0, 0);
            update_leds(); 
        } else if (y_g > 0.75f) {
            leds_set_rgb(1, 0, 0, 0);
            leds_set_rgb(2, 0, 0, 0); 
            leds_set_rgb(3, 255, 0, 0); 
            update_leds(); 
        } else if (y_g < -0.35f & y_g > -0.75f) {
            leds_set_rgb(0, 0, 0, 0);
            leds_set_rgb(1, 0, 255, 0); 
            leds_set_rgb(2, 0, 0, 0); 
            update_leds(); 
        } else if (y_g < -0.75f) {
            leds_set_rgb(2, 0, 0, 0); 
            leds_set_rgb(1, 0, 0, 0); 
            leds_set_rgb(0, 255, 0, 0); 
            update_leds(); 
        } else {
            leds_set_rgb(0, 0, 0, 0); 
            leds_set_rgb(1, 0, 0, 255); 
            leds_set_rgb(2, 0, 0, 255); 
            leds_set_rgb(3, 0, 0, 0); 
            update_leds(); 
        }

    }

    return 0;
}