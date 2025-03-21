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
void run_lab7_part1();
void SysTick_Handler();
void lcd_string_parser(const char* string, uint8_t start_lcd_addr,
                       uint8_t max_lcd_addr);
void run_lab7_part2();

#define MSPM0_CLOCK_FREQUENCY (40e6)
#define SYST_TICK_PERIOD (10.25E-3)
#define SYST_TICK_PERIOD_COUNT (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define NUM_STATES 18
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
const uint8_t seg7_letter_code[] = {
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00,
    // s
    0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x00,
    // 0
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00
    // s
};

const uint8_t delay_count[] = {
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x24, 0x24, 0x24, 0x24,
    0x24, 0x24, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x30
    // dots
    // dash
    // dots
};
// These timings for morse code dots and dashes can vary slightly based on
// different implementations, but the 1:3 ratio for dots:dashes is a
// typuiical ration for Morse code. The delay times in terms of number
// SysTick interrupts.

// Define a structure to hold different data types

int main(void) {
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

   run_lab7_part1();
   while (!is_pb_down(PB1_IDX));  // wait for pb1 to be down

   sys_tick_disable();
   seg7_off();
   lcd_clear();

   lcd_set_ddram_addr(0x00);
   lcd_write_string("Press PB2");
   while (!is_pb_down(PB2_IDX));  // wait for pb2
   run_lab7_part2();
   
   lcd_clear();
   lcd_set_ddram_addr(0x00);
   lcd_write_string("Press PB2");
   while (!is_pb_down(PB2_IDX));  // wait for pb2

   // Endless loop to prevent program from ending
   while (1);

} /* main */

void run_lab7_part1() {
   bool done = false;
   while (!done) {
      int indx = 0;

      char msg[] = "Microcontrollers are fun";

      for (int led_add = 0x4F; ((led_add > 0x40) && (!done)); led_add--) {
         lcd_clear();
         lcd_set_ddram_addr(led_add);
         lcd_write_string(msg);
         msec_delay(100);
         if (is_pb_down(PB1_IDX)) {
            done = true;
         }
      }
      int indx2 = 1;
      int address = 0x40;

      while ((msg[indx] != '\0') && (!done)) {
         lcd_clear();
         lcd_set_ddram_addr(address);
         lcd_write_string(msg + indx);
         msec_delay(100);

         indx++;
         if (is_pb_down(PB1_IDX)) {
            done = true;
         }
      }
   }
}

void run_lab7_part2() {
   char* msg =
       "Microcontrollers are fun. I love programming in MSPM0+ assembly "
       "code!!!";
   int led_add = 0x4F;
   int index = 0;
   int index2 = 0;
   bool done = false;

   while ((msg[index] != '\0') && (!done)) {
      index++;

      if (index > 16) {
         lcd_clear();
         lcd_set_ddram_addr(0x40);
         index2++;
         lcd_string_parser(msg + index2, 0x40, 0x50);
         msec_delay(50);

         if (is_pb_down(PB2_IDX)) {
            done = true;
         }

      } else {
         lcd_clear();
         lcd_set_ddram_addr(led_add);

         lcd_string_parser((msg), 0x40, 0x50);
         msec_delay(100);

         led_add--;

         if (is_pb_down(PB2_IDX)) {
            done = true;
         }
      }
   }
}

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
      seg7_on(seg7_letter_code[code_index], SEG7_DIG0_ENABLE_IDX);

      delay_time = delay_count[code_index];
      code_index++;

      if (code_index == NUM_STATES) {
         code_index = 0;
      }
   }
}

void lcd_string_parser(const char* string, uint8_t start_lcd_addr,
                       uint8_t max_lcd_addr) {
   uint8_t maxCount = max_lcd_addr - start_lcd_addr;
   uint8_t count = 0;
   // for each character in string, write it to the LCD module

   while ((*string != '\0') && (count < maxCount)) {
      lcd_write_char(*string++);
      count++;
   } /* while */

} /* lcd_string_parser */