#include "HRL_MCP492x.h"

#ifdef __cplusplus
extern "C" {
#endif

void writeMCP492x(uint16_t data, uint8_t config) {
  // Take the top 4 bits of config and the top 4 valid bits (data is actually a 12 bit number) and or them together
  uint8_t top_msg = (config & 0xF0) | (0x0F & (data >> 8));
  
  // Take the bottom octet of data
  uint8_t lower_msg = (data & 0x00FF);
   
  // Select our DAC
  SELECT_DAC;
  // Send first 8 bits
  sendSPI(top_msg);
  // Send second 8 bits
  sendSPI(lower_msg);
  DESELECT_DAC;
}

#ifdef __cplusplus
}
#endif
