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
#include <cstdint>

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
bool is_valid_option(char c);
int celsius_to_fahrenheit(int temp);
void printMenu(void);
void flashLEDS(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define enter (char) '/r'
#define backspace 8

int main_uart_lcd_demo(void) {
   clock_init_40mhz();
   launchpad_gpio_init();

   I2C_init();
   lcd1602_init();
   UART_init(115200);
   led_init();
   seg7_init();
   ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);
   char currentChar = ' ';
   uint8_t seg7Counter = 0;
   uint32_t temp = 0;

   seg7_hex(seg7Counter, 0);

   while (currentChar != '4') {

      printMenu();
      currentChar = UART_in_char();
      UART_out_char(currentChar);
      while (!is_valid_option(currentChar)) {
         msp_printf("\nIncorrect Input, please try again :) : \r ", 0);
         currentChar = UART_in_char();
         UART_out_char(currentChar);
      }

      // echo character back to UART 

      switch (currentChar) {

      case ('1'):
         seg7Counter++;
         seg7_hex(seg7Counter, 0);
         break;

      case ('2'):
         temp = ADC0_in(ADC12_MEMCTL_CHANSEL_CHAN_5);
         temp = celsius_to_fahrenheit(temp);
         lcd_set_ddram_addr(LCD_LINE_NUM_1);
         lcd_write_string("Temp: ");
         lcd_write_byte(temp);
         lcd1602_write(LCD_IIC_ADDRESS, 0xDF, LCD_DATA_REG); //displays little dot
         break;

      case ('3'):
         flashLEDS();

         break;

      case ('4'):
            msp_printf("\nProgram End\r ", 0);
         break;

      default:
            msp_printf("\nIncorrect Input, please try again :) : \r ", 0);
         break;

      }
   } /* while */
   lcd_clear();
   lcd_set_ddram_addr(LCD_LINE_NUM_1);
   lcd_write_string("Program Done");
   while (1);
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

bool is_valid_option(char c) {
   return c == '1' || c == '2' || c == '3' || c == '4';
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Converts a temperature from Celsius to Fahrenheit.
//    Uses the formula: (temp * 9/5) + 32.
//
// INPUT PARAMETERS:
//    int temp - Temperature in Celsius.
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    int - Temperature in Fahrenheit.
// -----------------------------------------------------------------------------
int celsius_to_fahrenheit(int temp) {
   return ((temp * 9.0 / 5.0) + 32.0);
}

void printMenu(void) {
   msp_printf("MENU\r\n\r\n", 0);
   msp_printf("\t1. Increment count on 7-segment display\r\n", 0);
   msp_printf("\t2. Show current temperature \r\n", 0);
   msp_printf("\t3. Flash LEDs 3 times  \r\n", 0);
   msp_printf("\t3. 4. End Program   \r\n", 0);
   msp_printf("Enter your selection:\r ", 0);
}

void flashLEDS(void) {
   led_enable();
   leds_on(255);
   msec_delay(1000);
   leds_off();
   msec_delay(1000);
   leds_on(255);
   msec_delay(1000);
   leds_off();
   msec_delay(1000);
   leds_on(255);
   msec_delay(1000);
   leds_off();
   msec_delay(1000);
   led_disable();
}