#include "buttons.h"
#include <avr/io.h>

void setupButtons() 
{
  // Setup all Button pins to inputs with pullup resistors
  BUTTONS_0to5_DDR &= ~BUTTONS_0to5_m;
  BUTTONS_0to5_PORT |= BUTTONS_0to5_m;
  
  BUTTONS_6to7_DDR &= ~BUTTONS_6to7_m;
  BUTTONS_6to7_PORT |= BUTTONS_6to7_m;

  BUTTONS_8to9_DDR &= ~BUTTONS_8to9_m;
  BUTTONS_8to9_PORT |= BUTTONS_8to9_m;
}

// Read off individual buttons by number.
// See schematic for numbering convention.
uint8_t readButton(uint8_t button_no)
{
  // Use this to easily read off a button
  switch( button_no )
    {
    case 0: return (BUTTONS_0to5_PIN & BUTTON0_m) == 0;
    case 1: return (BUTTONS_0to5_PIN & BUTTON1_m) == 0;
    case 2: return (BUTTONS_0to5_PIN & BUTTON2_m) == 0;
    case 3: return (BUTTONS_0to5_PIN & BUTTON3_m) == 0;
    case 4: return (BUTTONS_0to5_PIN & BUTTON4_m) == 0;
    case 5: return (BUTTONS_0to5_PIN & BUTTON5_m) == 0;
    case 6: return (BUTTONS_6to7_PIN & BUTTON6_m) == 0;
    case 7: return (BUTTONS_6to7_PIN & BUTTON7_m) == 0;
    case 8: return (BUTTONS_8to9_PIN & BUTTON8_m) == 0;
    case 9: return (BUTTONS_8to9_PIN & BUTTON9_m) == 0;
    default: return 0;
    }
}

// Dump the 8 keyboard buttons (bottom row of 8) into a uint8_t representing there state
// 0 is pressed
// 1 is unpressed
// MSB -> LSB
// BUTTON7 -> BUTTON 0
uint8_t readKeyboard()
{
  return ((BUTTONS_6to7_PIN & BUTTONS_6to7_m)<<6)|(BUTTONS_0to5_PIN & BUTTONS_0to5_m);
}

// Dump the two settings buttons into a uint8_t with the lower two bits indicating state
uint8_t readSettings()
{
  return (BUTTONS_8to9_PIN & BUTTONS_8to9_m) >> 4;
}
