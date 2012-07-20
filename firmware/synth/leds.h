#ifndef _leds_h_
#define _leds_h_
#include <avr/io.h>

#define LED_PORT PORTC
#define LED_DDR DDRC
#define LED0_1_bm  (1<<3)
#define LED2_3_bm  (1<<2)
#define LED4_5_bm  (1<<1)
#define LED6_7_bm  (1<<0)

// Define flags in led_refresh_flag
#define LED_REF 0
#define LED_INT 1

/**
 * uint8_t setupDisplay()
 *
 * Initialize and setup display.
 *
 */
uint8_t setupDisplay();

void setDisplay(uint8_t *disp, uint8_t new_disp);

/**
 *
 */
void clearDisplay(uint8_t *disp);

/**
 *
 */
void updateDisplay(uint8_t *disp, uint8_t *refresh);
#endif
