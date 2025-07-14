#include <stdio.h>
#include "pico/stdlib.h"
#include "boardPins.h"
#include "lis3dh.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "drivers/logging/logging.h"
#include "drivers/microphone.h"
#include "hardware/adc.h"

// Initialise the microphone
void microphone_init() {
    // Initialize the ADC for the microphone
    adc_init();
    adc_gpio_init(MIC_ADC_PIN); // Initialize the GPIO pin for the microphone ADC
    adc_select_input(0); // Select ADC input 0 (assuming the microphone is connected to ADC0)
    adc_set_clkdiv(1114); // Set the ADC clock divider to 1114.0f for a sample rate of 44.1kHz
    // Set up the ADC FIFO
    adc_fifo_setup(true, false, 0, false, false); // Enable FIFO, enable DREQ, set threshold to 1, no error in FIFO, no byte shift
    adc_fifo_drain(); // Drain the FIFO to clear any previous data
    adc_run(false); // Stop the ADC to ensure it is ready for use
    printf("Microphone initialized on ADC pin %d\n", MIC_ADC_PIN);
}

void microphone_read(uint16_t *data, size_t length) {
    // Enable ADC free running mode
    adc_run(true); // Start the ADC in free running mode
    // Read from FIFO buffer using adc_fifo_get_blocking() to read 1024 number of samples into a buffer
    // number of samples defines as size_t samples = 1024; 
    for (size_t i = 0; i < length; i++) {
        data[i] = adc_fifo_get_blocking(); // Read the ADC value from the FIFO
    }
    adc_run(false); // Stop the ADC after reading the data
    adc_fifo_drain(); // Drain the FIFO to clear any remaining data
    printf("Microphone data read complete. %zu samples read.\n", length);

    // for (size_t i = 0; i < length; i++) {
        // adc_select_input(0); // Select ADC input 0 
        // data[i] = adc_read(); // Read the ADC value
    // }
}

