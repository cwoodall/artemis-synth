#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "HRL_SPI.h"
#include "HRL_MCP492x.h"
#include "notes.h"



// Sine Wave
uint16_t channelA[256] = { 2047, 2097, 2147, 2198, 2248, 2298, 2347, 2397, 2446, 2496, 2545, 2593, 2641, 2689, 2737, 2784, 2831, 2877, 2922, 2968, 3012, 3056, 3100, 3142, 3185, 3226, 3267, 3307, 3346, 3384, 3422, 3459, 3495, 3530, 3564, 3597, 3630, 3661, 3692, 3721, 3749, 3777, 3803, 3829, 3853, 3876, 3898, 3919, 3939, 3957, 3975, 3991, 4006, 4020, 4033, 4045, 4055, 4064, 4072, 4079, 4085, 4089, 4092, 4094, 4095, 4094, 4092, 4089, 4085, 4079, 4072, 4064, 4055, 4045, 4033, 4020, 4006, 3991, 3975, 3957, 3939, 3919, 3898, 3876, 3853, 3829, 3803, 3777, 3749, 3721, 3692, 3661, 3630, 3597, 3564, 3530, 3495, 3459, 3422, 3384, 3346, 3307, 3267, 3226, 3185, 3142, 3100, 3056, 3012, 2968, 2922, 2877, 2831, 2784, 2737, 2689, 2641, 2593, 2545, 2496, 2446, 2397, 2347, 2298, 2248, 2198, 2147, 2097, 2047, 1997, 1947, 1896, 1846, 1796, 1747, 1697, 1648, 1598, 1549, 1501, 1453, 1405, 1357, 1310, 1263, 1217, 1172, 1126, 1082, 1038, 994, 952, 909, 868, 827, 787, 748, 710, 672, 635, 599, 564, 530, 497, 464, 433, 402, 373, 345, 317, 291, 265, 241, 218, 196, 175, 155, 137, 119, 103, 88, 74, 61, 49, 39, 30, 22, 15, 9, 5, 2, 0, 0, 0, 2, 5, 9, 15, 22, 30, 39, 49, 61, 74, 88, 103, 119, 137, 155, 175, 196, 218, 241, 265, 291, 317, 345, 373, 402, 433, 464, 497, 530, 564, 599, 635, 672, 710, 748, 787, 827, 868, 909, 952, 994, 1038, 1082, 1126, 1172, 1217, 1263, 1310, 1357, 1405, 1453, 1501, 1549, 1598, 1648, 1697, 1747, 1796, 1846, 1896, 1947, 1997 };

//Square Wave
uint16_t channelB[256] = { 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

//Sawtooth Wave
uint16_t channelC[256] = { 0, 15, 31, 47, 63, 79, 95, 111, 127, 143, 159, 175, 191, 207, 223, 239, 255, 271, 287, 303, 319, 335, 351, 367, 383, 399, 415, 431, 447, 463, 479, 495, 511, 527, 543, 559, 575, 591, 607, 623, 639, 655, 671, 687, 703, 719, 735, 751, 767, 783, 799, 815, 831, 847, 863, 879, 895, 911, 927, 943, 959, 975, 991, 1007, 1023, 1039, 1055, 1071, 1087, 1103, 1119, 1135, 1151, 1167, 1183, 1199, 1215, 1231, 1247, 1263, 1279, 1295, 1311, 1327, 1343, 1359, 1375, 1391, 1407, 1423, 1439, 1455, 1471, 1487, 1503, 1519, 1535, 1551, 1567, 1583, 1599, 1615, 1631, 1647, 1663, 1679, 1695, 1711, 1727, 1743, 1759, 1775, 1791, 1807, 1823, 1839, 1855, 1871, 1887, 1903, 1919, 1935, 1951, 1967, 1983, 1999, 2015, 2031, 2047, 2063, 2079, 2095, 2111, 2127, 2143, 2159, 2175, 2191, 2207, 2223, 2239, 2255, 2271, 2287, 2303, 2319, 2335, 2351, 2367, 2383, 2399, 2415, 2431, 2447, 2463, 2479, 2495, 2511, 2527, 2543, 2559, 2575, 2591, 2607, 2623, 2639, 2655, 2671, 2687, 2703, 2719, 2735, 2751, 2767, 2783, 2799, 2815, 2831, 2847, 2863, 2879, 2895, 2911, 2927, 2943, 2959, 2975, 2991, 3007, 3023, 3039, 3055, 3071, 3087, 3103, 3119, 3135, 3151, 3167, 3183, 3199, 3215, 3231, 3247, 3263, 3279, 3295, 3311, 3327, 3343, 3359, 3375, 3391, 3407, 3423, 3439, 3455, 3471, 3487, 3503, 3519, 3535, 3551, 3567, 3583, 3599, 3615, 3631, 3647, 3663, 3679, 3695, 3711, 3727, 3743, 3759, 3775, 3791, 3807, 3823, 3839, 3855, 3871, 3887, 3903, 3919, 3935, 3951, 3967, 3983, 3999, 4015, 4031, 4047, 4063, 4079 };

// Triangle Wave
uint16_t channelD[256] = { 0, 31, 63, 95, 127, 159, 191, 223, 255, 287, 319, 351, 383, 415, 447, 479, 511, 543, 575, 607, 639, 671, 703, 735, 767, 799, 831, 863, 895, 927, 959, 991, 1023, 1055, 1087, 1119, 1151, 1183, 1215, 1247, 1279, 1311, 1343, 1375, 1407, 1439, 1471, 1503, 1535, 1567, 1599, 1631, 1663, 1695, 1727, 1759, 1791, 1823, 1855, 1887, 1919, 1951, 1983, 2015, 2047, 2079, 2111, 2143, 2175, 2207, 2239, 2271, 2303, 2335, 2367, 2399, 2431, 2463, 2495, 2527, 2559, 2591, 2623, 2655, 2687, 2719, 2751, 2783, 2815, 2847, 2879, 2911, 2943, 2975, 3007, 3039, 3071, 3103, 3135, 3167, 3199, 3231, 3263, 3295, 3327, 3359, 3391, 3423, 3455, 3487, 3519, 3551, 3583, 3615, 3647, 3679, 3711, 3743, 3775, 3807, 3839, 3871, 3903, 3935, 3967, 3999, 4031, 4063, 4095, 4063, 4031, 3999, 3967, 3935, 3903, 3871, 3839, 3807, 3775, 3743, 3711, 3679, 3647, 3615, 3583, 3551, 3519, 3487, 3455, 3423, 3391, 3359, 3327, 3295, 3263, 3231, 3199, 3167, 3135, 3103, 3071, 3039, 3007, 2975, 2943, 2911, 2879, 2847, 2815, 2783, 2751, 2719, 2687, 2655, 2623, 2591, 2559, 2527, 2495, 2463, 2431, 2399, 2367, 2335, 2303, 2271, 2239, 2207, 2175, 2143, 2111, 2079, 2047, 2015, 1983, 1951, 1919, 1887, 1855, 1823, 1791, 1759, 1727, 1695, 1663, 1631, 1599, 1567, 1535, 1503, 1471, 1439, 1407, 1375, 1343, 1311, 1279, 1247, 1215, 1183, 1151, 1119, 1087, 1055, 1023, 991, 959, 927, 895, 863, 831, 799, 767, 735, 703, 671, 639, 607, 575, 543, 511, 479, 447, 415, 383, 351, 319, 287, 255, 223, 191, 159, 127, 95, 63, 31 };

void setupSampleRate();

#define MCP492x_CONFIG (uint8_t) 0x30

uint16_t noteA = 0;
uint16_t noteB = 0;
uint16_t noteC = 0;
uint16_t noteD = 0;

#define BUTTONSD_pm 0b00111111
#define BUTTON0_pm 0x01
#define BUTTON1_pm 0x02
#define BUTTON2_pm 0x04
#define BUTTON3_pm 0x08
#define BUTTON4_pm 0x10
#define BUTTON5_pm 0x20
#define BUTTONSB_pm 0b00000011
#define BUTTON6_pm 0x01
#define BUTTON7_pm 0x02
#define BUTTONSC_pm 0b00110000
#define BUTTON8_pm 0x10
#define BUTTON9_pm 0x20

void setupButtons() 
{
  DDRD &= ~BUTTONSD_pm;
  PORTD |= BUTTONSD_pm;
  DDRB &= ~BUTTONSB_pm;
  PORTB |= BUTTONSB_pm;
  DDRC &= ~BUTTONSC_pm;
  PORTC |= BUTTONSC_pm;
}

uint8_t readButton(uint8_t button_no)
{
  switch( button_no )
    {
    case 0: return (PIND & BUTTON0_pm) == 0;
    case 1: return (PIND & BUTTON1_pm) == 0;
    case 2: return (PIND & BUTTON2_pm) == 0;
    case 3: return (PIND & BUTTON3_pm) == 0;
    case 4: return (PIND & BUTTON4_pm) == 0;
    case 5: return (PIND & BUTTON5_pm) == 0;
    case 6: return (PINB & BUTTON6_pm) == 0;
    case 7: return (PINB & BUTTON7_pm) == 0;
    case 8: return (PINC & BUTTON8_pm) == 0;
    case 9: return (PINC & BUTTON9_pm) == 0;
    }
}

uint8_t readKeyboard()
{
  uint8_t res = ((PINB & BUTTONSB_pm)<<6)|(PIND & BUTTONSD_pm);
  return res;
}

uint16_t poly_buffer[5] = {0,0,0,0};
uint8_t poly_i = 0;
uint8_t key_pressed;
  #define SCALE_LENGTH 8
  uint16_t scales[4][SCALE_LENGTH] = {
    {C5, D5, E5, F5, G5, A5, B5, C6},
    {C5, D5, E5, G5, A5, C6, D6, E6},
    {C5, Ds5, F5, Fs5, G5, As5, C6, Ds6},
    {C5, D5, Ds5, F5, G5, A5, As5, C6}
  };


  uint8_t scale_i = 0;

int main(void) 
{
  // Disable Interrupts for setup
  cli();
  
  // SETUP BUTTON INPUTS
  setupButtons();

  // Setup the SPI Bus
  setupSPI(SPI_MODE_0, SPI_MSB, SPI_NO_INTERRUPT, SPI_MASTER_CLK2);
  SETUP_DAC;
  setupSampleRate(22000); // Setup Timer1 to work at 22KHz sample rate

  DDRC |= 0x0F;  
  PORTC &= 0xF0;
  
  int i;
  int j;
  uint8_t keyboard;

  uint8_t settings_pressed;
  sei(); // Enable Interrupts now that we are all ready to go
  for(;;) 
    {

      if (readButton(9) && ((settings_pressed & (0x01 << 0)) == 0)) 
	{
	  PORTC ^= 0x0F;
	  if (scale_i < 3) 
	    scale_i += 1;
	  else
	    scale_i = 0;
	  
	  settings_pressed = 0x01;
	}
      else if (!readButton(9) && ((settings_pressed & (0x01 << 0)) != 0))
	{
	  settings_pressed = 0;
	}
      
      if (readButton(8) && ((settings_pressed & (0x01 << 1)) == 0)) 
	{
	  PORTC ^= 0x0F;
	  if (scale_i > 0) 
	    scale_i -= 1;
	  else
	    scale_i = 3;
	  settings_pressed = 0x02;
	}
      else if (!readButton(8) && ((settings_pressed & (0x01 << 1)) != 0))
	{
	  settings_pressed &= ~(0x01<<1);
    }
      _delay_ms(2);
    }

  return(0);
}

#define MAX_LOOKUP  0x7FFF
#define INCREMENT_NOTE(note, i) 			\
  if (note)						\
    {							\
      if (i + note < MAX_LOOKUP)			\
      {							\
	i += note;					\
      }							\
    else if (i + note > MAX_LOOKUP)			\
      {							\
	i = note - (MAX_LOOKUP - i);			\
      }							\
    else						\
      {							\
	i = 0;						\
      }							\
    }							\
  else							\
    {							\
      i = 0;						\
    }							

#define FREQUENCY_TO_COUNT(x) (F_CPU/1000)/(x/1000)
// Setup Sample rate at 22kHz, externally we filter at 11kHz
// this satisfies Nyquist and technically we should be able
// to accurately reconstruct waves up to 11kHz

void setupSampleRate(uint16_t frequency)
{    
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  
  // set compare match register to desired timer count:
  // We want to compare at 22kHz
  // target time/timer resolution
  // (1/22kHz)/(1/16MHz) = 727
  OCR1A = 727; 
  //OCR1A = (uint16_t) FREQUENCY_TO_COUNT(frequency);

  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS10); // No prescaler
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts:
}

uint8_t keyboard = 0;
ISR(TIMER1_COMPA_vect) 
{
  static uint16_t iA = 0;
  static uint16_t iB = 0;
  static uint16_t iC = 0;
  static uint16_t iD = 0;
  static uint16_t final = 0;
  static uint8_t j = 0;
  static uint8_t keyboard;
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

  INCREMENT_NOTE(poly_buffer[0], iA);
  INCREMENT_NOTE(poly_buffer[1], iB);
  INCREMENT_NOTE(poly_buffer[2], iC);
  INCREMENT_NOTE(poly_buffer[3], iD);
  
  final = (channelA[iA>>NOTES_BASE]) ;

  writeMCP492x(final, MCP492x_CONFIG);
}
