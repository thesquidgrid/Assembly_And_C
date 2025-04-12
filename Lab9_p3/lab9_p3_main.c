//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Sophia Buchman
//  LAB NAME:  Lab 9
//  FILE NAME:  lab9_p3_main.c
//
//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This program controls a servo motor based on the value of the potentiometer.
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Standard C include files
//-----------------------------------------------------------------------------
#include <stdio.h>

//-----------------------------------------------------------------------------
// MSP430 LaunchPad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>

#include "LaunchPad.h"

#include "clock.h"

#include "adc.h"

#include "ti/devices/msp/peripherals/hw_dma.h"

#include "uart.h"

#include "lcd1602.h"

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void GROUP1_IRQHandler(void);
void config_pb1_interrupt(void);
void config_pb2_interrupt(void);
void run_lab9_part3(void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

#define servoCount_Min 100
#define servoCount_Max 500
#define maxADCVal 1024
//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
bool g_SW1_pressed = false;
bool g_SW2_pressed = false;

//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------
int main(void) {

   
   clock_init_40mhz();
   launchpad_gpio_init();
   dipsw_init();
   led_init();
   led_enable();
   I2C_init();
   lcd1602_init();
   keypad_init();


   ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);
   motor0_init();
   motor0_pwm_init(4000, 0);
   motor0_pwm_enable();

   
   config_pb1_interrupt();
   config_pb2_interrupt();

   run_lab9_part3();

   while (1);
}

/**
 * Runs Lab 9 Part 3: Controls motor and displays values on the LCD.
 *
 * This function reads the ADC value, calculates the servo count, adjusts the 
 * motor PWM, and displays the ADC and servo count values on the LCD. It also 
 * checks for user input to stop the program via push button 1 (SW1).
 */

void run_lab9_part3(void) {
   lcd_clear();
   lcd_write_string("ADC VALUE =  ");
   lcd_set_ddram_addr(LCD_LINE2_ADDR);
   lcd_write_string("SERVO CNT =  ");
   bool done = false;
   leds_off();
   uint16_t pot_output = 0;
   uint16_t servoCount = 0;

   while (!done) {
      pot_output = ADC0_in(ADC12_MEMCTL_CHANSEL_CHAN_7);
      pot_output = pot_output >> 2;
      servoCount = servoCount_Min + ((pot_output * (servoCount_Max - servoCount_Min)) / maxADCVal);
      motor0_set_pwm_count(servoCount);

      lcd_set_ddram_addr(LCD_CHAR_POSITION_12 + LCD_LINE1_ADDR);
      lcd_write_doublebyte(pot_output);
      lcd_set_ddram_addr(LCD_CHAR_POSITION_12 + LCD_LINE2_ADDR);
      lcd_write_doublebyte(servoCount);
      if (g_SW1_pressed) {
         done = true;
         motor0_pwm_disable();
      }

   }

   lcd_clear();
   lcd_write_string("Program Done");
}

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Configures an interrupt for push button 1 on GPIOB.
//    This function sets the polarity for a rising edge trigger,
//    clears any pending interrupt, enables the interrupt, and
//    sets its priority in the NVIC.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
// -----------------------------------------------------------------------------
void config_pb1_interrupt(void) {

   GPIOB -> POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;
   GPIOB -> CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
   GPIOB -> CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

   NVIC_SetPriority(GPIOB_INT_IRQn, 2);
   NVIC_EnableIRQ(GPIOB_INT_IRQn);

}

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Configures an interrupt for push button 2 on GPIOA.
//    This function sets the polarity for a rising edge trigger,
//    clears any pending interrupt, enables the interrupt, and
//    sets its priority in the NVIC.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
// -----------------------------------------------------------------------------
void config_pb2_interrupt(void) {

   GPIOA -> POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;
   GPIOA -> CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
   GPIOA -> CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

   NVIC_SetPriority(GPIOA_INT_IRQn, 2);
   NVIC_EnableIRQ(GPIOA_INT_IRQn);

}

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    Handles GPIO interrupts for push buttons 1 and 2.
//    This function checks the interrupt status for GPIOA and GPIOB,
//    sets global flags accordingly, and clears the interrupt flags.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
// -----------------------------------------------------------------------------
void GROUP1_IRQHandler(void) {
   uint32_t group_iidx_status;
   uint32_t gpio_mis;
   do {
      group_iidx_status = CPUSS -> INT_GROUP[1].IIDX;

      switch (group_iidx_status) {

      case CPUSS_INT_GROUP_IIDX_STAT_INT0:
         gpio_mis = GPIOA -> CPU_INT.MIS;
         if ((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) == GPIO_CPU_INT_MIS_DIO15_SET) {
            g_SW2_pressed = true;
            msec_delay(15);
            GPIOA -> CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
         }
         break;

      case CPUSS_INT_GROUP_IIDX_STAT_INT1:
         gpio_mis = GPIOB -> CPU_INT.MIS;
         if ((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) == GPIO_CPU_INT_MIS_DIO18_SET) {
            g_SW1_pressed = true;
            msec_delay(15);

            GPIOB -> CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
         }
         break;

      default:
         break;
      }
   } while (group_iidx_status != 0);
}