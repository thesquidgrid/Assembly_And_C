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


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab5_part1();
void run_lab5_part2();
void run_lab5_part3();
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

enum States {
    GET_LOW,
    GET_HIGH,
    DISPLAY
} states;

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

int main(void) {
   clock_init_40mhz();
   launchpad_gpio_init();
   lp_leds_init();
   lpsw_init();
   led_init();
   led_enable();
   led_disable();
   seg7_init();
   dipsw_init();
   keypad_init();
   
   
   
   //run_lab5_part1();
   //run_lab5_part2();
   run_lab5_part3();

   // Endless loop to prevent program from ending
   while (1);

} /* main */


void run_lab5_part1() {

   uint8_t loopCount = 0;
   bool hexON = false;
   bool pushButtonOneDown = false;
   
   while (loopCount < 3) {

      if (is_pb_down(PB1_IDX)) {
         if (hexON == false) {
            seg7_hex(3, 0);
            msec_delay(10);
            hexON = true;
         } else {
            hexON = false;
            seg7_off();
            loopCount++;
         }

         while (is_pb_down(PB1_IDX)) {
            msec_delay(15);
         }

      }

   }
}


void run_lab5_part2(){
    uint8_t lowNib;
    uint8_t highNib;
    uint8_t loopCounter = 0;
    // Initialize state
    states = GET_LOW;
    
    while (loopCounter < 3) { //suspend state machine after 3 runs
        // wait until LP_SW2 is pressed
        while (is_lpsw_up(LP_SW2_IDX) && (states != DISPLAY)); // Wait for button press unless inputs have already been put in         
        
        // state machine logic
        switch (states) {
            case GET_LOW:
                msec_delay(3000); //delay for user input
                lowNib = dipsw_read();  // Read lower nibble
                states = GET_HIGH;      // Move to next state
                break;

            case GET_HIGH:
                msec_delay(3000); //delay for user input
                highNib = dipsw_read();
                highNib = highNib << 4; // Shift high nibble to upper 4 bits
                states = DISPLAY;       // Move to next state
                break;

            case DISPLAY:
                msec_delay(2000); //delay for user input
                if(is_pb_down(PB1_IDX)){
                    msec_delay(15); // debounce delay
                    seg7_on(highNib | lowNib, 2); // Display final value at index 2
                } else{
                    seg7_on(highNib | lowNib, 0); // Display final value at index 0
                }
                states = GET_LOW; // Reset back to first state
                loopCounter++; //increase loop counter by 1
                break;
        }

        // Wait until LP_SW1 is released before continuing
        while (is_lpsw_down(LP_SW1_IDX));
        leds_off(); //cleanup
        seg7_off(); //cleanup
    }
    
}

void run_lab5_part3(){
    led_enable();
    uint8_t loopCounter = 0;
    uint8_t ledVal = 0;
    while(loopCounter < 8){

        loopCounter++;
        ledVal = getkey_pressed(); //waits until there is a key pressed - then records the value
        wait_no_key_pressed(); //prevents the code from speeding past. the user must press and then unpress for the code to continue.
        leds_on(ledVal); //turn on LED from value gotten from keypad
        
    }

    msec_delay(2000);
    //delay prevents the 8th input from immediately shutting off

    leds_off();
}