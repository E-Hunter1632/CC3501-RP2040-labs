#include <stdio.h>
#include "pico/stdlib.h"
#include "boardPins.h"
#include "lis3dh.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "drivers/logging/logging.h"

uint8_t deviceID = 0x33; // 00110011 correct WHO_AM_I return value from datasheet converted to hex

void accel_init() {
    i2c_init(I2C_INSTANCE , 400 * 1000); // Initialize I2C with 400kHz frequency
    gpio_set_function(ACCEL_SDA_PIN, GPIO_FUNC_I2C); // Set SDA pin to I2C function
    gpio_set_function(ACCEL_SCL_PIN, GPIO_FUNC_I2C); // Set SCL pin to I2C function
    printf("Accelerometer initialized.\n");
}

bool accel_write_register(uint8_t reg, uint8_t data) {
    uint8_t buf[2];
    buf[0] = reg; // Register address
    buf[1] = data; // Data to write
    int bytes_written = i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, buf, 2, false); // Write data to the register
    if (bytes_written != 2) {
        log(LogLevel::ERROR, "Failed to write to accelerometer register.");
        return false;
    }
    return true;
}

bool accel_read_register(uint8_t reg, uint8_t *data, size_t length) {
    if (1 != i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &reg, 1, true)) { 
        log(LogLevel::ERROR, "lis3dh::read_registers: Failed to select register address.");
    return false;
    }
    int bytes_read = i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, data, length, false);
    return bytes_read;
     if (bytes_read != length) {
        log(LogLevel::ERROR, "lis3dh::read_registers: Failed to read data.");
    return false;
     }
    }

