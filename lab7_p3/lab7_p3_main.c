//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Sophia
//
//       LAB NAME:  lab 7
//
//      FILE NAME:  lab7_p3_main.c.c
//
//----------------------------------------------------------------------------- 
//
// DESCRIPTION:
//    This program configures GPIO interrupts on the MSPM0G3507 microcontroller.
//    It handles button presses using interrupts instead of polling, updates an
//    LCD display, and manages global flags for state changes. The SysTick timer
//    is used to update a seven-segment display. Additionally, a counter is
//    implemented that increments until a button is pressed.
//
//    The program initializes system peripherals, configures interrupts,
//    and enters an infinite loop where the logic is handled via ISRs.
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

//////////////////////////////////////////////////////
// DESCRIPTION:
//  Main function initializes all required peripherals, sets up GPIO,
//  configures SysTick, and then enters an infinite loop.
//  The main logic is managed through interrupts.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//////////////////////////////////////////////////////
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
   
   led_disable();

 // Endless loop to prevent program from ending
 while (1);
} /* main */


//////////////////////////////////////////////////////
// DESCRIPTION:
//  This function represents the ISR (Interrupt Service Routine) for
//  the SysTick timer. It is called at regular intervals based on the
//  configured SysTick period. This ISR is responsible for updating the
//  seven-segment display based on dipswitch inputs. It cycles through
//  predefined states and resets when reaching the end.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////
// DESCRIPTION:
//  Function to execute Lab 7 Part 3 functionality.
//  This function updates the LCD display, counts up to 99,
//  and waits for a button press to exit.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//////////////////////////////////////////////////////
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
