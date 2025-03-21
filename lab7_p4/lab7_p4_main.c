//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Sophia Buchman
//
//       LAB NAME:  lab7
//
//      FILE NAME:  lab7_p4_main.c
//
//----------------------------------------------------------------------------- 
//
// DESCRIPTION:
//    This program serves as a GPIO interrupt handler for the MSPM0G3507
//    microcontroller. It detects button presses using interrupts instead of 
//    polling, updates an LCD display, and manages global flags for state changes.
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
void run_lab7_part4();
void SysTick_Handler();

//----------------------------------------------------------------------------- 
// Define system clock and SysTick period
//----------------------------------------------------------------------------- 
#define MSPM0_CLOCK_FREQUENCY (40e6) // System clock frequency: 40MHz
#define SYST_TICK_PERIOD (10.25E-3)  // SysTick interrupt period: 10.25ms
#define SYST_TICK_PERIOD_COUNT (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

//----------------------------------------------------------------------------- 
// Define global variables
//----------------------------------------------------------------------------- 
bool pb1_flag = false;  // Flag for button 1 press
bool pb2_flag = false;  // Flag for button 2 press

//////////////////////////////////////////////////////
// DESCRIPTION:
//	Main function initializing peripherals and running Lab 7 Part 4.
//  The function sets up the system clock, GPIO, LED, I2C, and LCD.
//  It initializes SysTick for handling periodic interrupts.
//  The function then enters an infinite loop to keep the program running.
//
// INPUT PARAMETERS:
//	none
//
// OUTPUT PARAMETERS:
//	none
//
// RETURN:
//	none
//////////////////////////////////////////////////////
int main(void) {
    clock_init_40mhz();   // Initialize system clock to 40MHz
    launchpad_gpio_init(); // Initialize GPIO pins for buttons
    led_init();           // Initialize LEDs
    led_enable();         // Enable LED functionality
    I2C_init();           // Initialize I2C communication
    lcd1602_init();       // Initialize LCD display
    lcd_clear();          // Clear LCD screen
    dipsw_init();         // Initialize DIP switch (if applicable)
    sys_tick_init(SYST_TICK_PERIOD_COUNT); // Configure SysTick interrupt
    seg7_off();

    SysTick_Handler();    // Call SysTick handler for initial setup
    run_lab7_part4();     // Execute main lab function

    // Endless loop to prevent program from ending
    while (1);
} 

//////////////////////////////////////////////////////
// DESCRIPTION:
//	This function represents the ISR (Interrupt Service Routine) for
//	the SysTick timer. It is triggered at regular intervals to handle
//	button press events. If PB2 is pressed, the pb2_flag is set, and a 
//	debounce delay is applied. If PB1 is pressed, the pb1_flag is set.
//
// INPUT PARAMETERS:
//	none
//
// OUTPUT PARAMETERS:
//	none
//
// RETURN:
//	none
//////////////////////////////////////////////////////
void SysTick_Handler(void) {
    if (is_pb_down(PB2_IDX)) {
        pb2_flag = true;
        msec_delay(15); // Debounce delay
    }
    else if (is_pb_down(PB1_IDX)) {
        pb1_flag = true;
    }
}

//////////////////////////////////////////////////////
// DESCRIPTION:
//	This function handles the logic for Lab 7 Part 4. It monitors button
//	presses using global flags and updates the LCD display accordingly.
//  If PB2 is pressed, a message "PB2 PRESSED" is shown on the LCD.
//  If PB1 is pressed, the loop terminates, and "Program Stopped" is displayed.
//
// INPUT PARAMETERS:
//	none
//
// OUTPUT PARAMETERS:
//	none
//
// RETURN:
//	none
//////////////////////////////////////////////////////
void run_lab7_part4() {
    bool toggle = pb2_flag;
    uint8_t counter = 0;

    while (!pb1_flag) {
        lcd_set_ddram_addr(0X04);
        leds_on(counter); // Display counter on LCD
        lcd_write_doublebyte(counter); // Show binary representation
        msec_delay(300); // Delay for visibility

        if (pb2_flag) {
            toggle = !toggle;
            if (toggle) {
                lcd_set_ddram_addr(0X40);
                lcd_write_string("PB2 PRESSED");
            } else {
                lcd_clear();
            }
            pb2_flag = false; // Reset flag after handling
        } 
        
        counter++;
        if (counter > 99) {
            counter = 0;
            lcd_clear();
        }
    }
    
    lcd_clear();
    lcd_write_string("Program Stopped");
}
