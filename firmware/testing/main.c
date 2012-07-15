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

#define LED_PORT PORTC
#define LED_DDR DDRC
#define LED0_m ~(0x08)
#define LED1_m 0x08
#define LED2_m ~(0x04)
#define LED3_m 0x04
#define LED4_m ~(0x02)
#define LED5_m 0x02
#define LED6_m ~(0x01)
#define LED7_m 0x01

uint8_t led_display = 0x00000000;

void setupDisplay()
{
  LED_PORT &= 0xF0;
  LED_DDR &= 0xF0;
}

void updateDisplay(uint8_t new_display)
{
  led_display = new_display;
}

#define MAX_LOOKUP  0x7FFF
#define INCREMENT_NOTE(note, i) 			\
  if (note)                                 \
    {                                       \
       if (i + note < MAX_LOOKUP)			\
        {                                   \
          i += note;                        \
        }                                   \
      else                                  \
        {                                   \
          i = note - (MAX_LOOKUP - i);      \
        }                                   \
    }                                       \
  else                                      \
    {                                       \
      i = 0;                                \
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
uint8_t settings_debounce = 0;

int main(void) 
{
  // Disable Interrupts for setup
  cli();
  
  // SETUP BUTTON INPUTS
  setupButtons();
  
  setupDisplay();

  // Setup the SPI Bus
  setupSPI(SPI_MODE_0, SPI_MSB, SPI_NO_INTERRUPT, SPI_MASTER_CLK2);
  SETUP_DAC;
  setupSampleRate(22000); // Setup Timer1 to work at 22KHz sample rate
  setupControlTimer();

  uint8_t settings = 0;
  uint8_t settings_prev = 0;

  led_display = 0b11111111;

  sei(); // Enable Interrupts now that we are all ready to go

  for(;;) 
    {      
        settings = readSettings();
        if (settings_debounce == 0)
        {

          if (((settings & 0x02) == 0) && ((settings_prev & 0x02) != 0))
            { 
              led_display <<= 1;
              led_display |= 0x01;
              if (scale_i < 3)
                {
                  scale_i += 1;
                }
              else
                {
                  scale_i = 0;
                }
              settings_debounce = 1;
            }
          if (((settings & 0x01) == 0) && ((settings_prev & 0x01) != 0))
            {
              led_display >>= 1;
              if (scale_i > 0) 
                {
                  scale_i -= 1;
                }
              else
                {
                  scale_i = 3;
                }
              settings_debounce = 1;
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
  // target_freq = 120 Hz
  // timer_res = 1/16MHz
  // prescaler = 1024
  // timer_counts = (1.0/target_freq)/(timer_res*prescaler) - 1
  OCR2A = 129; 

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
  static uint8_t keyboard = 0;
  static uint8_t j = 0; 
  static uint8_t led_refresh_flag = 0;

  keyboard = readKeyboard();
  poly_buffer[0] = 0;
  poly_buffer[1] = 0;
  poly_buffer[2] = 0;
  poly_buffer[3] = 0;
  poly_i = 0;
  
  for (j = 0; j < 8; j++) 
    {
      if (((keyboard & (0x01 << j)) == 0) && (poly_i <= 3))
        {
          poly_buffer[poly_i] = scales[scale_i][j];
          poly_i += 1;
        }
    }
  
  if (settings_debounce > 0)
    {
      (settings_debounce < 20)?(settings_debounce += 1):(settings_debounce = 0);
    }
  

  PORTC &= 0xF0;
  DDRC &= 0xF0;
  if (led_refresh_flag)
    {
      led_refresh_flag = 0;
      if (led_display & 1)
        {
          PORTC &= LED0_m;
          DDRC |= ~LED0_m;
        }
      else
        {
          DDRC &= LED0_m;
        }
      if ((led_display & (1<<2)) != 0)
        {
          PORTC &= LED2_m;
          DDRC |= ~LED2_m;
        }
      else
        {
          DDRC &= LED2_m;
        }
      if ((led_display & (1<<4)) != 0)
        {
          PORTC &= LED4_m;
          DDRC |= ~LED4_m;
        }
      else
        {
          DDRC &= LED4_m;
        }
      if ((led_display & (1<<6)) != 0)
        {
          PORTC &= LED6_m;
          DDRC |= ~LED6_m;
        }
      else
        {
          DDRC &= LED6_m;
        }
    }
  else
    {
      led_refresh_flag = 0xFF;
      if (led_display & (1<<1))
        {
          DDRC |= LED1_m;
          PORTC |= LED1_m;
        }
      else
        {
          DDRC &= ~LED1_m;
        }

      if (led_display & (1<<3))
        {
          DDRC |= LED3_m;
          PORTC |= LED3_m;
        }
      else
        {
            DDRC &= ~LED3_m;
        }
      if (led_display & (1<<5))
        {
          DDRC |= LED5_m;
          PORTC |= LED5_m;
        }
      else
        {
          DDRC &= ~LED5_m;
        }
      if (led_display & (1<<7))
        {
          DDRC |= LED7_m;
          PORTC |= LED7_m;
        }
      else
        {
          DDRC &= ~LED7_m;
        }
    }
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
  
  final = (GET_WAVE_VALUE(channelA, 0) + 
           GET_WAVE_VALUE(channelA, 1) +
           GET_WAVE_VALUE(channelA, 2) +
           GET_WAVE_VALUE(channelA, 3))/poly_i;

  writeMCP492x(final, MCP492x_CONFIG);
}
