#ifndef _leds_h_
#define _leds_h_
#include <avr/io.h>

#define LED_PORT PORTC
#define LED_DDR DDRC
#define LED0_m ~(0x08)
#define LED1_m 0x08
#define LED2_m ~(0x04)
#define LED3_m 0x04
#define LED4_m ~(0x02)
#define LED5_m 0x02
#define LED6_m ~(0x01)
#define LED7_m 0x01

// Initialize display
void setupDisplay();

#endif
