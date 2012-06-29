#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "HRL_SPI.h"
#include "HRL_MCP492x.h"
#include "notes.h" // Note increments (format Q9.7)

// 256 samples = 1 wave
// 22kHz Sample frequency
// (22kHz/256) = 85.9375Hz <- with increment size of 1
// Tone = sample rate / (samples * increments)
// increments_for_tone = [Tone Frequency]*[SAMPLES]/[SAMPLE RATE]

void setupTimer() {
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
 
    // set compare match register to desired timer count:
    // We want to compare at 22kHz
    // target time/timer resolution
    // (1/22kHz)/(1/16MHz) = 727
    OCR1A = 727; 
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS10); // No prescaler
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    // enable global interrupts:
}



void setup() {
  // Initiate SPI in Mode 0 with MSB first, NO interrupts and a clock of F_CPU/2 
  cli();
  
  setupSPI(SPI_MODE_0, SPI_MSB, SPI_NO_INTERRUPT, SPI_MASTER_CLK2);
  SETUP_DAC;
  
  setupTimer();
 
  sei();    
}
/*
// ChannelA is a sine wave
uint16_t channelA[] = { 2048, 2098, 2148, 2198, 2248,
2298, 2348, 2398, 2447, 2496, 2545, 2594, 2642, 2690, 2737, 2784, 2831, 2877, 2923, 2968, 
3013, 3057, 3100, 3143, 3185, 3226, 3267, 3307, 3346, 3385, 3423, 3459, 3495, 3530, 3565,
3598, 3630, 3662, 3692, 3722, 3750, 3777, 3804, 3829, 3853, 3876, 3898, 3919, 3939, 3958,
3975, 3992, 4007, 4021, 4034, 4045, 4056, 4065, 4073, 4080, 4085, 4089, 4093, 4094, 4095,
4094, 4093, 4089, 4085, 4080, 4073, 4065, 4056, 4045, 4034, 4021, 4007, 3992, 3975, 3958, 
3939, 3919, 3898, 3876, 3853, 3829, 3804, 3777, 3750, 3722, 3692, 3662, 3630, 3598, 3565,
3530, 3495, 3459, 3423, 3385, 3346, 3307, 3267, 3226, 3185, 3143, 3100, 3057, 3013, 2968, 
2923, 2877, 2831, 2784, 2737, 2690, 2642, 2594, 2545, 2496, 2447, 2398, 2348,2298,
2248, 2198, 2148, 2098, 2048, 1997, 1947, 1897, 1847, 1797, 1747, 1697, 1648, 1599, 1550,
1501, 1453, 1405, 1358, 1311, 1264, 1218, 1172, 1127, 1082, 1038, 995, 952, 910, 869,
828, 788, 749, 710, 672, 636, 600, 565, 530, 497, 465, 433, 403, 373, 345, 318, 291, 
266, 242, 219, 197, 176, 156, 137, 120, 103, 88, 74, 61, 50, 39, 30, 22, 15, 10, 6,
2, 1, 0, 1, 2, 6, 10, 15, 22, 30, 39, 50, 61, 74, 88, 103, 120, 137, 156, 176, 197,
219, 242, 266, 291, 318, 345, 373, 403, 433, 465, 497, 530, 565, 600, 636, 672,
710, 749, 788, 828, 869, 910, 952, 995, 1038, 1082, 1127, 1172, 1218, 1264, 1311,
1358, 1405, 1453, 1501, 1550, 1599, 1648, 1697, 1747, 1797, 1847, 1897, 1947, 1997};
*/
// Sine Wave
uint16_t channelA[] = { 2047, 2097, 2147, 2198, 2248, 2298, 2347, 2397, 2446, 2496, 2545, 2593, 2641, 2689, 2737, 2784, 2831, 2877, 2922, 2968, 3012, 3056, 3100, 3142, 3185, 3226, 3267, 3307, 3346, 3384, 3422, 3459, 3495, 3530, 3564, 3597, 3630, 3661, 3692, 3721, 3749, 3777, 3803, 3829, 3853, 3876, 3898, 3919, 3939, 3957, 3975, 3991, 4006, 4020, 4033, 4045, 4055, 4064, 4072, 4079, 4085, 4089, 4092, 4094, 4095, 4094, 4092, 4089, 4085, 4079, 4072, 4064, 4055, 4045, 4033, 4020, 4006, 3991, 3975, 3957, 3939, 3919, 3898, 3876, 3853, 3829, 3803, 3777, 3749, 3721, 3692, 3661, 3630, 3597, 3564, 3530, 3495, 3459, 3422, 3384, 3346, 3307, 3267, 3226, 3185, 3142, 3100, 3056, 3012, 2968, 2922, 2877, 2831, 2784, 2737, 2689, 2641, 2593, 2545, 2496, 2446, 2397, 2347, 2298, 2248, 2198, 2147, 2097, 2047, 1997, 1947, 1896, 1846, 1796, 1747, 1697, 1648, 1598, 1549, 1501, 1453, 1405, 1357, 1310, 1263, 1217, 1172, 1126, 1082, 1038, 994, 952, 909, 868, 827, 787, 748, 710, 672, 635, 599, 564, 530, 497, 464, 433, 402, 373, 345, 317, 291, 265, 241, 218, 196, 175, 155, 137, 119, 103, 88, 74, 61, 49, 39, 30, 22, 15, 9, 5, 2, 0, 0, 0, 2, 5, 9, 15, 22, 30, 39, 49, 61, 74, 88, 103, 119, 137, 155, 175, 196, 218, 241, 265, 291, 317, 345, 373, 402, 433, 464, 497, 530, 564, 599, 635, 672, 710, 748, 787, 827, 868, 909, 952, 994, 1038, 1082, 1126, 1172, 1217, 1263, 1310, 1357, 1405, 1453, 1501, 1549, 1598, 1648, 1697, 1747, 1796, 1846, 1896, 1947, 1997, 1997 };
// Channel B is a square wave
uint16_t channelB[] = {4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Channel C is a sawtooth (ramp)
uint16_t channelC[] = {0, 15, 31, 47, 63, 79, 95, 111, 127, 143, 159, 175, 191, 207, 223, 239, 255, 271, 287, 303, 319, 335, 351, 367, 383, 399, 415, 431, 447, 463, 479, 495, 511, 527, 543, 559, 575, 591, 607, 623, 639, 655, 671, 687, 703, 719, 735, 751, 767, 783, 799, 815, 831, 847, 863, 879, 895, 911, 927, 943, 959, 975, 991, 1007, 1023, 1039, 1055, 1071, 1087, 1103, 1119, 1135, 1151, 1167, 1183, 1199, 1215, 1231, 1247, 1263, 1279, 1295, 1311, 1327, 1343, 1359, 1375, 1391, 1407, 1423, 1439, 1455, 1471, 1487, 1503, 1519, 1535, 1551, 1567, 1583, 1599, 1615, 1631, 1647, 1663, 1679, 1695, 1711, 1727, 1743, 1759, 1775, 1791, 1807, 1823, 1839, 1855, 1871, 1887, 1903, 1919, 1935, 1951, 1967, 1983, 1999, 2015, 2031, 2047, 2063, 2079, 2095, 2111, 2127, 2143, 2159, 2175, 2191, 2207, 2223, 2239, 2255, 2271, 2287, 2303, 2319, 2335, 2351, 2367, 2383, 2399, 2415, 2431, 2447, 2463, 2479, 2495, 2511, 2527, 2543, 2559, 2575, 2591, 2607, 2623, 2639, 2655, 2671, 2687, 2703, 2719, 2735, 2751, 2767, 2783, 2799, 2815, 2831, 2847, 2863, 2879, 2895, 2911, 2927, 2943, 2959, 2975, 2991, 3007, 3023, 3039, 3055, 3071, 3087, 3103, 3119, 3135, 3151, 3167, 3183, 3199, 3215, 3231, 3247, 3263, 3279, 3295, 3311, 3327, 3343, 3359, 3375, 3391, 3407, 3423, 3439, 3455, 3471, 3487, 3503, 3519, 3535, 3551, 3567, 3583, 3599, 3615, 3631, 3647, 3663, 3679, 3695, 3711, 3727, 3743, 3759, 3775, 3791, 3807, 3823, 3839, 3855, 3871, 3887, 3903, 3919, 3935, 3951, 3967, 3983, 3999, 4015, 4031, 4047, 4063, 4079};

// Channel D is a triangle wave
uint16_t channelD[] = { 0, 31, 63, 95, 127, 159, 191, 223, 255, 287, 319, 351, 383, 415, 447, 479, 511, 543, 575, 607, 639, 671, 703, 735, 767, 799, 831, 863, 895, 927, 959, 991, 1023, 1055, 1087, 1119, 1151, 1183, 1215, 1247, 1279, 1311, 1343, 1375, 1407, 1439, 1471, 1503, 1535, 1567, 1599, 1631, 1663, 1695, 1727, 1759, 1791, 1823, 1855, 1887, 1919, 1951, 1983, 2015, 2047, 2079, 2111, 2143, 2175, 2207, 2239, 2271, 2303, 2335, 2367, 2399, 2431, 2463, 2495, 2527, 2559, 2591, 2623, 2655, 2687, 2719, 2751, 2783, 2815, 2847, 2879, 2911, 2943, 2975, 3007, 3039, 3071, 3103, 3135, 3167, 3199, 3231, 3263, 3295, 3327, 3359, 3391, 3423, 3455, 3487, 3519, 3551, 3583, 3615, 3647, 3679, 3711, 3743, 3775, 3807, 3839, 3871, 3903, 3935, 3967, 3999, 4031, 4063, 4095, 4063, 4031, 3999, 3967, 3935, 3903, 3871, 3839, 3807, 3775, 3743, 3711, 3679, 3647, 3615, 3583, 3551, 3519, 3487, 3455, 3423, 3391, 3359, 3327, 3295, 3263, 3231, 3199, 3167, 3135, 3103, 3071, 3039, 3007, 2975, 2943, 2911, 2879, 2847, 2815, 2783, 2751, 2719, 2687, 2655, 2623, 2591, 2559, 2527, 2495, 2463, 2431, 2399, 2367, 2335, 2303, 2271, 2239, 2207, 2175, 2143, 2111, 2079, 2047, 2015, 1983, 1951, 1919, 1887, 1855, 1823, 1791, 1759, 1727, 1695, 1663, 1631, 1599, 1567, 1535, 1503, 1471, 1439, 1407, 1375, 1343, 1311, 1279, 1247, 1215, 1183, 1151, 1119, 1087, 1055, 1023, 991, 959, 927, 895, 863, 831, 799, 767, 735, 703, 671, 639, 607, 575, 543, 511, 479, 447, 415, 383, 351, 319, 287, 255, 223, 191, 159, 127, 95, 63, 31, 31 };

int config = 0x30;
uint16_t noteA = 0;
uint16_t noteB = 0;

void loop() {
  noteB = C4;
  noteA = C5;
  delay(500);
  noteB = G4;
  noteA = D5;
  delay(500);
  noteB = G4;
  noteA = E5;
  delay(500);
  noteB = A4;
  noteA = F5;
  delay(500);
  noteB = B4;
  noteA = G5;
  delay(250);
  noteB = As4;
  delay(250);
  noteB = A4;
  noteA = A5;
  delay(500);
  noteB = G4;
  noteA = B5;
  delay(500);
  noteB = G4;
  noteA = C6;
  delay(500);
  noteA = B5;
  delay(500);
  noteB = Fs4;
  noteA = A5;
  delay(500);
  noteB = G4;
  noteA = G5;
  delay(500);
  noteB = A4;
  noteA = F5;
  delay(500);
  noteB = B4;
  noteA = E5;  
  delay(500);
  noteA = D5;  
  delay(500);
  noteB = C5;
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
 noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
   noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
   noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
   noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
   noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
   noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = C5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = A5;
  delay(50);
 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50);
   
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50); 
  noteA = D5;
  delay(50);
  noteA = F5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
    delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C6;
    delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C6;
    delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C6;
    delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C6;
    delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C6;
    delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C6;
  delay(50)  ;
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = C6;
  delay(50);
  noteA = C5;
  delay(50);
  noteA = E5;
  delay(50);
  noteA = G5;
  delay(50);
  noteA = B5;
  delay(500);
  noteA = C6;
  delay(500);
  noteA = C5;
  delay(2000);
}

uint16_t iA = 0;
uint16_t iB = 0x000;
uint16_t final = 0;
uint8_t divis = 0;

ISR(TIMER1_COMPA_vect) {
  final = 0;
  divis = 1;
  if (noteA) {
    if (iA < 0x7FFF-noteA) 
      iA += noteA;
    else
      iA = 0;      

    divis += 1;
    final = (final + channelA[iA>>7])/divis;
    if (divis == 2) {
      divis = 1;
    } 
  }

/*
  if (noteB) {
    if (iB < 0x7FFF-noteB) 
      iB += noteB;
    else
      iB = 0;      
    
    divis += 1;
    final = (final + no[iB>>7])/divis;
    if (divis == 2) {
      divis = 1;
    } 
  }  
  */
  writeMCP492x(final, config);
}
