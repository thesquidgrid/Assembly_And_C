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
#include "uart.h"
#include "lcd1602.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void GROUP1_IRQHandler();
void config_pb1_interrupt();
void config_pb2_interrupt();
void run_lab9_part1();


//----------------------------------------------------------------------------- 
// Define system clock and SysTick period
//----------------------------------------------------------------------------- 
#define MSPM0_CLOCK_FREQUENCY (40e6) // System clock frequency: 40MHz
#define SYST_TICK_PERIOD (10.25E-3)  // SysTick interrupt period: 10.25ms
#define SYST_TICK_PERIOD_COUNT (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
enum States {
   MOTOR_OFF1,
   MOTOR_CW,
   MOTOR_OFF2,
   MOTOR_CCW
}
states;
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool g_SW1_pressed = false;
bool g_SW2_pressed = false;

// Define a structure to hold different data types

int main(void) {
   uint16_t adc_pot_value = 0;
   uint8_t switch_value = 1;
   
   // Configure the launchpad boards 
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

   
   run_lab9_part1();
   while(1);
   
}

void run_lab9_part1(void)
{
   int motor_speed = 50;
   int counter = 0;
   lcd_clear();
   lcd_set_ddram_addr(0x00);
   lcd_write_string("Motor Speed:");
   lcd_set_ddram_addr(LCD_CHAR_POSITION_12); 
   lcd_write_byte(motor_speed);
   lcd1602_write(LCD_IIC_ADDRESS, 0x25 ,LCD_DATA_REG);
   states = MOTOR_OFF1;

   
   while (g_SW1_pressed == false) { //while switch 1 is not pressed
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
      while ((!g_SW2_pressed) && (!g_SW1_pressed)){
        
        msec_delay(50);
        motor_speed = keypad_scan();
        if((motor_speed != 0x10)){
            
            lcd_set_ddram_addr(LCD_CHAR_POSITION_12);
            motor_speed = motor_speed * (100/16);
            motor0_set_pwm_dc(motor_speed);
            lcd_write_byte(motor_speed);
            lcd1602_write(LCD_IIC_ADDRESS, 0x25 ,LCD_DATA_REG);
        }
      }; 
      msec_delay(100); 
   }

   motor0_set_pwm_dc(0); //turn motor off
   led_off(LED_BAR_LD1_IDX);
   led_off(LED_BAR_LD2_IDX);
   lcd_clear();
   lcd_write_string("Program Stopped");
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