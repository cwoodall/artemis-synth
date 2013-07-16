#include "optoloader.h"


void disableOptoloader(optoloader_t *opto)
{
  ACSR &= ~(1<<ACIE);

  opto->flags = 0;
}

optoloader_t setupOptoloader() 
{
  // Set up Analog Compare Interrupt
  ACSR =
    (0<<ACD)   |   // Analog Comparator: Enabled
    (0<<ACBG)  |   // Analog Comparator Bandgap Select: AIN0 is applied to the positive input
    (0<<ACO)   |   // Analog Comparator Output: Off
    (1<<ACI)   |   // Analog Comparator Interrupt Flag: Clear Pending Interrupt
    (1<<ACIE)  |   // Analog Comparator Interrupt: Enabled
    (0<<ACIC)  |   // Analog Comparator Input Capture: Disabled
    (0<<ACIS1) | 
    (0<ACIS0);   // Analog Comparator Interrupt Mode: Comparator Interrupt on Rising Output Edge
  
  optoloader_t opto = {
    .zero_count = 0,
    .prev_count = 0,
    .counter = 0,
    .sync_count = 0,
    .message = 0x00,
    .message_count = 0x00,
    .flags = 0
  };
  return opto;
}

void enableOptoloader(optoloader_t *opto) 
{
  opto->zero_count = 0;
  opto->prev_count = 0;
  opto->counter = 0;
  opto->sync_count = 0;
  opto->message = 0x00;
  opto->message_count = 0x00;
  opto->flags |= OPTO_ACTI_bm;
  

  // Set up Analog Compare Interrupt
  ACSR |= (1<<ACIE);
}

void runOptoloader(optoloader_t *optoloader, uint8_t *led_display)
{
  // If the sync flag is set we can do some stuff
  if ((optoloader->flags & OPTO_SYNC_bm) != 0)
    {
      // If we are getting a zero
      if (optoloader->counter > (5*optoloader->zero_count/8))
	{
	  // If both the start and sync bits are set we can start collecting messages
	  // This is a 0 so we shift in a 0
	  if (optoloader->flags & OPTO_STRT_bm)
	    {	      
	      optoloader->message <<= 1;
	      optoloader->message_count += 1;
	    }
	}
      else if (optoloader->counter > (optoloader->zero_count/16))
	{
	  // If the ones flag, ONEF is set the first part of the one message has been seen
	  if (optoloader->flags & OPTO_ONEF_bm)
	    {
	      optoloader->flags &= ~OPTO_ONEF_bm;
	      // When both parts of a 1 are observed 
	      // a one is shifted into the message buffer.
	      if (optoloader->flags & OPTO_STRT_bm)
		{
		  optoloader->message <<= 1;
		  optoloader->message |= 0x01;
		  optoloader->message_count += 1;
		}
	      else
		{
		  // The first one we see sets the start bit, it does not get shifted
		  // into the message.
		  optoloader->flags |= OPTO_STRT_bm;
		}
	    }
	  else
	    {
	      optoloader->flags |= OPTO_ONEF_bm;
	    }
	}
    }
  else // When we are not synced we need to find a sync
    {
      // If we have more than 4 successful counts taht are close to one another,
      // Accept that as our sync count.
      if (optoloader->sync_count == 4)
	{
	  optoloader->flags |= OPTO_SYNC_bm;
	  *led_display = 0x00;
	}
      else
	{
	  if ((optoloader->counter < (4*optoloader->prev_count/3)) &&
	      (optoloader->counter > (2*optoloader->prev_count/3)))
	    {
	      // Average them together if zero_count has already been set.
	      if (optoloader->zero_count > 0)
		{
		  // Add and divide by two (shift right by 1)
		  optoloader->zero_count = (optoloader->zero_count + optoloader->counter)>>1;
		}
	      else 
		{
		  optoloader->zero_count = optoloader->counter;
		}
	      optoloader->sync_count += 1;
	    } 
	  else
	    {
	      optoloader->zero_count = 0;
	      optoloader->sync_count = 0;
	    }
	  *led_display = (1<<(optoloader->sync_count + 1))|1;
	  //	  setDisplay(&led_display,(1<<(optoloader->sync_count+1))|1);
	}
    }
  optoloader->prev_count = optoloader->counter;
  optoloader->counter = 0;
}
  
