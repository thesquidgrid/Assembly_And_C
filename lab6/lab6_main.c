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
void run_lab6_part1();
void pushButton1();
void pushButton2();
void run_lab6_part2();
void run_lab6_part3();
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

#define char_perLine 15
#define displayNumOne (uint32_t) 1234567890
#define displayNumTwo (uint16_t) 1234
#define displayNumThree (uint8_t) 123
#define DEBOUNCE 15

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{   
    clock_init_40mhz();
    launchpad_gpio_init();
    seg7_init();
    dipsw_init();
    I2C_init();
    lcd1602_init();
    lcd_clear();
    keypad_init();

    // run_lab6_part1();
    // pushButton2();
    // run_lab6_part2();
    // pushButton2();
    // run_lab6_part3();
    // pushButton2();

    //part 4:
    lcd_clear();
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string("Running Part 4");
    msec_delay(1000);
    lcd_clear();
    
    bool flag = false;
    bool rowFlag = false;
    int8_t count = 0;
    int8_t ascii;
    while(flag == false){

        if(rowFlag == true)){
            lcd_clear();
            count = 0;
            rowFlag = false;
        }

        if(rowFlag == false){
            lcd_set_ddram_addr(LCD_LINE1_ADDR + count);
            count++;
        } else{
            lcd_set_ddram_addr(LCD_LINE2_ADDR + count);
            count++;
        }

        ascii = getkey_pressed();

        hex_to_lcd(ascii);
        msec_delay(200);
        
        if(is_pb_down(PB1_IDX)){
            count = 0;
            lcd_clear();
            msec_delay(DEBOUNCE);
        }

        if(is_pb_down(PB2_IDX)){
            msec_delay(DEBOUNCE);
            lcd_clear();
            lcd_write_string("program stopped");
            flag = true;
        }
    }



    
    




    

    


    

 
 
 // Endless loop to prevent program from ending
 while (1);

} /* main */

void run_lab6_part1(){
    for(char letter = 'A'; letter <= 'Z'; letter++){
        msec_delay(50); //delay for visibility :)
        lcd_write_char(letter);
        if(letter=='A'+char_perLine){
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
        }
    }
}

void pushButton1(){
    while(is_pb_down(PB1_IDX));
    msec_delay(DEBOUNCE);
    while(is_pb_up(PB1_IDX));
    msec_delay(DEBOUNCE);
}

void pushButton2(){
    while(is_pb_down(PB2_IDX));
    msec_delay(DEBOUNCE);
    while(is_pb_up(PB2_IDX));
    msec_delay(DEBOUNCE);
}

void run_lab6_part2(){
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
    pushButton2();
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_quadbyte(displayNumTwo);
    pushButton2();
    lcd_clear();
    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_7);
    lcd_write_byte(displayNumThree);
    pushButton2();
    lcd_clear();
    lcd_write_string("Part 2 Done.");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string("Press PB2");
}

void run_lab6_part3(){
    lcd_clear();
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string("Running Part 3");
    msec_delay(1000);
    lcd_clear();
    
    bool flag = false;
    int8_t count = 100;
    while(flag ==false){
        
        lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_7);
        lcd_write_byte(count);
        count--;
        msec_delay(200);
        if(count < 0){
            count = 100;
        }

        if(is_pb_down(PB1_IDX)){
            msec_delay(DEBOUNCE);
            count = 100;
        }

        if(is_pb_down(PB2_IDX)){
            msec_delay(DEBOUNCE);
            flag = true;
        }
    }

    lcd_clear();
    lcd_write_string("Part 3 Done");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string("Press PB2");
}