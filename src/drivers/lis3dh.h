#pragma once

#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"
// #define I2C_ADDRESS 001100xb // 0x1b? from datasheet i2c
#define I2C_ADDRESS 0x19 // because accel is internally pulled up. so it sets this register. 
#define ACCEL_WHO_AM_I 0x0F // from datasheet i2c
#define CTRL_REG1 0x20 // from datasheet i2c
#define OUT_X_L 0x28
#define OUT_X_H 0x29 
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

// SAD[0] = SA0 -> val of 0
#define I2C_READ_REG 0x33 // from datasheet -> SAD+R/W?
#define I2C_WRITE_REG 0x32 // from datasheet -> SAD+R/W?

// Initialise the accelerometer
void accel_init();

bool accel_write_register(uint8_t reg, uint8_t data);

bool accel_read_register(uint8_t reg, uint8_t *data, size_t length);

// void accel_write_register();
