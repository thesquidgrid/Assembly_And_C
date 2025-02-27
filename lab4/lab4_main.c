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
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

#define LED1_PIN    31

#define LED2_PIN    20

#define LED3_PIN    13

#define LED4_PIN    10

#define LED5_PIN    11


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
clock_init_40mhz();
//led_enable();
led_init();
seg7_on(2, 0);
seg7_init();
launchpad_gpio_init();



 seg7_init();  // Initialize 7-segment display

    while (1) {
        for (uint8_t num = 0; num <= 9; num++) {
            for (uint8_t digit = 0; digit < 4; digit++) {
                seg7_hex(num, digit);  // Display number on the segment
                delay_ms(250);         // Delay for visibility
            }
        }
    }

 }
 
 void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < (ms * 4000); i++);  // Approximate delay
}
 // Endless loop to prevent program from ending
 

 /* main */


