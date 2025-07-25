#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "drivers/logging/logging.h"
#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "boardPins.h"
#include "hardware/adc.h"

#define FFT_LEN 1024 // Size of the microphone buffer

// Initialize the microphone
void microphone_init();

void microphone_read(uint16_t *data, size_t length); // Read microphone data into a buffer
