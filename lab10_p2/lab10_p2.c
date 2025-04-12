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

#include "clock.h"

#include "LaunchPad.h"

#include "adc.h"

#include "lcd1602.h"

#include "uart.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void msp_printf(char * buffer, unsigned int value);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define enter(char)
'/r'
#define backspace 8

int main_uart_lcd_demo(void) {
   // Configure the launchpad boards clock_init_40mhz(); launchpad_gpio_init(); 
   // Configure I2C 
   I2C_init();
   // Configure LCD 
   lcd1602_init();
   // Configure the UART 
   UART_init(115200);
   // Create an endless loop for demonstration purposes 
   char currentChar = '';
   uint8_t
   while (currentChar != '4') {
      msp_printf('\r\n', 0);
      msp_printf("MENU\r\n\r\n", 0);
      msp_printf("\t1. Increment count on 7-segment display\r\n", 0);
      msp_printf("\t2. Show current temperature \r\n", 0);
      msp_printf("\t3. Flash LEDs 3 times  \r\n", 0);
      msp_printf("\t3. 4. End Program   \r\n", 0);
      msp_printf("Enter your selection:\r ", 0);
      currentChar = UART_in_char();
      // echo character back to UART 
      UART_out_char(character);
      switch (currentChar) {

      case ('1'):

         break;

      case ('2');

        break;

      case ('3');

        break;

      case ('4');

        break;

      default:

         break;

      }
   } /* while */

   lcd_set_ddram_addr(LCD_LINE_NUM_2);
   lcd_write_string("Program Done");
} /* main */

void msp_printf(char * buffer, unsigned int value) {
   unsigned int i = 0;
   unsigned int len = 0;
   char string[80];

   len = sprintf(string, buffer, value);

   // Walk through arrya to send each character to serial port
   for (i = 0; i < len; i++) {
      UART_out_char(string[i]);
   } /* for */
} /* msp printf */