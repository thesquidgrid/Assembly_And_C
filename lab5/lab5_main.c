/* **************************************************************************
 *  LAB NAME:       TBD
 *  DESIGNER NAME:  TBD
 *  FILE NAME:      main.c
 * **************************************************************************
 *
 *  DESCRIPTION:
 *  This program implements four parts of Lab 5. It utilizes GPIO inputs,
 *  outputs, and a state machine to interact with the MSP microcontroller.
 *  The lab exercises include:
 *
 *  - Part 1: Displaying numbers on a 7-segment display upon button press.
 *  - Part 2: Capturing DIP switch inputs and displaying the combined values.
 *  - Part 3: Using a keypad to turn on corresponding LEDs.
 *  - Part 4: Flashing LEDs based on keypad inputs.
 *
 * **************************************************************************/

#include <stdio.h>
#include <ti/devices/msp/msp.h>
#include "LaunchPad.h"
#include "clock.h"

//*****************************************************************************
// Define function prototypes
//*****************************************************************************
void run_lab5_part1();
void run_lab5_part2();
void run_lab5_part3();
void run_lab5_part4();
void flashAllLEDS(uint8_t);

//*****************************************************************************
// Define symbolic constants
//*****************************************************************************
#define LOOP_LIMIT  3
#define LED_FLASHES 500
#define DELAY_SHORT 1000
#define DELAY_MED   2000
#define DELAY_LONG  3000
#define DEBOUNCE    15

enum States {
    GET_LOW,
    GET_HIGH,
    DISPLAY
} states;

//*****************************************************************************
// Main function
//*****************************************************************************
int main(void) {
    clock_init_40mhz();
    launchpad_gpio_init();
    lp_leds_init();
    lpsw_init();
    led_init();
    led_enable();
    led_disable();
    seg7_init();
    dipsw_init();
    keypad_init();

    run_lab5_part1();
    msec_delay(LED_FLASHES);
    run_lab5_part2();
    msec_delay(LED_FLASHES);
    run_lab5_part3();
    msec_delay(LED_FLASHES);
    run_lab5_part4();

    while (1);  // Endless loop
}

/* **************************************************************************
 * FUNCTION: run_lab5_part1
 * DESCRIPTION: Implements a button-controlled 7-segment display.
 * PARAMETERS: None
 * RETURNS: None
 * **************************************************************************/
void run_lab5_part1() {
    uint8_t loopCount = 0;
    bool hexON = false;
    while (loopCount < LOOP_LIMIT) {
        if (is_pb_down(PB1_IDX)) {
            if (!hexON) {
                seg7_hex(3, 0);
                msec_delay(DELAY_SHORT / 100);
                hexON = true;
            } else {
                hexON = false;
                seg7_off();
                loopCount++;
            }
            while (is_pb_down(PB1_IDX)) {
                msec_delay(DEBOUNCE);
            }
        }
    }
}

/* **************************************************************************
 * FUNCTION: run_lab5_part2
 * DESCRIPTION: Reads DIP switch values and displays them on 7-segment display.
 * PARAMETERS: None
 * RETURNS: None
 * **************************************************************************/
void run_lab5_part2(){
    uint8_t lowNib, highNib, loopCounter = 0;
    states = GET_LOW;
    while (loopCounter < LOOP_LIMIT) {
        while (is_lpsw_up(LP_SW2_IDX) && (states != DISPLAY));
        switch (states) {
            case GET_LOW:
                msec_delay(DELAY_SHORT);
                lowNib = dipsw_read();
                states = GET_HIGH;
                loopCounter++;
                break;
            case GET_HIGH:
                msec_delay(DELAY_SHORT);
                highNib = dipsw_read() << 4;
                states = DISPLAY;
                loopCounter++;
                break;
            case DISPLAY:
                msec_delay(DELAY_MED);
                seg7_on(highNib | lowNib, is_pb_down(PB1_IDX) ? 2 : 0);
                states = GET_LOW;
                loopCounter++;
                break;
        }
        while (is_lpsw_down(LP_SW1_IDX));
    }
    msec_delay(DELAY_LONG);
    leds_off();
    seg7_off();
}

/* **************************************************************************
 * FUNCTION: run_lab5_part3
 * DESCRIPTION: Reads keypad input and lights corresponding LED.
 * PARAMETERS: None
 * RETURNS: None
 * **************************************************************************/
void run_lab5_part3(){
    led_enable();
    uint8_t loopCounter = 0, ledVal = 0;
    while(loopCounter < 9){
        loopCounter++;
        ledVal = getkey_pressed();
        wait_no_key_pressed();
        leds_on(ledVal);
    }
    msec_delay(DELAY_MED);
    leds_off();
}

/* **************************************************************************
 * FUNCTION: run_lab5_part4
 * DESCRIPTION: Reads keypad input and flashes LEDs accordingly.
 * PARAMETERS: None
 * RETURNS: None
 * **************************************************************************/
void run_lab5_part4(){
    led_enable();
    seg7_off();
    uint8_t loopCounter = 0, ledVal = 0;
    while(loopCounter < 4){
        loopCounter++;
        while(getkey_pressed() == 0x10);
        ledVal = keypad_scan();
        wait_no_key_pressed();
        flashAllLEDS(ledVal);
    }
    msec_delay(DELAY_MED);
    leds_off();
}

/* **************************************************************************
 * FUNCTION: flashAllLEDS
 * DESCRIPTION: Flashes all LEDs a specified number of times.
 * PARAMETERS: numberOfTimes - Number of LED flashes
 * RETURNS: None
 * **************************************************************************/
void flashAllLEDS(uint8_t numberOfTimes){
    uint8_t counter = 0;
    while(counter < numberOfTimes){
        leds_on(255);
        msec_delay(LED_FLASHES);
        leds_off();
        msec_delay(LED_FLASHES);
        counter++;
    }
}
