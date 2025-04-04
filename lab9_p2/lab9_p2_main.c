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
#include "clock.h"
#include "lcd1602.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
int celsius_to_fahrenheit(int temp);
void GROUP1_IRQHandler();
void config_pb1_interrupt();
void config_pb2_interrupt();
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool g_SW1_pressed = false;
bool g_SW2_pressed = false;

// Define a structure to hold different data types

int main(void)
{

    clock_init_40mhz();
   launchpad_gpio_init();
   dipsw_init();
   led_init();
   led_enable();
   I2C_init();
   lcd1602_init();
   keypad_init();
   // configure ADC 
   ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);
   // Configure motor0 with 50kHz PWM, 0% duty cycle 
   motor0_init();
   motor0_pwm_init(4000, 0);
   motor0_pwm_enable();
   config_pb1_interrupt();
   config_pb2_interrupt();

   int temp = celsius_to_fahrenheit(ADC0_in(ADC12_MEMCTL_CHANSEL_CHAN_5));
   uint8_t threshold = 70;
   lcd_set_ddram_addr(0x00);
   lcd_write_string("Temperature");
   lcd_set_ddram_addr(0x12);
   lcd1602_write(LCD_IIC_ADDRESS, 0xDF ,LCD_DATA_REG); //displays little dot
   lcd_write_string("F");
   lcd_set_ddram_addr(0x40);
   lcd_write_string("Motor Speed:");
   lcd_set_ddram_addr(0x4A); 
   lcd1602_write(LCD_IIC_ADDRESS, 0x25 ,LCD_DATA_REG);

   while(!g_SW1_pressed){
    msec_delay(250); //delay for temp sensor stabilization
   }
 // Endless loop to prevent program from ending
 while (1);

} /* main */


int celsius_to_fahrenheit(int temp) 
{ 
    return ((temp * 9.0 / 5.0) + 32.0); 
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

