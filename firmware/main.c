/** 
 *  Artemis Synthesizer v2.00 Firmware
 *  Date: 2013-07-15  
 *
 *  Copyright (c) 2012-2013 Christopher Woodall 
 *  MIT License
 */
#define F_CPU 8000000 // Used to run at 16MHz, but removed XTAL now runs at 8kHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/eeprom.h> 

#include "HRL_SPI.h"
#include "HRL_MCP492x.h"
#define MCP492x_CONFIG 0x30 // Define configuration bits for MCP4921, that we are going to use

#include "notes.h"
#include "inst.h"
#include "buttons.h"
#include "leds.h"
#include "optoloader.h"

uint8_t opto_enable_ctr = 0;
uint8_t keyboard_ctr = 0;
uint8_t sequencer_ctr = 0;
uint8_t msg_ctr = 0;
uint8_t speed = 0x5F;

#define KEYBOARD  0x01
#define SEQUENCER 0x02
#define BLINKING  0x03
#define OPTOLOADER 0x04 // Currently not used, maintianed by optoloader.flag...
uint8_t mode = KEYBOARD;

#define SEQUENCER_LENGTH 8
#define TOTAL_SEQUENCES 8
uint8_t sequencer_i = 0;

//#define SEQUENCER_ee (const void*)12
uint8_t EEMEM sequencer_ee[TOTAL_SEQUENCES][SEQUENCER_LENGTH] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
};

uint8_t EEMEM first_ee = 0x00;

uint8_t sequencer[TOTAL_SEQUENCES][SEQUENCER_LENGTH] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
};

uint8_t sequencer_display_i = 0;
uint8_t sequencer_metronome = 0;
uint8_t sequencer_metronome_ctr = 0;
uint8_t sequencer_flag = 0;

// WAVE/HARMONICS stuff
uint16_t harmonics[256];
uint16_t harmonics_amplitudes[5] = {0,0,0,0,0};

// LED, Optoloader and Keyboard globals
uint8_t led_refresh_flag = 0;
uint8_t led_display = 0x00000000;
optoloader_t optoloader;
uint8_t read_keyboard_flag = 0;
uint8_t settings_debounce = 0;

#define MAX_LOOKUP  0x7FFF
// Change to inline function
#define GET_WAVE_VALUE(inst, num) (poly_buffer[num]?inst[inc[num]>>NOTES_BASE]:0)
// Change to inline function
#define INCREMENT_NOTE(note, i)                 \
	if (note)									\
    {                                           \
		if (i + note < MAX_LOOKUP)				\
        {                                       \
			i += note;							\
        }                                       \
		else									\
        {                                       \
			i = note - (MAX_LOOKUP - i);		\
        }                                       \
    }                                           \
	else										\
    {                                           \
		i = 0;									\
    }							

// Setup Sample rate at 11kHz, externally we filter at 5.5kHz
// this satisfies Nyquist and technically we should be able
// to accurately reconstruct waves up to 5.5kHz, more than enough 
// for this application.
void setupSampleRate11kHz();

// Setup controls timer at a much lower frequency (near 300Hz)
void setupControlTimer();

#define SCALE_LENGTH 8
uint8_t scale_i = 0;
uint16_t scales[4][SCALE_LENGTH] = {
  {C5, D5, E5, F5, G5, A5, B5, C6},
  {C5, D5, E5, G5, A5, C6, D6, E6},
  {C5, Ds5, F5, Fs5, G5, As5, C6, Ds6},
  {C5, D5, Ds5, F5, G5, A5, As5, C6}
};

uint16_t poly_buffer[8] = {0,0,0,0,0,0,0,0};
uint8_t poly_i = 0;
uint8_t key_pressed;
uint8_t debounce = 0;

inline uint8_t shiftOnes(uint8_t a) 
{
	uint8_t output = 0;
	uint8_t i = 0;
	for (i = 0; i < a; i++) {
		output |= (1<<i);
	}
	return output;
}

int main(void)
{
	// Disable Interrupts for setup
	cli();
	
	// By default load a sine_table wave into harmonic 
	// [*] replace with EEPROM reads
	for (int i = 0; i < 256; i++) {
		harmonics[i] = sine_table[i];
    }
	
	first_ee = eeprom_read_byte(&first_ee);
	if (first_ee == 0xFF) {
		eeprom_write_byte(&first_ee,0x33);
		eeprom_write_block((const void *)sequencer, (void *)sequencer_ee, 64);
    } else {
		// Load up sequencer information from ee_prom
		eeprom_read_block((void*)sequencer, (const void *)sequencer_ee, 64);
    }
	
	// Setup Timer2, which is our "control timer"
	setupControlTimer();
	
	// SETUP BUTTON INPUTS
	setupButtons();
	
	// Setup the SPI Bus
	setupSPI(SPI_MODE_0, SPI_MSB, SPI_NO_INTERRUPT, SPI_MASTER_CLK2);
	SETUP_DAC;
	
	// Setup Timer1 to work at 22KHz sample rate
	// This is our sample rate for the output audio
	setupSampleRate11kHz(); 
  
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
	uint8_t keyboard_prev = 0;
	
	
	sei(); // Enable Interrupts now that we are all ready to go
	for(;;) {
		updateDisplay(&led_display, &led_refresh_flag);
		
		settings = readSettings();
		// Check if we are in Optoloader mode (active Optoloader)
		if (optoloader.flags & OPTO_ACTI_bm) {
			if (settings_debounce == 0) {	
				if ((settings == 0) && (settings_prev != 0)) {
					opto_enable_ctr = 1;
					settings_debounce = 1;
				} else if ((settings != 0) && (settings_prev == 0))	{
					settings_debounce = 1;
					opto_enable_ctr = 0;
				}
			}	  
		} else {
			if ((settings == 0) && (opto_enable_ctr == 0)) {
				opto_enable_ctr = 1;
				sequencer_ctr = 0;
				keyboard_ctr = 0;
			} else if (settings) {
				opto_enable_ctr = 0;
				if (!(settings & 0x02) && (sequencer_ctr == 0))	{
					sequencer_ctr = 1;
				} else if (settings & 0x02)	{
					sequencer_ctr = 0;
				}
				if (!(settings & 0x01) && (keyboard_ctr == 0)) {
					keyboard_ctr = 1;
				} else if (settings & 0x01)	{
					keyboard_ctr = 0;
				}
			}
	  
			if (mode == KEYBOARD) {
				if (settings_debounce == 0)	{	
					if (((settings & 0x01) == 0) && ((settings_prev & 0x01) != 0)) { 
						if (scale_i < 3) {
							scale_i += 1;
						} else {
							scale_i = 0;
						}
						settings_debounce = 1;
						setDisplay(&led_display, shiftOnes(scale_i+1));
					}
					if (((settings & 0x02) == 0) && ((settings_prev & 0x02) != 0)) {
						if (scale_i > 0) {
							scale_i -= 1;
						} else {
							scale_i = 3;
						}
						setDisplay(&led_display, shiftOnes(scale_i+1));		  
						settings_debounce = 1;
					}
				}
				if (read_keyboard_flag) {
					read_keyboard_flag = 0;
					keyboard = readKeyboard();
					
					// Prevent the following array accesses to be disturbed, otherwise you get nasty sounds
					ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
						poly_buffer[0] = 0;
						poly_buffer[1] = 0;
						poly_buffer[2] = 0;
						poly_buffer[3] = 0;
						poly_buffer[4] = 0;
						poly_buffer[5] = 0;
						poly_buffer[6] = 0;
						poly_buffer[7] = 0;
						
						poly_i = 0;
						
						for (uint8_t j = 0; j < 8; j++) {
							if (((keyboard & (0x01 << j)) == 0) && (poly_i <= 3)) {
								poly_buffer[poly_i] = scales[scale_i][j];
								poly_i += 1;
							}
						}
					}
				}
			} else if (mode == SEQUENCER) {
				//setDisplay(&led_display, sequencer_metronome);
				setDisplay(&led_display, shiftOnes(sequencer_i+1));
				
				keyboard = readKeyboard();
				if (settings_debounce == 0)	{
					if (((settings & 0x02) == 0) && ((settings_prev & 0x02) != 0)) { 
						if (speed < 0xCF) {
							speed += 0x10;
						} else {
							speed = 0xCF;
						}
						settings_debounce = 1;
						setDisplay(&led_display, shiftOnes(scale_i+1));
					}

					if (((settings & 0x01) == 0) && ((settings_prev & 0x01) != 0)) {
						if (speed > 0x2F) {
							speed -= 0x10;
						} else {
							speed = 0x2F;
						}
						settings_debounce = 1;
					} 

					if (((keyboard & 0x01)==0) && ((keyboard_prev & 0x01) != 0)) {
						sequencer_i = 0;
						settings_debounce = 1;
					}
					if (((keyboard & 0x02)==0) && ((keyboard_prev & 0x02) != 0)) {
						sequencer_i = 1;
						settings_debounce = 1;
					}
					if (((keyboard & 0x04)==0) && ((keyboard_prev & 0x04) != 0)) {
						sequencer_i = 2;
						settings_debounce = 1;
					}

					if (((keyboard & 0x08)==0) && ((keyboard_prev & 0x08) != 0)) {
						sequencer_i = 3;
						settings_debounce = 1;
					}

					if (((keyboard & 0x10)==0) && ((keyboard_prev & 0x10) != 0)) {
						sequencer_i = 4;
						settings_debounce = 1;
					}

					if (((keyboard & 0x20)==0) && ((keyboard_prev & 0x20) != 0)) {
						sequencer_i = 5;
						settings_debounce = 1;
					} 

					if (((keyboard & 0x40)==0) && ((keyboard_prev & 0x40) != 0)) {
						sequencer_i = 6;
						settings_debounce = 1;
					} 

					if (((keyboard & 0x80)==0) && ((keyboard_prev & 0x80) != 0)) {
						sequencer_i = 7;
						settings_debounce = 1;
					}
				}
				keyboard_prev = keyboard;
				poly_i = 4;
				
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
					sequencer_flag = 0;
					if (sequencer[sequencer_i][sequencer_metronome] & (1<<0)) {
						poly_buffer[0] = C5;
					} else {
						poly_buffer[0] = 0;
					} 
					if (sequencer[sequencer_i][sequencer_metronome] & (1<<1)) {
						poly_buffer[1] = D5;
					} else {
						poly_buffer[1] = 0;
					}
					if (sequencer[sequencer_i][sequencer_metronome] & (1<<2))	{
						poly_buffer[2] = E5;
					} else {
						poly_buffer[2] = 0;
					}
					if (sequencer[sequencer_i][sequencer_metronome] & (1<<3))	{
						poly_buffer[3] = F5;
					} else {
						poly_buffer[3] = 0;
					}
					if (sequencer[sequencer_i][sequencer_metronome] & (1<<4))	{
						poly_buffer[4] = G5;
					} else {
						poly_buffer[4] = 0;
					} 
					if (sequencer[sequencer_i][sequencer_metronome] & (1<<5))	{
						poly_buffer[5] = A5;
					} else {
						poly_buffer[5] = 0;
					}
					if (sequencer[sequencer_i][sequencer_metronome] & (1<<6))	{
						poly_buffer[6] = B5;
					} else {
						poly_buffer[6] = 0;
					}
					if (sequencer[sequencer_i][sequencer_metronome] & (1<<7))	{
						poly_buffer[7] = C6;
					} else {
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
	if (optoloader.flags & OPTO_ACTI_bm) {
		optoloader.counter += 1;
		if (opto_enable_ctr == 0xFF) {
			optoloader.flags &= ~OPTO_ACTI_bm;
			setDisplay(&led_display, shiftOnes(scale_i+1));
			disableOptoloader(&optoloader);
			opto_enable_ctr = 0;
			
			eeprom_write_block((const void *)sequencer, (void *)sequencer_ee, 64);

			setupSampleRate11kHz();
		} 
		
		if (opto_enable_ctr > 0) {
			opto_enable_ctr += 1;
		}      
	} else {
		if (opto_enable_ctr == 0xFF) {
			optoloader.flags |= OPTO_ACTI_bm;
			setDisplay(&led_display, 0xFF);
			TCNT2 = 0;      
			TCCR1B = 0;
			enableOptoloader(&optoloader);
			opto_enable_ctr = 0;
			msg_ctr = 0;
		} else if (opto_enable_ctr > 0) {
			opto_enable_ctr += 1;
		}

		if (sequencer_ctr == 0xFF) {
			setDisplay(&led_display, 0x0F);
			sequencer_ctr = 0;
			mode = SEQUENCER;
		} else if (sequencer_ctr > 0) {
			sequencer_ctr += 1;
		}

		if (keyboard_ctr == 0xFF) {
			setDisplay(&led_display, 0xF0);
			keyboard_ctr = 0;
			mode = KEYBOARD;
		} else if (keyboard_ctr > 0) {
			keyboard_ctr += 1;
		}

		if (mode == SEQUENCER) {
			sequencer_metronome_ctr += 1;
			if ((sequencer_metronome_ctr >= speed) && !(sequencer_metronome % 2)) {
				sequencer_metronome += 1;
				sequencer_metronome_ctr = 0;
				sequencer_flag = 1;
			} else if ((sequencer_metronome_ctr >= (speed-0x05))) {
				sequencer_metronome += 1;
				sequencer_metronome_ctr = 0;
				sequencer_flag = 1;
			} 

			if (sequencer_metronome > 7){
				sequencer_flag = 1;
				sequencer_metronome = 0;
			}
		}
	}

	if (settings_debounce == 30) {
		settings_debounce = 0;
	} else if (settings_debounce >= 1) {
		settings_debounce += 1;
	}

	led_refresh_flag ^= 0x01;
	led_refresh_flag |= 0x02;
}

ISR(ANALOG_COMP_vect)
{
	static uint8_t num_bytes = 0; // number of bytes expected to be received

	static uint32_t temp_harmonics = 0;
	static uint8_t msg_mode = 0; 
	// Turn off Timer 2 Interrupt A
	TIMSK2 &= ~(1<<OCIE2A);
	// Reset Timer2 Counter
	TCNT2 = 0;

	// Get optoloader information
	runOptoloader(&optoloader, &led_display);

	// When we have a full byte, read it out and display it.
	if (optoloader.message_count == 8) {
		optoloader.message_count = 0;
		led_display = optoloader.message;

		// NEED TO FINISH WRITING MESSAGE INTERPRETATION STUFF
		if (msg_ctr == 0) {
			if (optoloader.message & SEQUENCER_MSG) {
				// if we are getting a sequencer message do something here
				msg_mode = SEQUENCER_MSG;
			} else if (optoloader.message & HARMONICS_MSG) {
				// if we are getting a harmonics message do the appropraite thing here
				msg_mode = HARMONICS_MSG;
			} //else {
				// ERROR STATE!!!
//			}
			// How many bytes before epilogue?
			num_bytes = 0x0F & optoloader.message;
		} else if (msg_ctr > num_bytes)	{
			if (optoloader.message == EPILOGUE_MSG)	{ 
				if (msg_mode == HARMONICS_MSG)	{
					for (uint16_t i = 0; i < 256; i++) {
						// Simplify this code...
						// Also % 256 -> and 255 (& 255)
						temp_harmonics = (((harmonics_amplitudes[0]*(sine_table[i]<<2))>>4) + 
															((harmonics_amplitudes[1]*(sine_table[(i*2)%256]<<2))>>4) + 
															((harmonics_amplitudes[2]*(sine_table[(i*4)%256]<<2))>>4) + 
															((harmonics_amplitudes[3]*(sine_table[(i*8)%256]<<2))>>4) + 
															((harmonics_amplitudes[4]*(sine_table[(i*16)%256]<<2))>>4))/5;

						harmonics[i] = (uint16_t) temp_harmonics;
					}
				}
				opto_enable_ctr = 0xFF;
			}
		} else if (msg_ctr > 0) {
			if (msg_mode == SEQUENCER_MSG) {
				sequencer[sequencer_i][msg_ctr-1] = optoloader.message;
			} else if (msg_mode == HARMONICS_MSG) {
				harmonics_amplitudes[(optoloader.message>>5)] = (optoloader.message>>2) & 0x07;
			}
		}
		msg_ctr += 1;
	}
	
	TIMSK2 |= (1<<OCIE2A);
}

void setupSampleRate11kHz()
{    
	TCCR1A = 0;     // set entire TCCR1A register to 0
	TCCR1B = 0;     // same for TCCR1B
	
	// set compare match register to desired timer count:
	// We want to compare at 11kHz
	// target time/timer resolution
	// (1/11kHz)/(1/8MHz) = 727
	OCR1A = 727; 
	
	// turn on CTC mode:
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS10); // No prescaler
	// enable timer compare interrupt:
	TIMSK1 |= (1 << OCIE1A);
}

   
ISR(TIMER1_COMPA_vect) 
{
	static uint16_t inc[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	static uint16_t final = 0;
	
	INCREMENT_NOTE(poly_buffer[0], inc[0]);
	INCREMENT_NOTE(poly_buffer[1], inc[1]);
  INCREMENT_NOTE(poly_buffer[2], inc[2]);
	INCREMENT_NOTE(poly_buffer[3], inc[3]);
	INCREMENT_NOTE(poly_buffer[4], inc[4]);
	INCREMENT_NOTE(poly_buffer[5], inc[5]);
	INCREMENT_NOTE(poly_buffer[6], inc[6]);
	INCREMENT_NOTE(poly_buffer[7], inc[7]);
	
	final = (GET_WAVE_VALUE(harmonics, 0) + 
					 GET_WAVE_VALUE(harmonics, 1) +
					 GET_WAVE_VALUE(harmonics, 2) +
					 GET_WAVE_VALUE(harmonics, 3) +
					 GET_WAVE_VALUE(harmonics, 4) +
					 GET_WAVE_VALUE(harmonics, 5) +
					 GET_WAVE_VALUE(harmonics, 6) +
					 GET_WAVE_VALUE(harmonics, 7))>>2;
	
	writeMCP492x(final, MCP492x_CONFIG);
}
