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

int main_uart_lcd_demo(void) {
   char character;
   // Configure the launchpad boards clock_init_40mhz(); launchpad_gpio_init(); 
   // Configure I2C 
   I2C_init();
   // Configure LCD 
   lcd1602_init();
   // Configure the UART 
   UART_init(115200);
   // Create an endless loop for demonstration purposes 
   while (1) {
      // wait for character to enter UART character 
      UART_in_char();
      // echo character back to UART 
      UART_out_char(character);
      // Write character to LCD 
      lcd_write_char(character); 
   } /* while */
} /* main */