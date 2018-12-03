/**
 * LCD code
 */
#include "main.h"
char str[SCREEN_LENGTH + 1];
void lcd_update(int song_position) {
 	int num_notes_remaining = SONG_LENGTH - song_position;
 	if (num_notes_remaining > SCREEN_LENGTH)
 		num_notes_remaining = SCREEN_LENGTH;
	for (int i = 0; i < NUM_BUTTONS; i++) {
		for (int j = 0; j < num_notes_remaining; j++) {
			unsigned char notes = song[song_position + num_notes_remaining - j - 1];
			unsigned char note = notes & (1 << i);
			if (note) {
				str[j + SCREEN_LENGTH - num_notes_remaining] = '-';
			} else {
				str[j + SCREEN_LENGTH - num_notes_remaining] = ' ';
			}
		}
		str[SCREEN_LENGTH] = '\0';
		GLCD_GoTo(0, 7 - i);
		GLCD_WriteString(str);
	}
}

void lcd_print_beginning() {
	char* update = "Hit: 0, Missed: 0";
	GLCD_GoTo(0,0);
	GLCD_WriteString(update);
	for (int pos = 0; pos < SCREEN_LENGTH; pos++) {
		for (int i = 0; i < NUM_BUTTONS; i++) {
			for (int j = 0; j < SCREEN_LENGTH && j < pos; j++) {
				unsigned char notes = song[pos - j];
				unsigned char note = notes & (1 << i);
				if (note) {
					str[j] = '-';
				} else {
					str[j] = ' ';
				}
			}
			str[SCREEN_LENGTH - 1] = '\0';
			GLCD_GoTo(0, 7 - i);
			GLCD_WriteString(str);
		}
		nano_wait(LOOP_WAIT);
	}
}
