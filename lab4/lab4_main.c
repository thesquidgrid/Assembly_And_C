//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Sophia Buchman
//
//       LAB NAME:  Lab 4
//
//      FILE NAME:  lab4_main.c
//
//----------------------------------------------------------------------------- 
// DESCRIPTION:
//    This program serves as an introduction to controlling GPIO pins on the 
//    Launch Pad.
//*****************************************************************************
//*****************************************************************************

//----------------------------------------------------------------------------- 
// Loads standard C include files
//----------------------------------------------------------------------------- 
#include <stdio.h>

//----------------------------------------------------------------------------- 
// Loads MSP launchpad board support macros and definitions
//----------------------------------------------------------------------------- 
#include <ti/devices/msp/msp.h>
#include "clock.h"
#include "LaunchPad.h"
#include "uart.h"

//----------------------------------------------------------------------------- 
// Define function prototypes used by the program
//----------------------------------------------------------------------------- 
void run_lab4_part1();
void run_lab4_part2();
void run_lab4_part3();
void run_lab4_part4();
void run_lab4_part5();
void run_lab5_part6();

//----------------------------------------------------------------------------- 
// FUNCTION: main
// DESCRIPTION:
//    Initializes the necessary peripherals and executes the different lab 
//    functions sequentially.
//----------------------------------------------------------------------------- 
int main(void) {
    // Initialize system clock and GPIO peripherals
    clock_init_40mhz();
    launchpad_gpio_init();
    lp_leds_init();
    led_init();
    led_enable();
    seg7_init();

    //Lab code:
    
     run_lab4_part1();
    // msec_delay(1000); //pause for visibility
    // run_lab4_part2();
    // run_lab4_part3();
    // msec_delay(500);
    
    
    run_lab4_part4();
    msec_delay(500);
    
    run_lab4_part5();
    run_lab5_part6();

    // Turn off all LEDs
    leds_off();
    
    while (1); // prevents exit.c issues
}

//----------------------------------------------------------------------------- 
// FUNCTION: run_lab4_part1
// DESCRIPTION:
//    Turns on LEDs sequentially from LED0 to LED7, pausing briefly for 
//    visibility.
//----------------------------------------------------------------------------- 
void run_lab4_part1() {
    seg7_off(); //Turn off seg7 display
    for (uint8_t i = 0; i < 8; i++) {
        led_on(i); // Turn on the current LED
        msec_delay(500); // Delay for visibility
    }
}

//----------------------------------------------------------------------------- 
// FUNCTION: run_lab4_part2
// DESCRIPTION:
//    Loops through LEDs in a forward and backward sequence, repeating the 
//    pattern a set number of times.
//----------------------------------------------------------------------------- 
void run_lab4_part2() {
    int8_t counter = 4;

    while (counter >= 0) {
        // Forward LED sequence
        for (uint8_t i = 0; i < 6; i++) {
            leds_off(); // Turn off all LEDs
            led_on(i); // Turn on the current LED
            msec_delay(200);
        }
        // Reverse LED sequence
        for (int8_t i = 6; i >= 0; i--) {
            leds_off(); // Turn off all LEDs
            led_on(i); // Turn on the current LED
            msec_delay(200);
        }
        leds_off();
        counter--;
    }
}

//----------------------------------------------------------------------------- 
// FUNCTION: run_lab4_part3
// DESCRIPTION:
//    Cycles through zero to 255, then resets after a full cycle.
//----------------------------------------------------------------------------- 
void run_lab4_part3() {
    uint8_t counter = 0;
    uint8_t loopCounter = 0;

    while (loopCounter < 2) {
        leds_on((uint32_t) counter); // Light up LEDs based on counter value
        counter++;
        msec_delay(100);
        leds_off();
        
        if (counter == 255) {
            loopCounter++;
        }
    }
}

//----------------------------------------------------------------------------- 
// FUNCTION: run_lab4_part4
// DESCRIPTION:
//    Displays 'L' on the 7-segment display and disables LEDs.
//----------------------------------------------------------------------------- 
void run_lab4_part4() {
    led_disable(); // Disable all LEDs
    seg7_on(0x38,0);
    msec_delay(2000);
    leds_off();
}

//----------------------------------------------------------------------------- 
// FUNCTION: run_lab4_part5
// DESCRIPTION:
//    Flashes a digit on the 7-segment display repeatedly with delays in between.
//----------------------------------------------------------------------------- 
void run_lab4_part5() {
    uint8_t counter = 0;
    while (counter < 4) {
        seg7_hex(4, 1); // Display digit 4 on segment 1
        msec_delay(2000);
        seg7_off(); // Turn off display
        msec_delay(3000);
        counter++;
    }
}

//----------------------------------------------------------------------------- 
// FUNCTION: run_lab5_part6
// DESCRIPTION:
//    Rapidly cycles through digits on a 7-segment display for 200 iterations,
//    Creating a "unified" effect.
//----------------------------------------------------------------------------- 
void run_lab5_part6() {
    uint8_t counter = 0;
    led_disable(); // Disable LEDs
    
    while (counter < 200) {
        seg7_hex(12, 0); // Display 'C' on DIG0
        msec_delay(5);
        seg7_off();
        seg7_hex(10, 1); // Display 'A' on DIG1
        msec_delay(5);
        seg7_off();
        seg7_hex(15, 2); // Display 'F' on DIG2
        msec_delay(5);
        seg7_off();
        seg7_hex(14, 3); // Display 'E' on DIG3
        msec_delay(5);
        seg7_off();
        counter++; 
    }
}
