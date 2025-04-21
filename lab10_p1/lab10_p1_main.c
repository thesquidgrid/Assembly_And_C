//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Sophia
//
//       LAB NAME:  Buchman
//
//      FILE NAME:  lab10_p1_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program takes user input via UART and displays it on an LCD1602 screen. 
//    It supports live character echoing, backspace handling, and ends on Enter key. 
//    Once complete, it clears the display and shows a "Program Done" message.
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------

#include <stdio.h>

#define enter      '\r'
#define backspace  '\b'

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>

#include "clock.h"
#include "LaunchPad.h"
#include "adc.h"
#include "lcd1602.h"
#include "uart.h"

int main(void) {
   // Configure the launchpad boards 
   clock_init_40mhz(); 
   launchpad_gpio_init(); 
   I2C_init();
   lcd1602_init();
   UART_init(115200);

   lcd_write_string("NAME:");

   char currentChar = ' ';
   uint8_t currentPositon = 5;  // Start after "NAME:"
   bool nameEnterComplete_flag = false;

   while (!nameEnterComplete_flag) {
      currentChar = UART_in_char();
      UART_out_char(currentChar);  // Echo back to terminal

      if (currentChar == enter) {
         nameEnterComplete_flag = true;
      } else if (currentChar == backspace) {
         if (currentPositon > 5) {
            currentPositon--;
            lcd_set_ddram_addr(currentPositon + LCD_LINE_NUM_1);
            lcd_write_char(' '); // Erase character on LCD
            lcd_set_ddram_addr(currentPositon + LCD_LINE_NUM_1); // Move cursor back
         }
      } else {
         lcd_set_ddram_addr(currentPositon + LCD_LINE_NUM_1);
         lcd_write_char(currentChar);
         currentPositon++;
      }
   }

   lcd_clear();
   lcd_set_ddram_addr(LCD_LINE_NUM_2);
   lcd_write_string("Program Done");

   while(1);
}
