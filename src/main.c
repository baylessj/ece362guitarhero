/**
 * ECE 362 Mini-Project: Guitar Hero
 */
#include "main.h"
#include <stdio.h>

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
	strum_init();

	// Prints the first 23 rows before the user will interact with the notes
	lcd_print_beginning();

	// Main Loop
	for (int i = 0; i < SONG_LENGTH; i++) {
		lcd_update(i);

		int hit = check_if_note_hit(i);
		if (hit == 1)
			hit_notes++;
		else if (hit == -1)
			missed_notes++;

		nano_wait(LOOP_WAIT - ADC_DELAY);
		adc_read_first();
		nano_wait(ADC_DELAY);
		char update[40];
		snprintf(update, sizeof(update) / sizeof(char), "Hit: %2d, Missed: %2d", hit_notes, missed_notes);
		GLCD_GoTo(0,0);
		GLCD_WriteString(update);
	}
}
