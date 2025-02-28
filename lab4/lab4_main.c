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
//
// DESCRIPTION:
//    This program serves as an introduction to controlling GPIO pins on the 
//    Launch Pad.
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
/*  #include <atomic> */
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

void run_lab4_part2();
void run_lab4_part1();
void run_lab4_part3();
void run_lab4_part4();
void run_lab4_part5();
void run_lab5_part6();

//Start of Main

int main(void) {
    //start up needed periferals 
    clock_init_40mhz();
    launchpad_gpio_init();
    lp_leds_init();
    led_init();
    led_enable();
    // end of start up

    run_lab4_part1();
    msec_delay(1000);
    run_lab4_part2();
    run_lab4_part3();
    msec_delay(500);

    //initialize 7 seg.
    seg7_init();

    run_lab4_part4();
    msec_delay(500);

    run_lab4_part5();
    run_lab5_part6();

    //turn off LED perfieralls
    leds_off();

    while (1); //prevents issues with exit.c
} //end of main


void run_lab4_part1() {

    for (uint8_t i = 0; i < 8; i++) {
        led_on(i); // Turn on the current LED
        msec_delay(500); // Delay for visibility
    }

}

void run_lab4_part2() {

    int8_t counter = 4;

    while (counter >= 0) {
        // Loop through all 8 LEDs, turning them on one at a time
        for (uint8_t i = 0; i < 6; i++) {
            leds_off(); // Turn off all LEDs
            led_on(i); // Turn on the current LED
            msec_delay(200); // Delay for visibility
        }

        for (int8_t i = 6; i >= 0; i--) {

            leds_off(); // Turn off all LEDs
            led_on(i); // Turn on the current LED
            msec_delay(200); // Delay for visibility

        }

        leds_off();
        counter--;
    }
    // seg7_off();
}

void run_lab4_part3() {

    uint8_t counter = 0;
    uint8_t loopCounter = 0;

    while (loopCounter < 2) {

        leds_on((uint32_t) counter);
        counter++;
        msec_delay(100);
        leds_off();

        if (counter == 255) {
            loopCounter++;
        }

    }

}

void run_lab4_part4() {
    led_disable(); //disables LED 7:0
    seg7_hex(16, 0); //Added L to list of possible char's. Displays on DIG0
    msec_delay(2000);

    leds_off();
}

void run_lab4_part5() {

    uint8_t counter = 0;
    while (counter < 4) {

        seg7_hex(4, 1);

        msec_delay(2000);
        seg7_off();
        msec_delay(3000);
        counter++;
    }

}

void run_lab5_part6() {
    uint8_t counter = 0;
    led_disable();
    while (counter < 200) {

        seg7_hex(12, 0);
        msec_delay(5);
        seg7_off();
        seg7_hex(10, 1);
        msec_delay(5);
        seg7_off();
        seg7_hex(15, 2);
        msec_delay(5);
        seg7_off();
        seg7_hex(14, 3);
        msec_delay(5);
        seg7_off();

        counter++;
    }

}

