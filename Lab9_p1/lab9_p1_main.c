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

#include "adc.h"

#include "lcd1602.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

int main(void) {
   uint16_t adc_pot_value = 0;
   uint8_t switch_value = 0;
   uint8_t duty_cycle = 0;
   // Configure the launchpad boards 
   clock_init_40mhz();
   launchpad_gpio_init();
   dipsw_init();
   led_init();
   led_enable();
   I2C_init();
   lcd1602_init();
   // configure ADC 
   ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);
   // Configure motor0 with 50kHz PWM, 0% duty cycle 
   motor0_init();
   motor0_pwm_init(4000, 0);
   motor0_pwm_enable();

   // Create an endless loop for demotration purposes 
   while (1) {
      // Read SW1 to determine motor direction switch_value = dipsw_read(); 
      // read potentiometer; ADC ranges from 0 to 1023 adc_pot_value = ADC0_in(7); 
      // shift right by 2 to reduce noise; ADC ranges from 0 to 1023 adc_pot_value >>= 2; 
      // Scale ADC value to duty cycle percentage duty_cycle = (adc_pot_value * 100) / 1024; 
      motor0_set_pwm_dc(duty_cycle);
      // Write duty cycle to LCD 
      lcd_set_ddram_addr(0x00);
      lcd_write_byte(duty_cycle);
      // if SW1 = "xx01" then spin motor CCW
      if ((switch_value & 0x3) == 0x1){
         led_on(LED_BAR_LD1_IDX);
         led_off(LED_BAR_LD2_IDX);
      } /* if */

      // if SW1 = "xx10" then spin motor CW 
      else if ((switch_value & 0x3) == 0x2) {
         led_off(LED_BAR_LD1_IDX);
         led_on(LED_BAR_LD2_IDX);
      } /* else if */
      // else do not enable motor 
      else {
         led_off(LED_BAR_LD1_IDX);
         led_off(LED_BAR_LD2_IDX);
      } /* else */
   } /* while */
} /* main */
