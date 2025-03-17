//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Sophia Buchman
//
//       LAB NAME:  lab6_main.c
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//   This program runs multiple lab exercises on an MSP microcontroller, utilizing
//   an LCD display, push buttons, and a keypad. It sequentially executes different
//   functionalities, including displaying characters, handling push-button input,
//   performing a countdown, and processing keypad input. The program ensures proper
//   debouncing for buttons and interacts with various peripherals such as a
//   7-segment display, LEDs, and I2C communication.
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
void run_lab6_part1();
void pushButton_release(uint8_t index);
void run_lab6_part2();
void run_lab6_part3();
void run_lab6_part4();
int8_t padPress();
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

#define char_perLine 15
#define displayNumOne (uint32_t) 1234567890
#define displayNumTwo (uint16_t) 1234
#define displayNumThree (uint8_t) 123
#define DEBOUNCE 15

// start of program
int main(void) {
   clock_init_40mhz();
   launchpad_gpio_init();
   seg7_init();
   dipsw_init();
   I2C_init();
   lcd1602_init();
   lcd_clear();
   keypad_init();

   lcd_write_string("     WINNER");

   run_lab6_part1();
   pushButton_release(PB2_IDX);
   run_lab6_part2();
   pushButton_release(PB2_IDX);
   run_lab6_part3();
   pushButton_release(PB2_IDX);
   run_lab6_part4();

   // Endless loop to prevent program from ending
   while (1);

} /* main */

// -----------------------------------------------------------------------------
// DESCRIPTION
//    This function sequentially displays the letters A to Z on the LCD with a 
//    delay for visibility. Once the first line reaches its character limit, 
//    the function moves the cursor to the second line.
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void run_lab6_part1() {
   for (char letter = 'A'; letter <= 'Z'; letter++) {
      msec_delay(50); //delay for visibility :)
      lcd_write_char(letter);
      if (letter == 'A' + char_perLine) {
         lcd_set_ddram_addr(LCD_LINE2_ADDR);
      }
   }
}

// -----------------------------------------------------------------------------
// DESCRIPTION
//    This function handles debouncing for a push button. It waits until the 
//    button is released to ensure a stable input. The function introduces a 
//    delay before and after detecting the button release. for debouncing purposes
//
// INPUT PARAMETERS:
//    index - The index of the push button being checked.
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void pushButton_release(uint8_t index) {
   while (is_pb_down(index));
   msec_delay(DEBOUNCE);
   while (is_pb_up(index));
   msec_delay(DEBOUNCE);
}

// -----------------------------------------------------------------------------
// DESCRIPTION
//    This function disables specific peripherals, displays predefined numeric 
//    values on the LCD, and waits for a button press between each display update. 
//    It ends by displaying a message prompting the user to press PB2.
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------

void run_lab6_part2() {
   //make sure certain peripherals are disabled:
   led_disable();
   seg7_disable();

   //start of function
   lcd_clear();
   lcd_write_string("Running Part 2");
   msec_delay(1000);
   lcd_clear();
   lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_4);
   lcd_write_quadbyte(displayNumOne);
   pushButton_release(PB2_IDX);
   lcd_set_ddram_addr(LCD_LINE2_ADDR);
   lcd_write_quadbyte(displayNumTwo);
   pushButton_release(PB2_IDX);
   lcd_clear();
   lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_7);
   lcd_write_byte(displayNumThree);
   pushButton_release(PB2_IDX);
   lcd_clear();
   lcd_write_string("Part 2 Done.");
   lcd_set_ddram_addr(LCD_LINE2_ADDR);
   lcd_write_string("Press PB2");
}
// -----------------------------------------------------------------------------
// DESCRIPTION
//    This function performs a countdown from 100 to 0, looping back to 100 
//    when it reaches zero. The countdown speed is controlled by a delay. 
//    Pressing PB1 resets the count to 100, and pressing PB2 exits the loop.
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void run_lab6_part3() {
   lcd_clear();
   lcd_set_ddram_addr(LCD_LINE1_ADDR);
   lcd_write_string("Running Part 3");
   msec_delay(1000);
   lcd_clear();

   bool flag = false;
   int8_t count = 100;
   while (flag == false) {

      lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_7);
      lcd_write_byte(count);
      count--;
      msec_delay(200);
      if (count < 0) {
         count = 100;
      }

      if (is_pb_down(PB1_IDX)) {
         msec_delay(DEBOUNCE);
         count = 100;
      }

      if (is_pb_down(PB2_IDX)) {
         msec_delay(DEBOUNCE);
         flag = true;
      }
   }

   lcd_clear();
   lcd_write_string("Part 3 Done");
   lcd_set_ddram_addr(LCD_LINE2_ADDR);
   lcd_write_string("Press PB2");
}

// -----------------------------------------------------------------------------
// DESCRIPTION
//    This function handles user input from a keypad and displays characters on 
//    the LCD. The cursor position alternates between two display lines. Pressing 
//    PB1 clears the screen and resets the position, while pressing PB2 stops the 
//    program and displays a termination message.
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
// -----------------------------------------------------------------------------
void run_lab6_part4() {
   lcd_clear();
   lcd_set_ddram_addr(LCD_LINE1_ADDR);
   lcd_write_string("Running Part 4");
   msec_delay(1000);
   lcd_clear();

   bool flag = false;
   bool rowFlag = false;
   int8_t count = 0;
   int8_t ascii = 0;
   while (flag == false) {

      if (is_pb_down(PB1_IDX)) {
         count = 0;
         lcd_clear();
         msec_delay(DEBOUNCE);
      }

      if (is_pb_down(PB2_IDX)) {
         msec_delay(DEBOUNCE);
         lcd_clear();
         lcd_write_string("Program stopped");
         flag = true;
      }

      if (rowFlag == false) {
         lcd_set_ddram_addr(LCD_LINE1_ADDR + count);
         if (count == 16) {
            count = 0;
            rowFlag = true;
         }
      } else {
         lcd_set_ddram_addr(LCD_LINE2_ADDR + count);
         if (count == 16) {
            count = 0;
            rowFlag = false;
         }
      }

      if ((count == 0 && rowFlag == 0)) {
         ascii = keypad_scan();
         if (ascii != 0x10) {
            lcd_clear();
            lcd_set_ddram_addr(0x00);
            padPress();
            count++;
         }

      } else {
         ascii = padPress();
         if (ascii != 0x10) {
            count++;
         }

      }

   }
}

// -----------------------------------------------------------------------------
// DESCRIPTION
//    This function scans for a keypad press, converts the key value to its 
//    corresponding hexadecimal representation, and displays it on the LCD. 
//    It ensures no key is pressed before returning the scanned ASCII value.
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    ascii - The ASCII value of the key pressed on the keypad.
// -----------------------------------------------------------------------------

int8_t padPress() {
   int8_t ascii = keypad_scan();
   if (ascii != 0x10) {
      hex_to_lcd(ascii);
      msec_delay(200);
   }
   wait_no_key_pressed();
   return ascii;
}