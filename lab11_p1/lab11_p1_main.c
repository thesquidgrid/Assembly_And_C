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
#include <math.h>

void msp_printf(char * buffer, unsigned int value);
bool is_valid_option(char c);
void printMenu(void);
int power(int base, unsigned int exp);


int main(void) {
   clock_init_40mhz();
   launchpad_gpio_init();
   I2C_init();
   lcd1602_init();
   UART_init(115200);
   spi1_init();
   spi1_write_data(3);
   spi1_read_data();

   char currentChar = ' ';
   uint8_t seg7Counter = 0;
   uint32_t temp = 0;
   char dataSend_char = 'o';
   int dataSend_int = 0;
   lcd_write_string("SEND: ");
   lcd_set_ddram_addr(0x40);
   lcd_write_string("RECV: ");
  

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
            //reset values.
            dataSend_char = 'm';
            dataSend_int = 0;

                        
            msp_printf("\nWhat value would you like to input (between 0-255):\t", 0);
            
            int counter = 2;
            while((dataSend_char != enter) && (counter >= 0)){
              dataSend_char = UART_in_char();
              UART_out_char(dataSend_char);

              if(dataSend_char != enter){
                uint8_t multiplier = power(10, counter);
                //add string to uint16 here
                uint8_t inputVal = (dataSend_char - '0'); //converts from ascii to integer.
                dataSend_int = (inputVal * multiplier) +  dataSend_int;
                counter--;
              }
            }

            if(dataSend_char == enter){
                int multiplier = power(10, counter + 1);
                dataSend_int = dataSend_int / multiplier;
            }

            if (dataSend_int > 255) {
                dataSend_int = 0;
                msp_printf("\nERROR: invalid data entered.", 0);
                msp_printf("\nSPI data set to 0.", 0);
            }

            msp_printf("\n", 0);
            
            break;

         case '2':
            GPIOA->DOUT31_0[] //look at launchpad.c on hwo to do this. need RCLK strobbed low
            spi1_write_data(dataSend_int);
            lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_5);
            lcd_write_byte(dataSend_int);
            uint8_t dataRecivedBack = spi1_read_data();
            lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_5);
            lcd_write_byte(dataRecivedBack);
            
            break;

         case '3':
                        GPIOA->DOUT31_0[] //look at launchpad.c on hwo to do this. need RCLK strobbed high
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


// from https://stackoverflow.com/questions/213042/how-do-you-do-exponentiation-in-c
int power(int base, unsigned int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
 }
