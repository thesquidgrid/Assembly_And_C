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
void run_lab7_part3();
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
    0, 1, 2, 3, 4
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
   leds_off();
   
   sys_tick_init(SYST_TICK_PERIOD_COUNT);
   SysTick_Handler();
   run_lab7_part3();

   sys_tick_disable();
   seg7_disable();
   led_disable();

 
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
      int switchup = dipswitch_up();
      seg7_hex(switchup, 0);

      delay_time = delay_count[code_index];
      code_index++;
      
      if (code_index == NUM_STATES) {
         code_index = 0;
      }
   }
}


void run_lab7_part3() {
   lcd_clear();
   lcd_set_ddram_addr(LCD_LINE1_ADDR);
   lcd_write_string("Running Part 3");
   msec_delay(1000);
   lcd_clear();

   bool flag = false;
   int8_t count = 0;
   while (flag == false) {

      lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_7);
      lcd_write_byte(count);
      count++;
      msec_delay(200);
      if (count > 99) {
         count = 0;
      }

      if (is_pb_down(PB2_IDX)) {
         msec_delay(15);
         flag = true;
      }
   }

   lcd_clear();
   lcd_write_string("Part 3 Done");
   lcd_set_ddram_addr(LCD_LINE2_ADDR);
   lcd_write_string("Press PB2");
}