#ifndef _buttons_h_
#define _buttons_h_

#include <avr/io.h>
// Define the masks and port locations for the button
// Button Locations:
//   0-5: Port D
//   6-7: Port B
//   8-9: PORT C
#define BUTTONS_0to5_bm 0b00111111
#define BUTTONS_0to5_PIN PIND
#define BUTTONS_0to5_PORT PORTD
#define BUTTONS_0to5_DDR DDRD
#define BUTTON0_bm 0x01
#define BUTTON1_bm 0x02
#define BUTTON2_bm 0x04
#define BUTTON3_bm 0x08
#define BUTTON4_bm 0x10
#define BUTTON5_bm 0x20

#define BUTTONS_6to7_bm 0b00000011
#define BUTTONS_6to7_PIN PINB
#define BUTTONS_6to7_PORT PORTB
#define BUTTONS_6to7_DDR DDRB
#define BUTTON6_bm 0x01
#define BUTTON7_bm 0x02

#define BUTTONS_8to9_bm 0b00110000
#define BUTTONS_8to9_PIN PINC
#define BUTTONS_8to9_PORT PORTC
#define BUTTONS_8to9_DDR DDRC
#define BUTTON8_bm 0x10
#define BUTTON9_bm 0x20

void setupButtons();

// Read off individual buttons by number.
// See schematic for numbering convention.
uint8_t readButton(uint8_t button_no);

// Dump the 8 keyboard buttons (bottom row of 8) into a uint8_t representing there state
// 0 is pressed
// 1 is unpressed
// MSB -> LSB
// BUTTON7 -> BUTTON 0
uint8_t readKeyboard();
// Dump the two settings buttons into a uint8_t with the lower two bits indicating state
uint8_t readSettings();
#endif
