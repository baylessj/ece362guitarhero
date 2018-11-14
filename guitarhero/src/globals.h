/**
 * Global variables for game/lcd usage
 */

// In an ideal world, this should be able to be behind the header guards, but no... thanks linker
#define NUM_BUTTONS 5
#define SONG_LENGTH 100

#define SONG_NUMBER 1

extern int hit_notes, missed_notes;

extern unsigned char song1[SONG_LENGTH];
extern unsigned char song2[SONG_LENGTH];
