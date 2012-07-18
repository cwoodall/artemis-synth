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
uint8_t keyboard_ctr = 0;
uint8_t sequencer_ctr = 0;

#define KEYBOARD  0x01
#define SEQUENCER 0x02
#define BLINKING  0x03
#define OPTOLOADER 0x04 // Currently not used, maintianed by optoloader.flag...
uint8_t mode = KEYBOARD;

#define SEQUENCER_LENGTH 8
uint8_t sequencer[SEQUENCER_LENGTH] = {
  0b01010101,
  0b10101010,
  0b01010101,
  0b00000010,
  0b10111000,
  0b11111111,
  0b01000000,
  0b10000001
};
uint8_t sequencer_display_i = 0;
uint8_t sequencer_metronome = 0;
uint8_t sequencer_metronome_ctr = 0;
uint8_t sequencer_flag = 0;

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

uint16_t poly_buffer[8] = {0,0,0,0,0,0,0,0};
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
	  if ((settings == 0) && (opto_enable_ctr == 0))
	    {
	      opto_enable_ctr = 1;
	      sequencer_ctr = 0;
	      keyboard_ctr = 0;
	    }
	  else if (settings)
	    {
	      opto_enable_ctr = 0;
	      if (!(settings & 0x01) && (sequencer_ctr == 0))
		{
		  sequencer_ctr = 1;
		}
	      else if (settings & 0x01)
		{
		  sequencer_ctr = 0;
		}
	      
	      if (!(settings & 0x20) && (keyboard_ctr == 0))
		{
		  keyboard_ctr = 1;
		}
	      else if (settings & 0x02)
		{
		  keyboard_ctr = 0;
		}
	    }
	  
	  if (mode == KEYBOARD)
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
	      
	      if (read_keyboard_flag)
		{
		  read_keyboard_flag = 0;
		  keyboard = readKeyboard();
		  
		  // Prevent the following array accesses to be disturbed, otherwise you get nasty sounds
		  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		  {
		    
		    poly_buffer[0] = 0;
		    poly_buffer[1] = 0;
		    poly_buffer[2] = 0;
		    poly_buffer[3] = 0;
		    poly_buffer[5] = 0;
		    poly_buffer[6] = 0;
		    poly_buffer[7] = 0;
		    
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
	  else if (mode == SEQUENCER)
	    {
	      //setDisplay(&led_display, sequencer_metronome);
	      setDisplay(&led_display, sequencer[sequencer_display_i]);
	      if (settings_debounce == 0)
		{	
		  if (((settings & 0x02) == 0) && ((settings_prev & 0x02) != 0))
		    { 
		      if(sequencer_display_i < (SEQUENCER_LENGTH-1))
			{
			  sequencer_display_i += 1;
			}
		      settings_debounce = 1;
		    }
		  if (((settings & 0x01) == 0) && ((settings_prev & 0x01) != 0))
		    {
		      if (sequencer_display_i > 0) 
			{
			  sequencer_display_i -= 1;
			}
		      settings_debounce = 1;
		    }
		}
	      poly_i = 4;
	      ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	      {
		  {
		    sequencer_flag = 0;
		    if (sequencer[0] & (1<<sequencer_metronome))
		      {
			poly_buffer[0] = C6;
		      }
		    else
		      {
			poly_buffer[0] = 0;
		      }
		    
		    if (sequencer[1] & (1<<sequencer_metronome))
		      {
			poly_buffer[1] = D6;
		      }
		    else
		      {
			poly_buffer[1] = 0;
		      }
		    
		    if (sequencer[2] & (1<<sequencer_metronome))
		      {
			poly_buffer[2] = E6;
		      }
		    else
		      {
			poly_buffer[2] = 0;
		      }
		  }
		if (sequencer[3] & (1<<sequencer_metronome))
		  {
		    poly_buffer[3] = F6;
		  }
		else
		  {
		    poly_buffer[3] = 0;
		  }
		if (sequencer[4] & (1<<sequencer_metronome))
		  {
		    poly_buffer[4] = G6;
		  }
		else
		  {
		    poly_buffer[4] = 0;
		  }
		if (sequencer[5] & (1<<sequencer_metronome))
		  {
		    poly_buffer[5] = A6;
		  }
		else
		  {
		    poly_buffer[5] = 0;
		  }
		if (sequencer[6] & (1<<sequencer_metronome))
		  {
		    poly_buffer[6] = B6;
		  }
		else
		  {
		    poly_buffer[6] = 0;
		  }
		if (sequencer[7] & (1<<sequencer_metronome))
		  {
		    poly_buffer[7] = C7;
		  }
		else
		  {
		    poly_buffer[7] = 0;
		  }
	      }
	    }
	}
      settings_prev = settings;
    }
  return 0;
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
      else if (opto_enable_ctr > 0)
	{
	  opto_enable_ctr += 1;
	}      
      
      if (sequencer_ctr == 0xFF)
	{
	  setDisplay(&led_display, 0x0F);
	  sequencer_ctr = 0;
	  mode = SEQUENCER;
	}
      else if (sequencer_ctr > 0)
	{
	  sequencer_ctr += 1;
	}

      if (keyboard_ctr == 0xFF)
	{
	  setDisplay(&led_display, 0xF0);
	  keyboard_ctr = 0;
	  mode = KEYBOARD;
	}
      else if (keyboard_ctr > 0)
	{
	  keyboard_ctr += 1;
	}

      if (mode == SEQUENCER)
	{
	  sequencer_metronome_ctr += 1;

	  if ((sequencer_metronome_ctr >= 0x3F) && !(sequencer_metronome % 2))
	    {
	      sequencer_metronome += 1;
	      sequencer_metronome_ctr = 0;
	      sequencer_flag = 1;
	    }
	  else if ((sequencer_metronome_ctr >= 0x38))
	    {
	      sequencer_metronome += 1;
	      sequencer_metronome_ctr = 0;
	      sequencer_flag = 1;
	    }


	  if (sequencer_metronome > 7)
	    {
	      sequencer_flag = 1;
	      sequencer_metronome = 0;
	    }
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

  // Get optoloader information
  runOptoloader(&optoloader, &led_display);

  // When we have a full byte, read it out and display it.
  if (optoloader.message_count == 8) 
    {
      optoloader.message_count = 0;
      led_display = optoloader.message;
    }

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
  static uint16_t inc[8] = {0, 0, 0, 0,0,0,0,0};
  static uint16_t final = 0;

  INCREMENT_NOTE(poly_buffer[0], inc[0]);
  INCREMENT_NOTE(poly_buffer[1], inc[1]);
  INCREMENT_NOTE(poly_buffer[2], inc[2]);
  INCREMENT_NOTE(poly_buffer[3], inc[3]);
  INCREMENT_NOTE(poly_buffer[4], inc[4]);
  INCREMENT_NOTE(poly_buffer[5], inc[5]);
  INCREMENT_NOTE(poly_buffer[6], inc[6]);
  INCREMENT_NOTE(poly_buffer[7], inc[7]);
  
  final = (GET_WAVE_VALUE(channelC, 0) + 
           GET_WAVE_VALUE(channelC, 1) +
           GET_WAVE_VALUE(channelC, 2) +
           GET_WAVE_VALUE(channelC, 3) +
           GET_WAVE_VALUE(channelC, 4) +
           GET_WAVE_VALUE(channelC, 5) +
           GET_WAVE_VALUE(channelC, 6) +
           GET_WAVE_VALUE(channelC, 7))/poly_i;

  writeMCP492x(final, MCP492x_CONFIG);
}
