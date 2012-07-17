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
