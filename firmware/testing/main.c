#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
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
uint8_t read_keyboard_flag = 0;
uint8_t settings_debounce = 0;

#define MAX_LOOKUP  0x7FFF
#define INCREMENT_NOTE(note, i) 			\
  if (note)						\
    {							\
      if (i + note < MAX_LOOKUP)			\
        {						\
          i += note;					\
        }						\
      else						\
        {						\
          i = note - (MAX_LOOKUP - i);			\
        }						\
    }							\
  else							\
    {							\
      i = 0;						\
    }							

#define FREQUENCY_TO_COUNT(x) (F_CPU/1000)/(x/1000)

// Setup Sample rate at 22kHz, externally we filter at 11kHz
// this satisfies Nyquist and technically we should be able
// to accurately reconstruct waves up to 11kHz
void setupSampleRate();

void setupControlTimer();

#define SCALE_LENGTH 8
uint8_t scale_i = 0;
uint16_t scales[4][SCALE_LENGTH] = {
  {C6, D6, E6, F6, G6, A6, B6, C7},
  {C5, D5, E5, G5, A5, C6, D6, E6},
  {C5, Ds5, F5, Fs5, G5, As5, C6, Ds6},
  {C5, D5, Ds5, F5, G5, A5, As5, C6}
};

uint16_t poly_buffer[4] = {0,0,0,0};
uint8_t poly_i = 0;
uint8_t key_pressed;
uint8_t debounce = 0;

uint8_t shiftOnes(uint8_t a)
{
  uint8_t output = 0;
  uint8_t i = 0;
  for (i = 0; i < a; i++)
    {
      output |= (1<<i);
    }
  return output;
}

int main(void)
{
  // Disable Interrupts for setup
  cli();
  
  // Setup Timer2, which is our "control timer"
  setupControlTimer();

  // SETUP BUTTON INPUTS
  setupButtons();

  // Setup the SPI Bus
  setupSPI(SPI_MODE_0, SPI_MSB, SPI_NO_INTERRUPT, SPI_MASTER_CLK2);
  SETUP_DAC;
  
  // Setup Timer1 to work at 22KHz sample rate
  // This is our sample rate for the output audio
  setupSampleRate(22000); 
  
  
  // Initialize display for keyboard mode
  led_display = setupDisplay();
  setDisplay(&led_display, shiftOnes(scale_i+1));  

  // Initialize Optoloader and then turn it off
  optoloader = setupOptoloader();
  disableOptoloader(&optoloader);

  // For storing settings button presses
  uint8_t settings = 0;
  uint8_t settings_prev = 0;
  uint8_t keyboard = 0;

  sei(); // Enable Interrupts now that we are all ready to go
  for(;;) 
    {
      updateDisplay(&led_display, &led_refresh_flag);

      settings = readSettings();
      // Check if we are in Optoloader mode (active Optoloader)
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
	      if (((settings & 0x02) == 0) && ((settings_prev & 0x02) != 0))
		{ 
		  if (scale_i < 3)
		    {
		      scale_i += 1;
		    }
		  else
		    {
		      scale_i = 0;
		    }
		  settings_debounce = 1;
		  setDisplay(&led_display, shiftOnes(scale_i+1));
		}
	      if (((settings & 0x01) == 0) && ((settings_prev & 0x01) != 0))
		{
		  if (scale_i > 0) 
		    {
		      scale_i -= 1;
		    }
		  else
		    {
		      scale_i = 3;
		    }
		  setDisplay(&led_display, shiftOnes(scale_i+1));		  
		  settings_debounce = 1;
		}
	    }

	  if ((settings == 0) && (opto_enable_ctr == 0))
	    {
	      opto_enable_ctr = 1;
	    }
	  else if (settings)
	    {
	      opto_enable_ctr = 0;
	    }

	  if (read_keyboard_flag)
	    {
	      ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	        {
		  read_keyboard_flag = 0;
		  keyboard = readKeyboard();
		  poly_buffer[0] = 0;
		  poly_buffer[1] = 0;
		  poly_buffer[2] = 0;
		  poly_buffer[3] = 0;
		  poly_i = 0;
		  
		  for (uint8_t j = 0; j < 8; j++) 
		    {
		      if (((keyboard & (0x01 << j)) == 0) && (poly_i <= 3))
			{
			  poly_buffer[poly_i] = scales[scale_i][j];
			  poly_i += 1;
			}
		    }
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
  read_keyboard_flag = 1;
  if (optoloader.flags & OPTO_ACTI_bm)
    {
      optoloader.counter += 1;
      if (opto_enable_ctr == 0xFF)
	{
	  optoloader.flags &= ~OPTO_ACTI_bm;
	  setDisplay(&led_display, shiftOnes(scale_i+1));
	  disableOptoloader(&optoloader);
	  opto_enable_ctr = 0;
	  
	  setupSampleRate(22000);
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
	  TCCR1B = 0;
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

  runOptoloader(&optoloader, &led_display);
  // If the sync flag is set we can do some stuff
  /*if ((optoloader.flags & OPTO_SYNC_bm) != 0)
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
	  setDisplay(&led_display,(1<<(optoloader.sync_count+1))|1);
	}
    }
*/
  if (optoloader.message_count == 8) 
    {
      optoloader.message_count = 0;
      led_display = optoloader.message;
    }
  /*
  optoloader.prev_count = optoloader.counter;
  optoloader.counter = 0;
  */
  TIMSK2 |= (1<<OCIE2A);
}

void setupSampleRate(uint16_t frequency)
{    
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  
  // set compare match register to desired timer count:
  // We want to compare at 22kHz
  // target time/timer resolution
  // (1/22kHz)/(1/16MHz) = 727
  //OCR1A = 727; 
  OCR1A = (uint16_t) FREQUENCY_TO_COUNT(frequency);

  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS10); // No prescaler
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts:
}

#define GET_WAVE_VALUE(inst, num) (poly_buffer[num]?inst[inc[num]>>NOTES_BASE]:0)

ISR(TIMER1_COMPA_vect) 
{
  static uint16_t inc[4] = {0, 0, 0, 0};
  static uint16_t final = 0;

  INCREMENT_NOTE(poly_buffer[0], inc[0]);
  INCREMENT_NOTE(poly_buffer[1], inc[1]);
  INCREMENT_NOTE(poly_buffer[2], inc[2]);
  INCREMENT_NOTE(poly_buffer[3], inc[3]);
  
  final = (GET_WAVE_VALUE(channelC, 0) + 
           GET_WAVE_VALUE(channelC, 1) +
           GET_WAVE_VALUE(channelC, 2) +
           GET_WAVE_VALUE(channelC, 3))/poly_i;

  writeMCP492x(final, MCP492x_CONFIG);
}
