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
#include "drivers/microphone.h" 
#include "boardPins.h"
#include "hardware/adc.h"
#pragma once
#include "drivers/leds.h"

int microphone_task();
