//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Sophia Buchman
//
//       LAB NAME:  lab 10 part 2
//
//      FILE NAME:  lab10_p2_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program runs a menu-driven interface on the MSPM0G3507 microcontroller. 
//    It allows the user to interact via UART to control a 7-segment display, read temperature data, and flash LEDs. 
//    The program loops until the user chooses to exit.
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------

#include "spi.h"

#include <stdbool.h>

#define enter      '\r'

#include "clock.h"
#include "LaunchPad.h"
#include "lcd1602.h"
#include "uart.h"

void msp_printf(char * buffer, unsigned int value);
bool is_valid_option(char c);
void printMenu(void);


int main(void) {
   clock_init_40mhz();
   launchpad_gpio_init();
   I2C_init();
   lcd1602_init();
   UART_init(115200);
   spi1_init();
   spi1_write_data(255);
   spi1_read_data();

   char currentChar = ' ';
   uint8_t seg7Counter = 0;
   uint32_t temp = 0;
   char dataSend_char = 'o';
   int dataSend_int = 0;
   seg7_hex(seg7Counter, 0);

   while (currentChar != '4') {
      printMenu();
      
      currentChar = UART_in_char();
      UART_out_char(currentChar);  // Echo input

      while (!is_valid_option(currentChar)) {
         msp_printf("\nInvalid Input. Try again: ", 0);
         currentChar = UART_in_char();
         UART_out_char(currentChar);
      }

      switch (currentChar) {
         case '1':
            msp_printf("\nWhat value would you like to input (between 0-255):\t", 0);
            
            uint8_t counter = 0;
            while((dataSend_char != enter) || (counter > 2)){
              dataSend_char = UART_in_char();
              UART_out_char(dataSend_char); 
              dataSend_int = (dataSend_char - '0') * (10^counter);
              counter++;
            }

            msp_printf("\n", 0);
        
            // while(dataSend_int < 0 || dataSend_int > 255){
            //     msp_printf("\nInvalid Input. Try again: ", 0);
            //     dataSend = UART_in_char();
            // }
            break;

         case '2':
            spi1_write_data(dataSend_int);
            break;

         case '3':
            spi1_read_data();
            break;

         case '4':
            msp_printf("\nProgram End\r\n", 0);
            break;
      }
   }

   lcd_clear();
   lcd_set_ddram_addr(LCD_LINE_NUM_1);
   lcd_write_string("Program Done");

   while (1);
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Prints a formatted string via UART using sprintf and UART_out_char.
//
// INPUT PARAMETERS:
//    char* buffer - Format string containing the message to print.
//    unsigned int value - Value to format into the string.
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
//-----------------------------------------------------------------------------
void msp_printf(char * buffer, unsigned int value) {
   char string[80];
   int len = sprintf(string, buffer, value);

   for (int i = 0; i < len; i++) {
      UART_out_char(string[i]);
   }
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Validates if the input character is a valid menu option ('1' to '4').
//
// INPUT PARAMETERS:
//    char c - Character to validate.
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    bool - true if valid option, false otherwise.
//-----------------------------------------------------------------------------
bool is_valid_option(char c) {
   return c == '1' || c == '2' || c == '3' || c == '4';
}


//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Displays the menu options to the user via UART.
//    Prompts the user to select an option between 1 and 4.
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
//-----------------------------------------------------------------------------
void printMenu(void) {
   msp_printf("\r\n======== MENU ========\r\n", 0);
   msp_printf(" 1. Set Data to Send\r\n", 0);
   msp_printf(" 2. Send Data\r\n", 0);
   msp_printf(" 3. Update LED's\r\n", 0);
   msp_printf(" 4. End Program\r\n", 0);
   msp_printf("Enter your selection: ", 0);
}
