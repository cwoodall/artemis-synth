#ifndef _inst_h_
#define _inst_h_
#include <avr/io.h>
// Sine Wave
extern const uint16_t sine_table[256];

//Square Wave
extern const uint16_t square_table[256];

//Sawtooth Wave
extern const uint16_t saw_table[256];

// Triangle Wave
extern const uint16_t triangle_table[256];
#endif
