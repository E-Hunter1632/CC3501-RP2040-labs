#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/adc.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"

#include "arm_math.h" // Include CMSIS-DSP header for FFT functions

#include "drivers/leds.h" //---

#include "microphone_task.h" // Include the accelerometer task file for functionality

#define FFT_LEN 1024 
// Size of the microphone buffer

static int16_t hanning_window [FFT_LEN]  = {0,0,1,3,5,8,11,15,20,25,31,37,44,52,61,69,79,89,100,111,123,136,149,163,178,193,208,225,242,259,277,296,315,335,356,377,399,421,444,468,492,517,542,568,595,622,650,678,707,736,767,797,829,860,893,926,960,994,1029,1064,1100,1137,1174,1211,1250,1288,1328,1368,1408,1449,1491,1533,1576,1619,1663,1708,1753,1798,1844,1891,1938,1986,2034,2083,2133,2182,2233,2284,2335,2387,2440,2493,2547,2601,2656,2711,2766,2823,2879,2937,2994,3053,3111,3171,3230,3291,3351,3413,3474,3536,3599,3662,3726,3790,3855,3920,3985,4051,4118,4185,4252,4320,4388,4457,4526,4596,4666,4737,4808,4879,4951,5023,5096,5169,5243,5317,5391,5466,5541,5617,5693,5769,5846,5923,6001,6079,6158,6236,6316,6395,6475,6555,6636,6717,6799,6880,6962,7045,7128,7211,7295,7379,7463,7547,7632,7717,7803,7889,7975,8062,8148,8236,8323,8411,8499,8587,8676,8765,8854,8944,9033,9123,9214,9304,9395,9486,9578,9670,9761,9854,9946,10039,10132,10225,10318,10412,10505,10599,10694,10788,10883,10978,11073,11168,11264,11359,11455,11551,11648,11744,11841,11937,12034,12131,12229,12326,12424,12521,12619,12717,12815,12914,13012,13111,13209,13308,13407,13506,13605,13704,13804,13903,14003,14102,14202,14302,14401,14501,14601,14701,14802,14902,15002,15102,15203,15303,15403,15504,15604,15705,15806,15906,16007,16107,16208,16309,16409,16510,16610,16711,16812,16912,17013,17113,17214,17314,17415,17515,17616,17716,17816,17916,18017,18117,18217,18317,18416,18516,18616,18716,18815,18915,19014,19113,19213,19312,19411,19509,19608,19707,19805,19904,20002,20100,20198,20296,20393,20491,20588,20685,20782,20879,20976,21072,21169,21265,21361,21457,21552,21647,21743,21838,21932,22027,22121,22216,22309,22403,22497,22590,22683,22776,22868,22961,23053,23144,23236,23327,23418,23509,23599,23690,23780,23869,23959,24048,24136,24225,24313,24401,24489,24576,24663,24750,24836,24922,25008,25093,25178,25263,25347,25431,25515,25599,25682,25764,25847,25929,26010,26091,26172,26253,26333,26413,26492,26571,26650,26728,26806,26883,26960,27037,27113,27189,27265,27340,27414,27488,27562,27636,27708,27781,27853,27925,27996,28067,28137,28207,28276,28345,28414,28482,28550,28617,28683,28750,28815,28881,28946,29010,29074,29137,29200,29263,29325,29386,29447,29508,29568,29627,29686,29745,29803,29860,29917,29974,30029,30085,30140,30194,30248,30301,30354,30407,30458,30510,30560,30611,30660,30709,30758,30806,30853,30900,30947,30993,31038,31083,31127,31170,31213,31256,31298,31339,31380,31420,31460,31499,31538,31576,31613,31650,31686,31722,31757,31791,31825,31859,31891,31924,31955,31986,32017,32046,32076,32104,32132,32160,32187,32213,32239,32264,32288,32312,32335,32358,32380,32402,32422,32443,32462,32481,32500,32518,32535,32551,32567,32583,32598,32612,32625,32638,32651,32662,32673,32684,32694,32703,32712,32720,32727,32734,32740,32746,32751,32755,32759,32762,32764,32766,32767,32767,32767,32767,32766,32764,32762,32759,32755,32751,32746,32740,32734,32727,32720,32712,32703,32694,32684,32673,32662,32651,32638,32625,32612,32598,32583,32567,32551,32535,32518,32500,32481,32462,32443,32422,32402,32380,32358,32335,32312,32288,32264,32239,32213,32187,32160,32132,32104,32076,32046,32017,31986,31955,31924,31891,31859,31825,31791,31757,31722,31686,31650,31613,31576,31538,31499,31460,31420,31380,31339,31298,31256,31213,31170,31127,31083,31038,30993,30947,30900,30853,30806,30758,30709,30660,30611,30560,30510,30458,30407,30354,30301,30248,30194,30140,30085,30029,29974,29917,29860,29803,29745,29686,29627,29568,29508,29447,29386,29325,29263,29200,29137,29074,29010,28946,28881,28815,28750,28683,28617,28550,28482,28414,28345,28276,28207,28137,28067,27996,27925,27853,27781,27708,27636,27562,27488,27414,27340,27265,27189,27113,27037,26960,26883,26806,26728,26650,26571,26492,26413,26333,26253,26172,26091,26010,25929,25847,25764,25682,25599,25515,25431,25347,25263,25178,25093,25008,24922,24836,24750,24663,24576,24489,24401,24313,24225,24136,24048,23959,23869,23780,23690,23599,23509,23418,23327,23236,23144,23053,22961,22868,22776,22683,22590,22497,22403,22309,22216,22121,22027,21932,21838,21743,21647,21552,21457,21361,21265,21169,21072,20976,20879,20782,20685,20588,20491,20393,20296,20198,20100,20002,19904,19805,19707,19608,19509,19411,19312,19213,19113,19014,18915,18815,18716,18616,18516,18416,18317,18217,18117,18017,17916,17816,17716,17616,17515,17415,17314,17214,17113,17013,16912,16812,16711,16610,16510,16409,16309,16208,16107,16007,15906,15806,15705,15604,15504,15403,15303,15203,15102,15002,14902,14802,14701,14601,14501,14401,14302,14202,14102,14003,13903,13804,13704,13605,13506,13407,13308,13209,13111,13012,12914,12815,12717,12619,12521,12424,12326,12229,12131,12034,11937,11841,11744,11648,11551,11455,11359,11264,11168,11073,10978,10883,10788,10694,10599,10505,10412,10318,10225,10132,10039,9946,9854,9761,9670,9578,9486,9395,9304,9214,9123,9033,8944,8854,8765,8676,8587,8499,8411,8323,8236,8148,8062,7975,7889,7803,7717,7632,7547,7463,7379,7295,7211,7128,7045,6962,6880,6799,6717,6636,6555,6475,6395,6316,6236,6158,6079,6001,5923,5846,5769,5693,5617,5541,5466,5391,5317,5243,5169,5096,5023,4951,4879,4808,4737,4666,4596,4526,4457,4388,4320,4252,4185,4118,4051,3985,3920,3855,3790,3726,3662,3599,3536,3474,3413,3351,3291,3230,3171,3111,3053,2994,2937,2879,2823,2766,2711,2656,2601,2547,2493,2440,2387,2335,2284,2233,2182,2133,2083,2034,1986,1938,1891,1844,1798,1753,1708,1663,1619,1576,1533,1491,1449,1408,1368,1328,1288,1250,1211,1174,1137,1100,1064,1029,994,960,926,893,860,829,797,767,736,707,678,650,622,595,568,542,517,492,468,444,421,399,377,356,335,315,296,277,259,242,225,208,193,178,163,149,136,123,111,100,89,79,69,61,52,44,37,31,25,20,15,11,8,5,3,1,0,0}; // Define the Q15 format for Hanning windowing
// creating a fft bin in order to drive the 12 leds, based on the following binning: 6, 8, 11, 16, 24, 35, 51, 75, 110, 161, 237, 349, 513
static int16_t fft_bins[13] = {6, 8, 11, 16, 24, 35, 51, 75, 110, 161, 237, 349, 513}; // Bins for the FFT output to drive the LEDs


int microphone_task() {
    stdio_init_all();

    leds_init(); 
    leds_off(); // Turn off all LEDs initially in case some haven't reset from prior task. 

    microphone_init(); // Initialize the microphone
    uint16_t adc_data[1024]; // Buffer to hold microphone data
    // mircophone_read(adc_data); // Read microphone data into the buffer - TESTING PURPOSES ONLY
    // Use the arm_rfft_init_q15 function to initialise the FFT instance
    arm_rfft_instance_q15 S;
    arm_rfft_init_q15(&S, 1024, 0, 1);

    if (arm_rfft_init_q15(&S, FFT_LEN, 0, 1) != ARM_MATH_SUCCESS) {
    printf("FFT init failed!\n");
    return -1;
    }


    for (;;) {
        leds_off(); // Start with all LEDs off
        // Read 1024 samples from the microphone into a buffer of type uint16_t.
        microphone_read(adc_data, FFT_LEN); // Read microphone data into the buffer
        // Calculate DC Bias - (average of adc samples - use int32_t)
        int32_t dc_bias = 0;
        for (int i = 0; i < 1024; i++) {
            dc_bias += adc_data[i];
        }
        dc_bias = dc_bias / 1024;
        // for (int i = 0; i < 10; i++) {
        //     printf("ADC[%d] = %u\n", i, adc_data[i]);
        // }

        int16_t time_domain_sig[1024]; // Buffer to hold the time domain signal
        //  Fill in each element in the buffer by taking the raw ADC values and subtracting off the DC bias
        for (int i = 0; i < 1024; i++) {
            time_domain_sig[i] = (int16_t)(adc_data[i] - dc_bias); // Convert to int16_t and center around 0 - ?
            time_domain_sig[i] = (adc_data[i] - dc_bias) << 4;
        }
        // ADC has 12 bits, filling Q15 fixed point number, left shift by 3 bits to fit into Q15 format. (can also increase to 5 bits). 
        for (int i = 0; i < 1024; i++) {
            time_domain_sig[i] <<= 5; // Shift left by 3 bits to fit into Q15 format
        }
        // Print the time domain signal for debugging: ---------------------------------------
        // for (size_t i = 0; i < 1024; i++) {
        //     printf("Time Domain Signal[%zu]: %d\n", i, time_domain_sig[i]);
        // }
        // apply the window function to your measured samples. (element-wise multiplication). - CAN REDUCE SIGNAL BY 50% - COULD COMMENT OUT FOR TESTING. 
        for (int i = 0; i < FFT_LEN; i++) {
            time_domain_sig[i] = ((int32_t)time_domain_sig[i] * (int32_t)hanning_window[i]) >> 15; // Apply Hanning window
        }
        // Calculate the FFT using the arm_rfft_q15 function ->  - NEED TO DEBUG
        // Apply the FFT 
        static int16_t freq_domain [FFT_LEN * 2]; // CHANGED FROM [FFT_LEN + 2]
        arm_rfft_q15(&S, time_domain_sig, freq_domain);
        // print freq_domain for debugging:-------------------------------------------------------
        // for (int i = 0; i < 32; i += 2) {
        //     int16_t real = freq_domain[i + 2];
        //     int16_t imag = freq_domain[i + 3];
        //     printf("Bin %d: Real = %d, Imag = %d\n", i / 2, real, imag);
        // }
        // for (size_t i = 0; i < FFT_LEN*2; i++) {
        //     printf("Frequency Domain[%zu]: %d\n", i, freq_domain[i]);
        // }


        // Calculate the magnitude squared of each complex number using the arm_cmplx_mag_squared_q15 function.

        // int16_t complex_buffer[FFT_LEN]; // Real,Imag,Real,Imag...

        // complex_buffer[0] = freq_domain[0];  // Real 0
        // complex_buffer[1] = 0;               // Imag 0 (DC component is real only)

        // for (int i = 1; i < FFT_LEN / 2; i++) {
        //     complex_buffer[2 * i]     = freq_domain[2 * i];     // Real i
        //     complex_buffer[2 * i + 1] = freq_domain[2 * i + 1]; // Imag i
        // }

        static int16_t magnitude_squared[FFT_LEN / 2]; // Buffer to hold the magnitude squared values
        arm_cmplx_mag_squared_q15(freq_domain, magnitude_squared, FFT_LEN / 2); // Calculate the magnitude squared
        // arm_cmplx_mag_squared_q15(complex_buffer, magnitude_squared, FFT_LEN / 2);
        // above function implements 1.15 format, but output is converted to 3.13 format - So convert back to 1.15 (Q15) format
        for (int i = 0; i < FFT_LEN / 2; i++) {
            magnitude_squared[i] = magnitude_squared[i] << 2; // Convert Q13 to Q15
        }

        // print the output of the FFT for debugging ---------- DEBUGGING PURPOSES ONLY----------------------
        // for (size_t i = 0; i < FFT_LEN / 2; i++) {
        //     printf("Magnitude Squared[%zu]: %d\n", i, magnitude_squared[i]);
        // }


        // Driving the LEDs based on the magnitude and the BIN mapping defined above.
        // FFT Bins [6,8) to brightness of LED 1, [8,11) to LED 2, and so on. 
        // The notation [x,y) means that the set is inclusive on the lower side and exclusive on the upper side, 
        // for example, the range of FFT bins contributing to the last LED are 349 ≤ i < 513. 
        // Using 0-indexed C arrays, bin 512 is the highest frequency present.
        // if 

    const int THRESHOLD = 7; // Choose a threshold based on testing

    // leds_off(); // Start with all LEDs off

    for (int b = 0; b < 12; b++) {
        int start = fft_bins[b];
        int end   = fft_bins[b + 1];
        int32_t sum = 0;

        for (int i = start; i < end; i++) {
            sum += magnitude_squared[i];
        }

        int32_t avg = sum / (end - start);

        if (avg > THRESHOLD) {
            // Turn LED on with chosen color (e.g., white or band-specific)
            leds_set_rgb(b, 255, 255, 255); // White
        }
        // Else, LED stays off
    }

    update_leds(); // Push LED state




        // // Perform FFT on the microphone data
        // q15_t fft_input[1024];
        // // Convert uint16_t data to q15_t format
        // for (size_t i = 0; i < 1024; i++) {
        //     fft_input[i] = (q15_t)(adc_data[i] - 32768); // Convert to q15_t and center around 0
        // }
        // q15_t fft_output[1024];
        // arm_rfft_q15(&S, fft_input, fft_output); // Perform the FFT
        // // Process the FFT output (for example, calculate magnitude)
        // q15_t magnitude[512]; // Magnitude for the first half of the FFT output
        // for (size_t i = 0; i < 512; i++) {
        //     magnitude[i] = (q15_t)sqrtf((float)(fft_output[i] * fft_output[i] + fft_output[1024 - i - 1] * fft_output[1024 - i - 1]));
        // }
        // // Print the magnitude values for debugging
        // for (size_t i = 0; i < 512; i++) {
        //     printf("Magnitude[%zu]: %d\n", i, magnitude[i]);
        // }

        //         for (int i = 0; i < 12; i++) { // Loop through the 12 LEDs
        //     int16_t bin_value = magnitude_squared[fft_bins[i]]; // Get the magnitude for the corresponding bin
        //     if (bin_value > 0) {
        //         leds_set(i, bin_value); // Set the LED brightness based on the magnitude value
        //     } else {
        //         leds_set(i, 0); // Turn off the LED if the magnitude is zero or negative
        //     }
        // }

    }

    return 0;

}