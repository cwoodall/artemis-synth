#ifndef _optoloader_h_
#define _optoloader_h_

#include <avr/io.h>

#define OPTO_SYNC_bm 0x01
#define OPTO_STRT_bm 0x02
#define OPTO_ONEF_bm 0x04
#define OPTO_ACTI_bm 0x08

typedef struct {
  uint8_t zero_count;
  uint8_t prev_count;
  uint8_t counter;

  uint8_t sync_count;

  uint8_t message;
  uint8_t message_count;

  /* uint8_t flags stores a bunch of single bit flags.
   *
   * bit | name | function
   * ------------------------------------------------------------------
   *  0  | SYNC | set to '1' when the clock has been sent & synced.
   * ------------------------------------------------------------------
   *  1  | STRT | set to '1' when the start bit has been set.
   * ------------------------------------------------------------------
   *  2  | ONEF | toggles from 0 to 1 to keep track of ones and phase. 
   * ------------------------------------------------------------------
   *  3  | ACTI | '1' when active
   * ------------------------------------------------------------------
   *  4  | NONE | does nothing
   * ------------------------------------------------------------------
   *  5  | NONE | does nothing
   * ------------------------------------------------------------------
   *  6  | NONE | does nothing
   * ------------------------------------------------------------------
   *  7  | NONE | does nothing
   * ------------------------------------------------------------------   
   *
   * Macros are defined for the bitmasks:
   *   convention: OPTO_name_bm
   *   the _bm indicates bit mask
   */
  uint8_t flags; 
} optoloader_t;

void disableOptoloader(optoloader_t *opto);
optoloader_t setupOptoloader();

void enableOptoloader(optoloader_t *opto);
void runOptoloader(optoloader_t *optoloader, uint8_t *led_display);

#endif
