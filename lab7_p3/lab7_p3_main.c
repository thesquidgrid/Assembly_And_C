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
#include "lcd1602.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------

void SysTick_Handler();
#define MSPM0_CLOCK_FREQUENCY (40e6)
#define SYST_TICK_PERIOD (10.25E-3)
#define SYST_TICK_PERIOD_COUNT (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define NUM_STATES 5
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


const uint8_t seg7_number_code[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99
 
};

const uint8_t delay_count[] = {
    0x18, 0x18, 0x18, 0x18, 0x18
};

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{

   clock_init_40mhz();
   launchpad_gpio_init();
   led_init();
   seg7_init();
   I2C_init();
   lcd1602_init();
   lcd_clear();
   dipsw_init();

   sys_tick_init(SYST_TICK_PERIOD_COUNT);
   SysTick_Handler();
 
 // Endless loop to prevent program from ending
 while (1);

} /* main */


//////////////////////////////////////////////////////
// DESCRIPTION:
//	This function represents the ISR (Interrupt Service Routine) for
//	the sysTick timer. It is called at regular intervals based on the
//	configured SysTick period. This ISO is responsible for managing the
//	timing and display of the Morse code for .SOS. on the seven-segment
//	display. It controls the blink timing for the letters, with quick
//	blinks for "S. (dot-dot-dot) and slower blinks for "0" (dash-dash-dash).
//	The sequence of letters is repeated indefinitely.
//
// INPUT PARAMETERS:
// none
//
// OUTPUT PARAMETERS:
// none
//
// RETURN:
// none
///////////////////////////////////////////////
void SysTick_Handler(void) {
   static uint16_t delay_time = 1;
   static uint16_t code_index = 0;

   delay_time--;
   if (delay_time == 0) {
      seg7_on(seg7_number_code[code_index], SEG7_DIG0_ENABLE_IDX);

      delay_time = delay_count[code_index];
      code_index++;

      if (code_index == NUM_STATES) {
         code_index = 0;
      }
   }
}
