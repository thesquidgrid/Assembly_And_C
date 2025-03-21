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
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include <stddef.h>
#include <stdint.h>
#include <ti/drivers/GPIO.h>



//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------

bool run_lab7_part3(uint8_t count);
void run_lab7_part4();
void config_pb1_interrupts();
void config_pb2_interrupts();
#define MSPM0_CLOCK_FREQUENCY (40e6)


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define NUM_STATES 5
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
volatile uint8_t pb2_flag = 0;
volatile uint8_t pb1_flag = 0;

const uint8_t seg7_number_code[] = {
    0, 1, 2, 3, 4
};

const uint8_t delay_count[] = {
    0x18, 0x18, 0x18, 0x18, 0x18
};

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{
   clock_init_40mhz();
   launchpad_gpio_init();
   led_init();
   seg7_init();
   I2C_init();
   lcd1602_init();
   lcd_clear();
   dipsw_init();
   leds_off();
   
 
 // Endless loop to prevent program from ending
 while (1);

} /* main */


//////////////////////////////////////////////////////
// DESCRIPTION:
//	This function represents the ISR (Interrupt Service Routine) for
//	the sysTick timer. It is called at regular intervals based on the
//	configured SysTick period. This ISO is responsible for managing the
//	timing and display of the Morse code for .SOS. on the seven-segment
//	display. It controls the blink timing for the letters, with quick
//	blinks for "S. (dot-dot-dot) and slower blinks for "0" (dash-dash-dash).
//	The sequence of letters is repeated indefinitely.
//
// INPUT PARAMETERS:
// none
//
// OUTPUT PARAMETERS:
// none
//
// RETURN:
// none
///////////////////////////////////////////////



void run_lab7_part4() {
    config_pb1_interrupts();
    config_pb2_interrupts();
    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_7);

    uint8_t counter = 0;
    while (!pb1_flag) {
        leds_on(counter); // Display counter on LCD
        lcd_write_byte(counter); // Show binary representation
        
        if (pb2_flag) {
            lcd_clear();
            lcd_write_string("PB2 PRESSED");
            counter = 0;
        } else {
            lcd_clear();
        }
        counter++;
        if(counter > 99){
            counter = 0;
        }
        
        msec_delay(200); // 0.2s delay
        
    }
    
    lcd_clear();
    lcd_write_string("Program Stopped");
}




void config_pb1_interrupts() {
    GPIOB->GEN_EVENT1.ICLR = (1 << PB1_IDX);  // Clear any previous interrupt flag for PB1
    GPIOB->POLARITY15_0 |= (1 << PB1_IDX); // Set polarity for PB1
    GPIOB->GEN_EVENT1.IMASK |= (1 << PB1_IDX);  // Enable interrupt for PB1
    NVIC_EnableIRQ(GPIOB_INT_IRQn); // Enable NVIC interrupt for GPIOB
}

void config_pb2_interrupts() {
    GPIOB->GEN_EVENT1.ICLR = (1 << PB2_IDX);  // Clear any previous interrupt flag for PB2
    GPIOB->POLARITY15_0 |= (1 << PB2_IDX); // Set polarity for PB1    GPIOB->GEN_EVENT1.IMASK |= (1 << PB2_IDX);  // Enable interrupt for PB2
    NVIC_EnableIRQ(GPIOB_INT_IRQn); // Enable NVIC interrupt for GPIOB
}

void GROUP1_IRQHandler(void) {
    uint32_t source = GPIOB->GEN_EVENT1.IIDX; // Identify the interrupt source
    
    if (source == PB2_IDX) { // PB2 triggered the interrupt
        pb2_flag = !pb2_flag; // Toggle flag
        GPIOB->GEN_EVENT1.ICLR = (1 << PB2_IDX); // Clear interrupt flag
    }
    else if (source == PB1_IDX) { // PB1 triggered the interrupt
        pb1_flag = 1; // Set flag to stop program
        GPIOB->GEN_EVENT1.ICLR = (1 << PB1_IDX); // Clear interrupt flag
    }
}





   
