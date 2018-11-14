/**
 * LCD code
 */
#include "globals.h"
#include "lcd.h"
//#include "songs.c"

void lcd_update(int song_position) {
	unsigned char notes;
	switch (SONG_NUMBER) {
	case 1:
		notes = song1[song_position];
		break;
	case 2:
		notes = song2[song_position];
		break;
	default:
		break;
	}
	// display the notes
	// this will become a for loop likely that will grab the note arrays for a set range of displayable notes
}
