#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "boardPins.h"
#include <stdio.h>
#include "pico/stdlib.h"

// gpio_init(SW1); // Initialize the switch GPIO pin
// gpio_set_dir(SW1, GPIO_IN); // Set the switch pin as input
// gpio_set_irq_enabled_with_callback(SW1, GPIO_IRQ_EDGE_FALL, true, &button_callback); // Set up an interrupt for the switch pin - look for rising edge (button press) and call the button_callback function