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
#include <stdio.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>

#include "LaunchPad.h"

#include "clock.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab5_part1();
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

int main(void) {
   clock_init_40mhz();
   launchpad_gpio_init();
   lp_leds_init();
   led_init();
   led_enable();
   led_disable();
   seg7_init();
   dipsw_init();

   uint8_t loopCount = 0;
   bool hexON = false;
   bool pushButtonOneDown = false;
   run_lab5_part1();

   // Endless loop to prevent program from ending
   while (1);

} /* main */


void run_lab5_part1() {

   uint8_t loopCount = 0;
   bool hexON = false;
   bool pushButtonOneDown = false;

   while (loopCount < 3) {

      if (is_pb_down(PB1_IDX)) {
         if (hexON == false) {
            seg7_hex(3, 0);
            msec_delay(10);
            hexON = true;
         } else {
            hexON = false;
            seg7_off();
            loopCount++;
         }

         while (is_pb_down(PB1_IDX)) {
            msec_delay(15);
         }

      }

   }
}