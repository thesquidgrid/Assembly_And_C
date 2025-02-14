//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//
//  DESIGNER NAME:  Sophia Buchman
//
//       LAB NAME:  Lab 3 part 2
//
//      FILE NAME:  lab3_main_p2.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a practice exercise for students to implement 
//    bitwise operations in Assmebly. It includes a series of problems, each 
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

uint16_t my_asm_16bitset(uint16_t reg_value, uint16_t bit_mask); 
uint16_t my_asm_16bitclr(uint16_t reg_value, uint16_t bit_mask);
uint16_t my_asm_16bitcheck(uint16_t reg_value, uint16_t bit_mask);
void msp_printf(char* buffer, unsigned int value);

//-----------------------------------------------------------------------------
// Define symbolic constants used by program
//-----------------------------------------------------------------------------

// Uses the shift left operator to create the mask automatically based on 
// bit position 
#define A3_BIT_MASK                  (1 << 15)
#define A2_BIT_MASK                  (1 << 14)
#define A1_BIT_MASK                  (1 << 13)
#define A0_BIT_MASK                  (1 << 12)
#define PRS_BIT_MASK                 (7 << 9)
#define MODE_BIT_MASK                (3 << 7)
#define MODE_10_BIT_VALUE            (2 << 7)
#define MODE_01_BIT_VALUE            (1 << 7)
#define CRS_BIT_MASK                 (7 << 4)
#define MD_BIT_MASK                  (1 << 3)
#define RD_BIT_MASK                  (1 << 2)
#define EME_BIT_MASK                 (1 << 1)
#define PIE_BIT_MASK                 (1 << 0)
#define A0_to_A3_BIT_MASK            (15 << 12)


//-----------------------------------------------------------------------------
// Define global variable and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

uint16_t test_reg16 = 0xC000;


int main(void)
{
  // create local variable to hold register value
  uint16_t reg_value;
  
  clock_init_40mhz();
  launchpad_gpio_init();
  lp_leds_init();

  UART_init(115200);

  msp_printf(" *** PROGRAM RUNNING ***\r\n\r\n",0);
  msp_printf("CSC-202 ARM M0+ Assembly Bitwise Test Program\r\n",0);
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
  test_reg16 = my_asm_16bitset(test_reg16, PIE_BIT_MASK); 

  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);

  
  // ***************************************************************************
  // PROBLEM 2: Set the RD bit in test register
  // ***************************************************************************
  msp_printf("PROBLEM 2: Setting RD bit\r\n", 0);

  test_reg16 = my_asm_16bitset(test_reg16, RD_BIT_MASK); 

  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);

   
  // ***************************************************************************
  // PROBLEM 3: Set the CRS bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 3: Setting CRS bits\r\n", 0);

  // enter your code here for problem 3
  test_reg16 = my_asm_16bitset(test_reg16, CRS_BIT_MASK); 
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);

    
  // ***************************************************************************
  // PROBLEM 4: Set the A[3:0] bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 4: Setting A[3:0] bits\r\n", 0);


  test_reg16 = my_asm_16bitset(test_reg16, A0_to_A3_BIT_MASK); 
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);

   
  // ***************************************************************************
  // PROBLEM 5: Use an IF statement to test it A2 bit is set
  //            if A2 = 1 then print "Bit A2 is 1"
  //            else print "The bit A2 is 0"
  // ***************************************************************************
  msp_printf("PROBLEM 5: Testing bit A2\r\n", 0);

   if(my_asm_16bitcheck(test_reg16,A2_BIT_MASK)==1){
        msp_printf("Bit A2 is 1\r\n",0);
  } else{
        msp_printf("Bit A2 is 0\r\n",0);
  }
  msp_printf("\r\n",0);

  msp_printf("\r\n",0);

  
  // ***************************************************************************
  // PROBLEM 6: Clear A2 bit in test register
  // ***************************************************************************
  msp_printf("PROBLEM 6: Clearing A[2] bit\r\n", 0);

  test_reg16 = my_asm_16bitclr(test_reg16, A2_BIT_MASK);
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);

 
  // ***************************************************************************
  // PROBLEM 7: Clear CRS bits and set PRS bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 7: Clear CRS bits and set PRS bits\r\n", 0);

  test_reg16 = my_asm_16bitclr(test_reg16, CRS_BIT_MASK);
  test_reg16 = my_asm_16bitset(test_reg16, PRS_BIT_MASK);


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

 if(my_asm_16bitcheck(test_reg16,A2_BIT_MASK)==1){
        msp_printf("Bit A2=1 so clearing it\r\n",0);
        test_reg16 = my_asm_16bitclr(test_reg16, A2_BIT_MASK);
        
   } else{
        msp_printf("Bit A2=0 so setting it\r\n",0);
        test_reg16 = my_asm_16bitset(test_reg16, A2_BIT_MASK);
   }
  msp_printf("\r\n",0);
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

  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);
  
  if(my_asm_16bitcheck(test_reg16,MD_BIT_MASK)==0){
        msp_printf("Bit MD=0, setting mode=10\r\n",0);
        test_reg16 = my_asm_16bitset(test_reg16, MODE_10_BIT_VALUE);
        test_reg16 = my_asm_16bitclr(test_reg16, MODE_01_BIT_VALUE);
   } else{
        msp_printf("Bit MD=1, setting mode=11",0);
        test_reg16 = my_asm_16bitset( test_reg16, MODE_10_BIT_VALUE);
        test_reg16 = my_asm_16bitset( test_reg16, MODE_01_BIT_VALUE);

   }

   msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);

  // ***************************************************************************
  // PROBLEM 10: Clear all bits in test register
  // ***************************************************************************
  msp_printf("PROBLEM 10: Clearing all bits\r\n", 0);

  // enter your code here for problem 10

  test_reg16 = my_asm_16bitclr(test_reg16, 0xFFFF);
  msp_printf("    --> Test reg = 0x%04X\r\n", test_reg16);
  msp_printf("\r\n",0);


  msp_printf(" *** PROGRAM TERMINATED ***\r\n",0);
  

while(1);
} /* main */


/*
*Name: msp_printf
*Returns: nothing 
*Description: prints out to console
*/
    
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


