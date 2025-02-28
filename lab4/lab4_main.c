//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  TBD
//
//       LAB NAME:  TBD
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a ... 
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

void delay_ms(uint32_t ms);
void part1();
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

#define LED0_PIN    38

#define LED1_PIN    37

#define LED2_PIN    36

#define LED3_PIN    35

#define LED4_PIN    32

#define LED5_PIN    31

#define LED6_PIN    28

#define LED7_PIN    29

#define LED_EN      26


const char letters[] =
{
    0x39,
    0x77,
    0x71,
    0x79
};

int seg7_digit = 0;

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{
    part1();

 }
 
 void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < (ms * 4000); i++);  // Approximate delay
}
 // Endless loop to prevent program from ending
 


 void part1(){
    clock_init_40mhz();
    launchpad_gpio_init();
    lp_leds_init();
    led_init();

    // Enable the LED bar
    led_enable();

    for (uint8_t i = 0; i < 8; i++)
        {
            led_on(i);       // Turn on the current LED
            msec_delay(500);   // Delay for visibility
        }


 }

 void part2(){

    clock_init_40mhz();
    launchpad_gpio_init();
    lp_leds_init();
    led_init();

    // Enable the LED bar
    led_enable();

        // Loop through all 8 LEDs, turning them on one at a time
        for (uint8_t i = 0; i < 8; i++)
        {
            leds_off();      // Turn off all LEDs
            led_on(i);       // Turn on the current LED
            msec_delay(1000);   // Delay for visibility
        }


 }

 /* main */


