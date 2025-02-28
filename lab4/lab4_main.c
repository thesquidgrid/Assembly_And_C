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
/*  #include <atomic> */
#include <stdio.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>

#include "clock.h"

#include "LaunchPad.h"

#include "uart.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------

void run_lab4_part2();
void run_lab4_part1();
void run_lab4_part3();
void run_lab4_part4();

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

#define LED0_PIN 38

#define LED1_PIN 37

#define LED2_PIN 36

#define LED3_PIN 35

#define LED4_PIN 32

#define LED5_PIN 31

#define LED6_PIN 28

#define LED7_PIN 29

#define LED_EN 26

const char letters[] = {
  0x39,
  0x77,
  0x71,
  0x79
};

int seg7_digit = 0;

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

int main(void) {
  //start up needed periferals 
  clock_init_40mhz();
  launchpad_gpio_init();
  lp_leds_init();
  led_init();
  led_enable();
  // end of start up

  //run_lab4_part1();
  //run_lab4_part2();
  //run_lab4_part3();
  //msec_delay(500)

  //start up hex
  seg7_init();

  run_lab4_part4();

  while (1); //prevents issues with exit.c
}

void run_lab4_part1() {

  for (uint8_t i = 0; i < 8; i++) {
    led_on(i); // Turn on the current LED
    msec_delay(500); // Delay for visibility
  }

}

void run_lab4_part2() {

  int8_t counter = 4;

  while (counter >= 0) {
    // Loop through all 8 LEDs, turning them on one at a time
    for (uint8_t i = 0; i < 6; i++) {
      leds_off(); // Turn off all LEDs
      led_on(i); // Turn on the current LED
      msec_delay(200); // Delay for visibility
    }

    for (int8_t i = 6; i >= 0; i--) {

      leds_off(); // Turn off all LEDs
      led_on(i); // Turn on the current LED
      msec_delay(200); // Delay for visibility

    }

    leds_off();
    counter--;
  }
  // seg7_off();
}

void run_lab4_part3() {

  uint8_t counter = 0;
  uint8_t loopCounter = 0;

  while (loopCounter < 2) {

    leds_on((uint32_t) counter);
    counter++;
    msec_delay(100);
    leds_off();

    if (counter == 255) {
      loopCounter++;
    }

  }

}


void run_lab4_part4(){

    seg7_hex(16, 0); //Added L to list of possible char's. Displays on DIG0



}

/* main */