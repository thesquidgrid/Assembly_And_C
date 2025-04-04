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
#include "adc.h"
#include "clock.h"
#include "lcd1602.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_adc12.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/peripherals/m0p/hw_cpuss.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void OPA0_init();
void GROUP1_IRQHandler();
void config_pb1_interrupt();
void config_pb2_interrupt();
int celsius_to_fahrenheit(int temp);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

#define BIT12_TO_BIT8_DIV 16

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

bool g_SW1_pressed = false;
bool g_SW2_pressed = false;

// Define a structure to hold different data types

int main(void) {
  
  uint32_t pot_output;
  uint32_t temp_in_C;
  
  clock_init_40mhz();
  launchpad_gpio_init();

  I2C_init();
  ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);
  lcd1602_init();
  dipsw_init();
  led_init();
  led_enable();
  

  config_pb1_interrupt();
  config_pb2_interrupt();

  while (!g_SW1_pressed) {

    temp_in_C = ADC0_in(ADC12_MEMCTL_CHANSEL_CHAN_5);
    pot_output = ADC0_in(ADC12_MEMCTL_CHANSEL_CHAN_7); 

    int index = pot_output/455;
    
    leds_off();
    for(int led = 0; led<index; led++){
        led_on(led);
    }


    
    lcd_set_ddram_addr(0x00);

    lcd_write_string("ADC: ");
    lcd_write_doublebyte(pot_output);
    

    if(g_SW2_pressed){
       
        int temp = thermistor_calc_temperature(temp_in_C);
        temp = celsius_to_fahrenheit(temp);
        g_SW2_pressed = false;
        lcd_set_ddram_addr(0x40);
        
        lcd_write_string("Temp: ");
        lcd_write_byte(temp);
        lcd1602_write(LCD_IIC_ADDRESS, 0xDF ,LCD_DATA_REG); //displays little dot
        lcd_write_string("F");
    }
  }
  

  lcd_clear();
  lcd_write_string("Part 3 End");
  msec_delay(1000);
  lcd_set_display_off();
  leds_off();
  // Endless loop to prevent program from ending
  while (1);

} /* main */

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This function initializes the operational amplifier (OPA0) on the
//    MSPM0G3507 microcontroller. The configuration involves resetting the
//    module, enabling power, setting the gain bandwidth, and configuring
//    input and output pins. The OPA is then powered up and enabled for use
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
void OPA0_init(void) {
  OPA0->GPRCM.RSTCTL = (OA_RSTCTL_KEY_UNLOCK_W | OA_RSTCTL_RESETSTKYCLR_CLR |
                        OA_RSTCTL_RESETASSERT_ASSERT);

  OPA0->GPRCM.PWREN = (OA_PWREN_KEY_UNLOCK_W | OA_PWREN_ENABLE_ENABLE);

  // time for OPA to power up
  clock_delay(24);

  OPA0->CFGBASE &= ~(OA_CFGBASE_RRI_MASK);

  OPA0->CFGBASE |= ((uint32_t)OA_CFGBASE_GBW_HIGHGAIN);

  OPA0->CFG |= (OA_CFG_GAIN_MINIMUM | OA_CFG_MSEL_NC | OA_CFG_NSEL_EXTPIN0 |
                OA_CFG_PSEL_EXTPIN0 | OA_CFG_OUTPIN_ENABLED | OA_CFG_CHOP_OFF);

  // Enable the OPA
  OPA0->CTL |= OA_CTL_ENABLE_ON;

} /* OPA0_init */


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
void config_pb1_interrupt(void){

    GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;
    GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
    GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

    NVIC_SetPriority(GPIOB_INT_IRQn,2);
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
void config_pb2_interrupt(void){

    GPIOA->POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;
    GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
    GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

    NVIC_SetPriority(GPIOA_INT_IRQn,2);
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
        group_iidx_status = CPUSS->INT_GROUP[1].IIDX;

        switch (group_iidx_status) {

            case CPUSS_INT_GROUP_IIDX_STAT_INT0:
                gpio_mis = GPIOA->CPU_INT.MIS;
                if ((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) == GPIO_CPU_INT_MIS_DIO15_SET) {
                    g_SW2_pressed = true;
                    GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
                }
                break;

            case CPUSS_INT_GROUP_IIDX_STAT_INT1:
                gpio_mis = GPIOB->CPU_INT.MIS;
                if ((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) == GPIO_CPU_INT_MIS_DIO18_SET) {
                    g_SW1_pressed = true;
                    GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
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
int celsius_to_fahrenheit(int temp) 
{ 
    return ((temp * 9.0 / 5.0) + 32.0); 
}

