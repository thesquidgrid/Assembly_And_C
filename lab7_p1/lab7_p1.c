//////////////////////////////////////////////////////
// DESCRIPTION:
//    This program serves as a demonstration of GPIO interrupt handling
//    on the MSPM0G3507 microcontroller. It utilizes a SysTick timer
//    to control the display of Morse code on a seven-segment display.
//    Additionally, it updates an LCD screen based on button inputs.
//
//    The main loop waits for button presses to switch between two parts
//    of the lab, demonstrating different text scrolling behaviors on the LCD.
//
// INPUT PARAMETERS:
//    none
//
// OUTPUT PARAMETERS:
//    none
//
// RETURN:
//    none
//////////////////////////////////////////////////////

#include <stdio.h>
#include <ti/devices/msp/msp.h>
#include "LaunchPad.h"
#include "clock.h"
#include "lcd1602.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab7_part1();
void SysTick_Handler();
void lcd_string_parser(const char* string, uint8_t start_lcd_addr,
                       uint8_t max_lcd_addr);
void run_lab7_part2();

#define MSPM0_CLOCK_FREQUENCY (40e6)
#define SYST_TICK_PERIOD (10.25E-3)
#define SYST_TICK_PERIOD_COUNT (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible, avoid using global variables.
//-----------------------------------------------------------------------------
#define NUM_STATES 18
const uint8_t seg7_letter_code[] = {
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00, // S
    0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x00, // O
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00  // S
};

const uint8_t delay_count[] = {
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, // dots
    0x24, 0x24, 0x24, 0x24, 0x24, 0x24, // dashes
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x30  // dots
};

//-----------------------------------------------------------------------------
// Main function: Initializes peripherals and handles button-based transitions
//-----------------------------------------------------------------------------
int main(void) {
    clock_init_40mhz();
    launchpad_gpio_init();
    led_init();
    seg7_init();
    I2C_init();
    lcd1602_init();
    lcd_clear();
    dipsw_init();
    sys_tick_init(SYST_TICK_PERIOD_COUNT);

    SysTick_Handler();

    run_lab7_part1();
    while (!is_pb_down(PB1_IDX));  // wait for PB1 press

    sys_tick_disable();
    seg7_off();
    lcd_clear();
    lcd_set_ddram_addr(0x00);
    lcd_write_string("Press PB2");
    while (!is_pb_down(PB2_IDX));  // wait for PB2 press

    run_lab7_part2();
    lcd_clear();
    lcd_set_ddram_addr(0x00);
    lcd_write_string("Press PB2");
    while (!is_pb_down(PB2_IDX));  // wait for PB2 press

    while (1); // Infinite loop to prevent program termination
}

//////////////////////////////////////////////////////
// DESCRIPTION:
//    Handles scrolling text display on the LCD for Lab 7 Part 1.
//    The text scrolls from right to left while monitoring for
//    a button press to exit.
//////////////////////////////////////////////////////
void run_lab7_part1() {
    bool done = false;
    char msg[] = "Microcontrollers are fun";

    while (!done) {
        for (int led_add = 0x4F; ((led_add > 0x40) && (!done)); led_add--) {
            lcd_clear();
            lcd_set_ddram_addr(led_add);
            lcd_write_string(msg);
            msec_delay(100);
            if (is_pb_down(PB1_IDX)) done = true;
        }
    }
}

//////////////////////////////////////////////////////
// DESCRIPTION:
//    Handles scrolling text display on the LCD for Lab 7 Part 2.
//    The message moves dynamically based on button presses.
//////////////////////////////////////////////////////
void run_lab7_part2() {
    bool done = false;
    char* msg = "Microcontrollers are fun. I love programming in MSPM0+ assembly code!!!";
    int led_add = 0x4F;
    int index = 0;
    int index2 = 0;

    while ((msg[index] != '\0') && (!done)) {
        index++;
        if (index > 16) {
            lcd_clear();
            lcd_set_ddram_addr(0x40);
            index2++;
            lcd_string_parser(msg + index2, 0x40, 0x50);
            msec_delay(50);
        } else {
            lcd_clear();
            lcd_set_ddram_addr(led_add);
            lcd_string_parser(msg, 0x40, 0x50);
            msec_delay(100);
            led_add--;
        }
        if (is_pb_down(PB2_IDX)) done = true;
    }
}

//////////////////////////////////////////////////////
// DESCRIPTION:
//    Interrupt Service Routine for the SysTick timer. Controls
//    the blinking pattern for Morse code "SOS" on a 7-segment display.
//////////////////////////////////////////////////////
void SysTick_Handler(void) {
    static uint16_t delay_time = 1;
    static uint16_t code_index = 0;

    delay_time--;
    if (delay_time == 0) {
        seg7_on(seg7_letter_code[code_index], SEG7_DIG0_ENABLE_IDX);
        delay_time = delay_count[code_index];
        code_index++;
        if (code_index == NUM_STATES) code_index = 0;
    }
}

//////////////////////////////////////////////////////
// DESCRIPTION:
//    Parses a string and writes it to the LCD within specified bounds.
//////////////////////////////////////////////////////
void lcd_string_parser(const char* string, uint8_t start_lcd_addr,
                       uint8_t max_lcd_addr) {
    uint8_t maxCount = max_lcd_addr - start_lcd_addr;
    uint8_t count = 0;
    while ((*string != '\0') && (count < maxCount)) {
        lcd_write_char(*string++);
        count++;
    }
}
