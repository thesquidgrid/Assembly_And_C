//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//
//  DESIGNER NAME:  TBD
//
//       LAB NAME:  Lab 3, part 1
//
//      FILE NAME:  lab3_main_p1.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a practice exercise for students to implement 
//    bitwise operations in C. It includes a series of problems, each 
//    requiring the manipulation of bits in a test register using bitwise 
//    operations. The program initializes the UART communication, sets up 
//    the system clock, and provides a function 'msp_printf' to print 
//    messages over UART. 
//
//    Each problem is outlined with a description, and students are instructed 
//    to write code to solve the problem within the designated sections. 
//    Upon execution, the program demonstrates the effect of various 
//    bitwise operations on the test register, along with the corresponding 
//    output messages indicating the results of each operation. After 
//    completing all the problems, the program terminates.
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "clock.h"
#include "LaunchPad.h"
#include "uart.h"


//-----------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------
uint16_t set_bit(uint16_t reg_value, uint16_t bit_mask); //set_bit
bool check_bit(uint16_t reg_value, uint16_t bit_mask); //check bit
uint16_t clear_bit(uint16_t reg_value, uint16_t bit_mask); //Clear_bit

void msp_printf(char* buffer, unsigned int value) //output to serial console
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


//-----------------------------------------------------------------------------
// Define symbolic constants used by program
//-----------------------------------------------------------------------------

#define A0_BIT  0x0001 // PIE
#define A1_BIT  0x0002 // EME
#define A2_BIT  0x0004 // RD
#define A3_BIT  0x0008 // MD
#define A4_BIT  0x0010 // CRS
#define A5_BIT  0x0020 // CRS
#define A6_BIT  0x0040 // CRS
#define A7_BIT  0x0080 // MODE
#define A8_BIT  0x0100 // MODE
#define A9_BIT  0x0200 // PRS
#define A10_BIT 0x0400 // PRS
#define A11_BIT 0x0800 // PRS 
#define A12_BIT 0x1000 // A0
#define A13_BIT 0x2000 // A1
#define A14_BIT 0x4000 // A2
#define A15_BIT 0x8000 // A3



//-----------------------------------------------------------------------------
// Define global variable and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

uint16_t test_reg16 = 0x0000;


int main(void)
{
  // create local variable to hold register value
  uint16_t reg_value;
  
  clock_init_40mhz();
  launchpad_gpio_init();
  lp_leds_init();

  UART_init(115200);

  msp_printf(" *** PROGRAM RUNNING ***\r\n\r\n",0);
  msp_printf("CSC-202 ARM M0+ C Bitwise Test Program\r\n",0);
  msp_printf("--------------------------------------------------------\r\n",0);

  // Display the size of the test register
  msp_printf(" The size of the test reg is 0x%X bytes\r\n", sizeof(test_reg16));

  // Display the value of the test register
  msp_printf("The starting value of test reg is 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 1: Set the PIE bit in test register (test_reg16)
  // ***************************************************************************
  msp_printf("PROBLEM 1: Setting PIE bit\r\n", 0);

  // enter your code here for problem 1
  test_reg16 = set_bit(test_reg16, A0_BIT);
  //A15_BIT = test_reg16;
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 2: Set the RD bit in test register
  // ***************************************************************************
  msp_printf("PROBLEM 2: Setting RD bit\r\n", 0);

  // enter your code here for problem 2
  test_reg16 = set_bit(test_reg16, A2_BIT);
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 3: Set the CRS bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 3: Setting CRS bits\r\n", 0);

  // enter your code here for problem 3
  test_reg16 = set_bit(test_reg16, A4_BIT);
  test_reg16 = set_bit(test_reg16, A5_BIT);
  test_reg16 = set_bit(test_reg16, A6_BIT);
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 4: Set the A[3:0] bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 4: Setting A[3:0] bits\r\n", 0);

  // enter your code here for problem 4
  uint16_t bitMask = (A15_BIT | A14_BIT | A13_BIT | A12_BIT);
  test_reg16 = set_bit(test_reg16, bitMask);
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);

  
  // ***************************************************************************
  // PROBLEM 5: Use an IF statement to test it A2 bit is set
  //            if A2 = 1 then print "Bit A2 is 1"
  //            else print "The bit A2 is 0"
  // ***************************************************************************
  msp_printf("PROBLEM 5: Testing bit A2\r\n", 0);

  // enter your code here for problem 5
  if(check_bit(test_reg16,A2_BIT)==true){
        msp_printf("Bit A2 is 1\r\n",0);
  } else{
        msp_printf("Bit A2 is 0\r\n",0);
  }
  msp_printf("\r\n",0);

  
  // ***************************************************************************
  // PROBLEM 6: Clear A2 bit in test register
  // ***************************************************************************
  msp_printf("PROBLEM 6: Clearing A[2] bit\r\n", 0);

  test_reg16 = clear_bit(test_reg16, A14_BIT);
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 7: Clear CRS bits and set PRS bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 7: Clear CRS bits and set PRS bits\r\n", 0);
   
   uint16_t CRS_bitMask = (A4_BIT | A5_BIT | A6_BIT);
   uint16_t PRS_bitMask = (A9_BIT | A10_BIT | A11_BIT);

   test_reg16 = clear_bit(test_reg16, CRS_bitMask);
   test_reg16 = set_bit(test_reg16, PRS_bitMask);
   msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
   msp_printf("\r\n",0);

  
  // ***************************************************************************
  // PROBLEM 8: Use an IF statement to test if A2 is set
  //            if A2 = 1 then
  //                print "Bit A2=1 so clearing it"
  //                modify the reg to clear the bit
  //            else
  //                print "Bit A2=0 so setting it"
  //                modify the reg to set the bit
  // ***************************************************************************
  msp_printf("PROBLEM 8: Testing bit A2\r\n", 0);

   if(check_bit(test_reg16,A14_BIT)==true){
        msp_printf("Bit A2=1 so clearing it\r\n",0);
        test_reg16 = clear_bit(test_reg16, A14_BIT);
   } else{
        msp_printf("Bit A2=0 so setting it\r\n",0);
        test_reg16 = set_bit(test_reg16, A14_BIT);
   }
  
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);


  // ***************************************************************************
  // PROBLEM 9: Use an IF statement to test it MD is 0
  //            if MD = 0 then
  //                print "Bit MD=0, setting mode=10"
  //                set MODE to 10
  //            else
  //                print "Bit MD=1, setting mode=11"
  //                set MODE to 11
  // ***************************************************************************
  msp_printf("PROBLEM 9: Testing bit MD & setting mode bits\r\n", 0);

  // enter your code here for problem 9
    if(check_bit(test_reg16,A3_BIT)==false){
        msp_printf("Bit MD=0, setting mode=10\r\n",0);
        unsigned short mode10Mask = 0x0100; //bitmask in hex for MODE 10
        test_reg16 = set_bit(reg_value, mode10Mask);
   } else{
        msp_printf("Bit MD=1, setting mode=11",0);
        unsigned short mode11Mask = 0x0180; //bitmask in hex for MODE 11
        test_reg16 = set_bit( reg_value, mode11Mask);
   }

  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);

  /*
  // ***************************************************************************
  // PROBLEM 10: Clear all bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 10: Clearing all bits\r\n", 0);

  // enter your code here for problem 10

  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);


  msp_printf(" *** PROGRAM TERMINATED ***\r\n",0);
  
  for(;;);
*/
} /* main */


//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function sets the specified bit(s) in a 16-bit register value using
//  the provided bit mask. It performs a bitwise OR operation to set the bit(s).
//
// INPUT PARAMETERS:
//  reg_value - The original 16-bit register value.
//  bit_mask  - The bit mask indicating which bit(s) to clear.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  uint16_t - The modified register value with the specified bit(s) set.
// -----------------------------------------------------------------------------

uint16_t set_bit(uint16_t reg_value, uint16_t bit_mask)
{
    uint16_t returnval;
    returnval = reg_value | bit_mask;
    return returnval;
}


//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function clears the specified bit(s) in a 16-bit register value using
//  the provided bit mask. It performs a bitwise AND operation with the
//  complement of the bit mask to clear the bit(s).
//
// INPUT PARAMETERS:
//  reg_value - The original 16-bit register value.
//  bit_mask  - The bit mask indicating which bit(s) to clear.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  uint16_t - The modified register value with the specified bit(s) cleared.
// -----------------------------------------------------------------------------

uint16_t clear_bit(uint16_t reg_value, uint16_t bit_mask)
{
    bit_mask = ~bit_mask; //1's compliment on mask
    msp_printf("    --> bitmask = 0x%04X\r\n", bit_mask);
    
    uint16_t returnVal = reg_value & bit_mask; //will always be zero is the bit_mask is 0.
    msp_printf("    --> returnVal = 0x%04X\r\n", returnVal);
    return returnVal;

}

//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function checks if the specified bit(s) in a 16-bit register value are
//  set using the provided bit mask. It performs a bitwise AND operation to 
//  verify if the bit(s) are set.
//
// INPUT PARAMETERS:
//  reg_value - a 16-bit register value to check.
//  bit_mask  - a 16-bit mask indicating which bit(s) to check.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  bool - true if the specified bit(s) are set, false otherwise.
// -----------------------------------------------------------------------------

bool check_bit(uint16_t reg_value, uint16_t bit_mask){

    if((reg_value & bit_mask) == bit_mask)
    {
        return true;
    } else{
        return false;
    }
}
