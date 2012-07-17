#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "HRL_SPI.h"
#include "HRL_MCP492x.h"
#define MCP492x_CONFIG 0x30 // Define configuration bits for MCP4921

#include "notes.h"
#include "inst.h"
#include "buttons.h"
#include "leds.h"
#include "optoloader.h"

uint8_t opto_enable_ctr = 0;
uint8_t led_refresh_flag = 0;
uint8_t led_display = 0x00000000;
optoloader_t optoloader;

uint8_t settings_debounce = 0;

void setupControlTimer();

int main(void)
{
  // Disable Interrupts for setup
  cli();
  
  setupControlTimer();
  // SETUP BUTTON INPUTS
  setupButtons();

  led_display = setupDisplay();
  optoloader = setupOptoloader();

  disableOptoloader(&optoloader);

  uint8_t settings = 0;
  uint8_t settings_prev = 0;
  sei(); // Enable Interrupts now that we are all ready to go

  for(;;) 
    {
      updateDisplay(&led_display, &led_refresh_flag);
      settings = readSettings();
      if (optoloader.flags & OPTO_ACTI_bm)
	{
	  if (settings_debounce == 0)
	    {	
	      if ((settings == 0) && (settings_prev != 0))
		{
		  opto_enable_ctr = 1;
		  settings_debounce = 1;
		}
	      else if ((settings != 0) && (settings_prev == 0))
		{
		  settings_debounce = 1;
		  opto_enable_ctr = 0;
		}
	    }	  
	} 
      else
	{
	  if (settings_debounce == 0)
	    {	
	      if ((settings == 0) && (settings_prev != 0))
		{
		  opto_enable_ctr = 1;
		  settings_debounce = 1;
		}
	      else if ((settings != 0) && (settings_prev == 0))
		{
		  settings_debounce = 1;
		  opto_enable_ctr = 0;
		}
	    }
	}
      settings_prev = settings;
    }
  return(0);
}

void setupControlTimer()
{    
  TCCR2A = 0;     // set entire TCCR1A register to 0
  TCCR2B = 0;     // same for TCCR1B
  TIMSK2 = 0;
  TCNT2 = 0;

  // target_freq = 306.4 Hz
  // timer_res = 1/16MHz
  // prescaler = 1024
  // timer_counts = (1.0/target_freq)/(timer_res*prescaler) - 1
  OCR2A = 50; 
  
  // Turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // set CS20, CS21 & CS22 for 1024 precaler
  TCCR2B |= (1 << CS20);
  TCCR2B |= (1 << CS21);
  TCCR2B |= (1 << CS22);

  // Enable interrupts
  TIMSK2 |= (1 << OCIE2A);
}

ISR(TIMER2_COMPA_vect)
{

  if (optoloader.flags & OPTO_ACTI_bm)
    {
      optoloader.counter += 1;
      if (opto_enable_ctr == 0xFF)
	{
	  optoloader.flags &= ~OPTO_ACTI_bm;
	  setDisplay(&led_display, 0x00);
	  disableOptoloader(&optoloader);
	  opto_enable_ctr = 0;
	}
      if (opto_enable_ctr > 0)
	{
	  opto_enable_ctr += 1;
	}      

    }
  else
    {
      if (opto_enable_ctr == 0xFF)
	{
	  optoloader.flags |= OPTO_ACTI_bm;
	  setDisplay(&led_display, 0xFF);
	  TCNT2 = 0;
	  enableOptoloader(&optoloader);
	  opto_enable_ctr = 0;
	}
      if (opto_enable_ctr > 0)
	{
	  opto_enable_ctr += 1;
	}      
    }

  if (settings_debounce == 30) 
    {
      settings_debounce = 0;
    }
  else if (settings_debounce >= 1) 
    {
      settings_debounce += 1;
    }

  led_refresh_flag ^= 0x01;
  led_refresh_flag |= 0x02;
}

ISR(ANALOG_COMP_vect)
{
  // Turn off Timer 2 Interrupt A
  TIMSK2 &= ~(1<<OCIE2A);
  // Reset Timer2 Counter
  TCNT2 = 0;

  // If the sync flag is set we can do some stuff
  if ((optoloader.flags & OPTO_SYNC_bm) != 0)
    {
      // If we are getting a zero
      if (optoloader.counter > (5*optoloader.zero_count/8))
	{
	  // If both the start and sync bits are set we can start collecting messages
	  // This is a 0 so we shift in a 0
	  if (optoloader.flags & OPTO_STRT_bm)
	    {
	      
	      optoloader.message <<= 1;
	      optoloader.message_count += 1;
	    }
	}
      else if (optoloader.counter > (optoloader.zero_count/16))
	{
	  // If the ones flag, ONEF is set the first part of the one message has been seen
	  if (optoloader.flags & OPTO_ONEF_bm)
	    {
	      optoloader.flags &= ~OPTO_ONEF_bm;
	      // When both parts of a 1 are observed 
	      // a one is shifted into the message buffer.
	      if (optoloader.flags & OPTO_STRT_bm)
		{
		  optoloader.message <<= 1;
		  optoloader.message |= 0x01;
		  optoloader.message_count += 1;
		}
	      else
		{
		  // The first one we see sets the start bit, it does not get shifted
		  // into the message.
		  optoloader.flags |= OPTO_STRT_bm;
		}
	    }
	  else
	    {
	      optoloader.flags |= OPTO_ONEF_bm;
	    }
	}
    }
  else // When we are not synced we need to find a sync
    {
      // If we have more than 4 successful counts taht are close to one another,
      // Accept that as our sync count.
      if (optoloader.sync_count == 4)
	{
	  optoloader.flags |= OPTO_SYNC_bm;
	  clearDisplay(&led_display);
	}
      else
	{
	  if ((optoloader.counter < (4*optoloader.prev_count/3)) &&
	      (optoloader.counter > (2*optoloader.prev_count/3)))
	    {
	      // Average them together if zero_count has already been set.
	      if (optoloader.zero_count > 0)
		{
		  // Add and divide by two (shift right by 1)
		  optoloader.zero_count = (optoloader.zero_count + optoloader.counter)>>1;
		}
	      else 
		{
		  optoloader.zero_count = optoloader.counter;
		}
	      optoloader.sync_count += 1;
	    } 
	  else
	    {
	      optoloader.zero_count = 0;
	      optoloader.sync_count = 0;
	    }
	  setDisplay(&led_display,1<<(optoloader.sync_count+1));
	}
    }

  if (optoloader.message_count == 8) 
    {
      optoloader.message_count = 0;
      led_display = optoloader.message;
    }
  
  optoloader.prev_count = optoloader.counter;
  optoloader.counter = 0;

  TIMSK2 |= (1<<OCIE2A);
}

