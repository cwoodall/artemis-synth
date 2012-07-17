#include "leds.h"
#include <avr/io.h>

uint8_t setupDisplay()
{
  LED_PORT &= 0xF0;
  LED_DDR  &= 0xF0;
  
  return 0x00;
}

void clearDisplay(uint8_t *disp)
{
  LED_PORT &= 0xF0;
  LED_DDR &= 0xF0;

  *disp = 0x00;
}

void setDisplay(uint8_t *disp, uint8_t new_disp)
{
  *disp = new_disp;
}

void updateDisplay(uint8_t *disp, uint8_t *refresh)
{
  if (*refresh & (1<<LED_INT))
    {
      *refresh &= ~(1<<LED_INT);
      // clear up display
      LED_PORT &= 0xF0;
      LED_DDR  &= 0xF0;
      if (*refresh & (1<<LED_REF))
	{
	  if (*disp & 1)
	    {
	      LED_PORT &= ~LED0_1_bm;
	      LED_DDR |= LED0_1_bm;
	    }
	  else
	    {
	      LED_DDR &= ~LED0_1_bm;
	    }
	  if (*disp & 1<<2)
	    {
	      LED_PORT &= ~LED2_3_bm;
	      LED_DDR |= LED2_3_bm;
	    }
	  else
	    {
	      LED_DDR &= ~LED2_3_bm;
	    }
	  if (*disp & (1<<4))
	    {
	      LED_PORT &= ~LED4_5_bm;
	      LED_DDR |= LED4_5_bm;
	    }
	  else
	    {
	      LED_DDR &= ~LED4_5_bm;
	    }
	  if (*disp & (1<<6))
	    {
	      LED_PORT &= ~LED6_7_bm;
	      LED_DDR |= LED6_7_bm;
	    }
	  else
	    {
	      LED_DDR &= ~LED6_7_bm;
	    }
	}
      else 
	{
	  if (*disp & 1<<1)
	    {
	      LED_PORT |= LED0_1_bm;
	      LED_DDR |= LED0_1_bm;
	    }
	  else
	    {
	      LED_DDR &= ~LED0_1_bm;
	    }
	  if (*disp & 1<<3)
	    {
	      LED_PORT |= LED2_3_bm;
	      LED_DDR |= LED2_3_bm;
	    }
	  else
	    {
	      LED_DDR &= ~LED2_3_bm;
	    }
	  if (*disp & (1<<5))
	    {
	      LED_PORT |= LED4_5_bm;
	      LED_DDR |= LED4_5_bm;
	    }
	  else
	    {
	      LED_DDR &= ~LED4_5_bm;
	    }
	  if (*disp & (1<<7))
	    {
	      LED_PORT |= LED6_7_bm;
	      LED_DDR |= LED6_7_bm;
	    }
	  else
	    {
	      LED_DDR &= ~LED6_7_bm;
	    }
	}
    }
}
