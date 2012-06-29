/** 
 * Happy Robot Labs AVR SPI Library
 * 
 * An simple SPI Libary for AVR Microcontrollers.
 * This current library is not interrupt driven; however,
 * moving towards an interrupt driven system might be the
 *
 * Supported microcontrollers are:
 *   - AT90USB82 and AT90USB162
 *   - ATMega48/88/168/328/328P 
 *
 * Copyright (c) Christopher Woodall 2012
 *
 * MIT License
 */ 

#include "HRL_SPI.h"

#ifdef __cplusplus
extern "C" {
#endif

void setupSPI(uint8_t mode, uint8_t dord, uint8_t interrupt, uint8_t clk) {
  if (clk == SPI_SLAVE) {
    SPI_DDR &= ~(1<<SPI_MOSI_PIN); //INPUT
    SPI_DDR |= (1<<SPI_MISO_PIN);  // OUTPUT
    SPI_DDR &= (1<<SPI_SS_PIN);    // INPUT
    SPI_DDR &= ~(1<<SPI_SCK_PIN);  // INPUT
  } else {
    SPI_DDR |= (1<<SPI_MOSI_PIN);  // OUTPUT
    SPI_DDR &= ~(1<<SPI_MISO_PIN); // INPUT
    SPI_DDR |= (1<<SPI_SCK_PIN);   // OUTPUT
    SPI_DDR |= (1<<SPI_SS_PIN);    // OUTPUT
  }
  
  // Configure SPCR (SPI Control Register)
  SPCR = 0; // Clear SPCR
  SPCR = ((interrupt ? 1 : 0) << SPIE) // Interrupt Enable
       | (1<<SPE) // Enable SPI bus
       | (dord<<DORD) // LSB or MSB
       | (((clk != SPI_SLAVE) ? 1 : 0) << MSTR)  // Slave or Master
       | (((mode & 0x02) == 0x02) << CPOL) // Clock timing mode CPHA.
       | (((mode & 0x01) == 0x01) << CPHA) // Clock timing mode CPHA.
       | (((clk & 0x02) == 2) << SPR1) // CPU clock divisor SPR1
       | (((clk & 0x01) << SPR0)); // cpu clock divisor SPRO
  SPSR = (((clk & 0x04) == 4) << SPI2X);
}

void disableSPI() {
  SPCR &= ~(1<<SPE);
}

uint8_t sendSPI(uint8_t data) {
  SPDR = data;
  
  while (!(SPSR & (1<<SPIF)));
  
  return SPDR;
}

uint8_t received_from_spi(uint8_t data)
{
  SPDR = data;
  return SPDR;
}
