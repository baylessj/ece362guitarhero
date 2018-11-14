/**
 * ECE 362 Mini-Project: Guitar Hero
 */
#include "globals.h"
#include "lcd.h"

int hit_notes = 0, missed_notes = 0;

int main() {
	// Select which song?
	for (int i = 0; i < SONG_LENGTH; i++) {
		lcd_update(i);
	}
}
