/**
 * ECE 362 Mini-Project: Guitar Hero
 */
#include "main.h"

int hit_notes = 0, missed_notes = 0;

void nano_wait(int t) {
    asm("       mov r0,%0\n"
        "repeat:\n"
        "       sub r0,#83\n"
        "       bgt repeat\n"
        : : "r"(t) : "r0", "cc");
}

int main() {
	GLCD_Initialize();
	GLCD_ClearScreen();
//	GLCD_GoTo(0,7);
//	GLCD_WriteString("Hello    World");

	// Prints the first 23 rows before the user will interact with the notes
	lcd_print_beginning();

	// Main Loop
	for (int i = 0; i < SONG_LENGTH; i++) {
		lcd_update(i);
		char* update = sprintf("Hit: %d, Missed: %d", hit_notes, missed_notes);
		GLCD_GoTo(0,0);
		GLCD_WriteString(update);
		nano_wait(LOOP_WAIT);
	}
}
