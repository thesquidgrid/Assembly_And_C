//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Sophia Buchman
//  LAB NAME:  lab 9
//  FILE NAME:  lab9_p2_main.c
//
//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This program serves as a demonstration of various MSP430 functionalities.
//    It reads temperature from a thermistor, converts the value to Fahrenheit, 
//    adjusts motor speed based on a threshold, and displays values on an LCD.
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
#include "uart.h"
#include "lcd1602.h"

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
int celsius_to_fahrenheit(int temp);
void GROUP1_IRQHandler(void);
void config_pb1_interrupt(void);
void config_pb2_interrupt(void);

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

   uint8_t speed = 25;
   uint8_t threshold = 81;
   int temp_in_C = 0;
   int temp = 0;
   bool done = false;
 
   lcd_set_ddram_addr(0x00);
   lcd_write_string(" TEMP  = ");
   lcd_set_ddram_addr(0x40);
   lcd_write_string(" SPEED = ");

   
   led_off(LED_BAR_LD1_IDX);
   led_on(LED_BAR_LD2_IDX);
  
   while (!done) {
      
      temp_in_C = ADC0_in(ADC12_MEMCTL_CHANSEL_CHAN_5);
      temp = thermistor_calc_temperature(temp_in_C);
      temp = celsius_to_fahrenheit(temp);

      
      lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_10);
      lcd_write_byte(temp);
      lcd1602_write(LCD_IIC_ADDRESS, 0xDF, LCD_DATA_REG); 
      lcd_write_string("F");

      
      lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_10);
      if (temp > threshold) {
         speed = 50;
      } else {
         speed = 25;
      }
      motor0_set_pwm_dc(speed);

      
      lcd_write_byte(speed);
      lcd1602_write(LCD_IIC_ADDRESS, 0x25, LCD_DATA_REG); 

      msec_delay(250);

      if (g_SW1_pressed) {
         done = true;
      }
   }

   
   led_off(LED_BAR_LD1_IDX);
   led_off(LED_BAR_LD2_IDX);
   motor0_set_pwm_dc(0);
   lcd_clear();
   lcd_write_string("Program Stopped");
   msec_delay(1000);
   led_disable();
   lcd_set_display_off();
   
   while (1);

} /* main */

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