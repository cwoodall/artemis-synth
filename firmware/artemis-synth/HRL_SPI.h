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
#ifndef _HRL_SPI_h_
#define _HRL_SPI_h_

#include <avr/io.h>

#ifdef __cplusplus
extern "C" {
#endif

// create alias for the different SPI chip pins.
// In general Port B contains all of the SPI pins, but for
// flexibility you can define what port contains the SPI pins.
#if (defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__))
 #define SPI_SS_PIN PB0
 #define SPI_SCK_PIN PB1
 #define SPI_MOSI_PIN PB2
 #define SPI_MISO_PIN PB3
 #define SPI_PORT PORTB
 #define SPI_DDR  DDRB
 #define SPI_PIN  PINB
#elif (defined(__AVR_ATmega48__) || defined(_AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))
 #define SPI_SS_PIN PB2
 #define SPI_SCK_PIN PB5
 #define SPI_MOSI_PIN PB3
 #define SPI_MISO_PIN PB4
 #define SPI_PORT PORTB
 #define SPI_DDR  DDRB
 #define SPI_PIN  PINB
#else
 #error unknown processor - add to spi.h
#endif

// SPI clock modes
#define SPI_MODE_0 0x00 /* Sample (Rising) Setup (Falling) CPOL=0, CPHA=0 */
#define SPI_MODE_1 0x01 /* Setup (Rising) Sample (Falling) CPOL=0, CPHA=1 */
#define SPI_MODE_2 0x02 /* Sample (Falling) Setup (Rising) CPOL=1, CPHA=0 */
#define SPI_MODE_3 0x03 /* Setup (Falling) Sample (Rising) CPOL=1, CPHA=1 */

// data direction
#define SPI_LSB 1 /* send least significant bit (bit 0) first */
#define SPI_MSB 0 /* send most significant bit (bit 7) first */

// whether to raise interrupt when data received (SPIF bit received)
#define SPI_NO_INTERRUPT 0
#define SPI_INTERRUPT 1

// Slave or master with clock diviser
#define SPI_SLAVE 0xF0
#define SPI_MASTER_CLK4 0x00 /* F_CPU/4 */
#define SPI_MASTER_CLK16 0x01 /* F_CPU/16 */
#define SPI_MASTER_CLK64 0x02 /* F_CPU/64 */
#define SPI_MASTER_CLK128 0x03 /* F_CPU/128 */
#define SPI_MASTER_CLK2 0x04 /* F_CPU/2 */
#define SPI_MASTER_CLK8 0x05 /* F_CPU/8 */
#define SPI_MASTER_CLK32 0x06 /* F_CPU/32 */


void setupSPI(uint8_t mode, // timing mode SPI_MODES_[0-4] 
	      uint8_t dord, // data direction SPI_LSB|SPI_MSB
	      uint8_t interrupt,  // SPI_NO_INTERRUPT|SPI_INTERRUPT
	      uint8_t clk); // Clock Divisor, defines slave as well
                            // SPI_SLAVE
                            // SPI_MASTER_CLK[4|16|64|128|2|8|32]

void disableSPI();

uint8_t sendSPI(uint8_t data);
uint8_t received_from_spi(uint8_t data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
