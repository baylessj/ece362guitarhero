/**
 * ECE 362 Mini-Project: Guitar Hero
 */
#include "main.h"

int hit_notes = 0, missed_notes = 0;

int main() {
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_GoTo(0, 0);
	GLCD_WriteString("+--Voltage Control--+");
	GLCD_GoTo(0, 7);
	GLCD_WriteString("+-------------------+");
	while (1);

	// Select which song?
	for (int i = 0; i < SONG_LENGTH; i++) {
		lcd_update(i);
	}
}
