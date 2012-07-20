#include "leds.h"
#include <avr/io.h>

void setupDisplay()
{
  LED_PORT &= 0xF0;
  LED_DDR &= 0xF0;
}
