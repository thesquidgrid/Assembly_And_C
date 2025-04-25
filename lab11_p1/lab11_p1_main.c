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
//    This program controls the SPI interface and updates LEDS attached to a
//    SNx4HC595 8-Bit Shift Register. 
//*****************************************************************************
//*****************************************************************************

#include "spi.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"

#include <stdbool.h>
#include <stdio.h>


#include "clock.h"
#include "LaunchPad.h"
#include "lcd1602.h"
#include "uart.h"
#include <math.h>
#include <ti/devices/msp/msp.h>
#include <ti/devices/msp/peripherals/hw_iomux.h>

void msp_printf(char *format, unsigned int value);
bool is_valid_option(char c);
void printMenu(void);
int string_to_uint16(char string[]);

int main(void) {
   clock_init_40mhz();
   launchpad_gpio_init();
   I2C_init();
   lcd1602_init();
   UART_init(115200);
   spi1_init();
   //set initial value to 0
   spi1_write_data(0);
   spi1_read_data();
    // Configure CS pin
   IOMUX->SECCFG.PINCM[LP_SPI_CS0_IOMUX] = (IOMUX_PINCM_PC_CONNECTED |
                      IOMUX_PINCM23_PF_GPIOB_DIO06);

   GPIOB->DOE31_0 |= GPIO_DOE31_0_DIO6_ENABLE;
   GPIOB->DOUT31_0 &= ~GPIO_DOE31_0_DIO6_MASK;

   char currentChar = ' ';
   bool endflag = false;
   int dataSend_int = 0;

   lcd_clear();
   lcd_write_string("Program Running");
   msec_delay(1000);
   lcd_clear();

   lcd_write_string("SEND: ");
   lcd_set_ddram_addr(0x40);
   lcd_write_string("RECV: ");
   

   while (!endflag) {
      printMenu();
      
      currentChar = UART_in_char();
      
      if(currentChar == '4'){
        endflag = true;
      }
      UART_out_char(currentChar);  // Echo input

      while (!is_valid_option(currentChar)) {
         msp_printf("\nInvalid Input. Try again: ", 0);
         currentChar = UART_in_char();
         UART_out_char(currentChar);
      }

      switch (currentChar) {
         case '1': {
            char buffer[4] = {0};
            int counter = 0;
            char input_char;

            msp_printf("\nEnter a value between 0-255: ", 0);

            while (counter < 3) {
               input_char = UART_in_char();
               if (input_char == enter) break;
               UART_out_char(input_char);
               buffer[counter++] = input_char;
            }
            buffer[counter] = '\0';

            dataSend_int = string_to_uint16(buffer);

            if (dataSend_int > 255) {
                dataSend_int = 0;
                msp_printf("\nERROR: Invalid data entered.", 0);
                msp_printf("\nSPI data set to 0.", 0);
            } else {
                msp_printf("\nData to send set to: %d", dataSend_int);
            }

            break;
         }

         case '2': {
            spi1_write_data(dataSend_int);
            uint8_t dataReceivedBack = spi1_read_data();
            lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_7);
            lcd_write_byte(dataSend_int);
            lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_7);
            lcd_write_byte(dataReceivedBack);
            break;
         }

         case '3':
            GPIOB->DOUT31_0 |= GPIO_DOE31_0_DIO6_MASK;
            msec_delay(50);
            GPIOB->DOUT31_0 &= ~GPIO_DOE31_0_DIO6_MASK;
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
//    char* format - Format string containing the message to print.
//    unsigned int value - Value to format into the string.
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
//-----------------------------------------------------------------------------
void msp_printf(char *format, unsigned int value) {
   char string[80];
   int len = sprintf(string, format, value);

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

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Converts a numeric string into an unsigned 16-bit integer.
//
// INPUT PARAMETERS:
//    char string[] - Input string of digits.
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    int - Parsed integer.
//-----------------------------------------------------------------------------
int string_to_uint16(char string[]) {
    int result = 0;

    while (*string != NULL) {
        if (*string >= '0' && *string <= '9') {
            result = result * 10 + (*string - '0');
        } else {
            return 0;  // Invalid character
        }
        string++;
    }
    return result;
}


char* uint16_to_string(int val) {
    static char value[6];  
    int currentDigit = 0;
    int counter = 0;

    while (val != 0) {
        currentDigit = val % 10;
        val = val - currentDigit;
        val = val / 10;
        value[counter] = currentDigit + '0'; 
        counter++;
    }

    value[counter] = '\0';  
    
    for (int i = 0; i < counter / 2; i++) {
        char temp = value[i];
        value[i] = value[counter - i - 1];
        value[counter - i - 1] = temp;
    }
    
    return value;
}