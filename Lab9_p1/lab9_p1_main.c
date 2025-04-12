//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Sophia Buchman
//  LAB NAME:  Lab 9
//  FILE NAME:  lab9_p1_main.c
//
//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This program demonstrates the use of MSP430 peripherals such as ADC, PWM, 
//    and GPIO interrupts for controlling a motor and displaying information 
//    on an LCD based on user input.
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
void GROUP1_IRQHandler(void);
void config_pb1_interrupt(void);
void config_pb2_interrupt(void);
void run_lab9_part1(void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
enum States {
   MOTOR_OFF1,
   MOTOR_CW,
   MOTOR_OFF2,
   MOTOR_CCW
} states;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
bool g_SW1_pressed = false;
bool g_SW2_pressed = false;

//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------
int main(void) {
   uint16_t adc_pot_value = 0;
   uint8_t switch_value = 1;

   // Initialize peripherals
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

   // Run lab 9 part 1
   run_lab9_part1();

  
   while (1);
}

//-----------------------------------------------------------------------------
// Run Lab 9 Part 1 - Motor Control and Speed Adjustment
//-----------------------------------------------------------------------------
void run_lab9_part1(void) {
   int motor_speed = 20;
   int counter = 0;
   bool done = false;

   
   lcd_clear();
   lcd_set_ddram_addr(0x00);
   lcd_write_string("Motor Speed:");
   lcd_set_ddram_addr(LCD_CHAR_POSITION_12); 
   lcd_write_byte(motor_speed);
   lcd1602_write(LCD_IIC_ADDRESS, 0x25, LCD_DATA_REG);

   states = MOTOR_OFF1;
   motor0_set_pwm_dc(motor_speed); 
   while (!done) { 
      switch (states) {
         case MOTOR_OFF1:
            led_off(LED_BAR_LD1_IDX);
            led_off(LED_BAR_LD2_IDX);
            states = MOTOR_CW;
            break;

         case MOTOR_CW:
            led_off(LED_BAR_LD1_IDX);
            led_on(LED_BAR_LD2_IDX);
            states = MOTOR_OFF2;
            break;

         case MOTOR_OFF2:
            led_off(LED_BAR_LD1_IDX);
            led_off(LED_BAR_LD2_IDX);
            states = MOTOR_CCW;
            break;

         case MOTOR_CCW:
            led_on(LED_BAR_LD1_IDX);
            led_off(LED_BAR_LD2_IDX);
            states = MOTOR_OFF1;
            break;
      }

      g_SW2_pressed = false;
      while ((!g_SW2_pressed && !g_SW1_pressed)) {
         msec_delay(50);
         motor_speed = keypad_scan(); //poll until there is a keypad press
         if (motor_speed != 0x10) { 
            lcd_set_ddram_addr(LCD_CHAR_POSITION_12);
            motor_speed = motor_speed * (100 / 16);
            motor0_set_pwm_dc(motor_speed); 
            lcd_write_byte(motor_speed);
            lcd1602_write(LCD_IIC_ADDRESS, 0x25, LCD_DATA_REG);
         }
    
      if(g_SW1_pressed){
        done = true;
      }
      }
      msec_delay(100);
   }

   motor0_set_pwm_dc(0); // Turn motor off
   led_off(LED_BAR_LD1_IDX);
   led_off(LED_BAR_LD2_IDX);
   lcd_clear();
   lcd_write_string("Program Stopped");
   msec_delay(1000);
   led_disable();
   lcd_set_display_off();
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



void msp_printf(char* buffer, unsigned int value)
{
    unsigned int i=0;
    unsigned int len = 0;
    char string[80];

    len = sprintf(string, buffer, value);

    // Walk through arrya to send each character to serial port
    for (i=0; i<len; i++)
    {
        UART_out_char(string[i]);
    } /* for */
} /* msp printf */