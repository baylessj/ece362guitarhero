/**
 * Global variables for game/lcd usage
 */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define NUM_BUTTONS 5
#define SONG_LENGTH 100

#define SCREEN_LENGTH 21

#define SONG_NUMBER 1

extern int hit_notes, missed_notes;

extern unsigned char song[SONG_LENGTH];

#define LOOP_WAIT 500000000

void nano_wait(int t);

#endif
