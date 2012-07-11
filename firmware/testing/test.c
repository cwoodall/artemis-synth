#include <avr/io.h>

int main(void) {
  // Setup Pins
  DDRC = (0<<PC5)|(0<<PC4)|(1<<PC3)|(1<<PC2)|(1<<PC1)|(1<<PC0);
  PORTC = 0x30; // Turn on internal pullups and turn on bottom 4
  PORTC |= (1<<PC3)|(1<<PC2)|(0<<PC1)|(0<<PC0);
  for(;;) {
  }

  return(0);
}
