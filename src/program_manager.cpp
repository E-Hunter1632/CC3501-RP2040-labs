#include "pico/stdlib.h"
#include "programs.h"
#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpio.h"
#include "boardPins.h"

#define TOTAL_PROGRAMS 3

volatile int current_program = 0;

void next_program(void) {
    current_program = (current_program + 1) % TOTAL_PROGRAMS;
}

// Button GPIO interrupt handler
void gpio_callback(uint gpio, uint32_t events) {
    static uint32_t last_press = 0;
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (now - last_press > 200) { // debounce ~200ms
        last_press = now;
        next_program();
    }
}