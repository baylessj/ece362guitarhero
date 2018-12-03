/**
 * Strumming Functionality
 */
#ifndef _STRUM_H_
#define _STRUM_H_

#define ADC_DELAY 500000000

void strum_init();

int check_if_note_hit(int song_position);

void adc_read_first();

#endif
