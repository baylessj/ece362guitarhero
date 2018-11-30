/**
 * LCD code
 */
#include "main.h"
char str[24];
void lcd_update(int song_position) {
	// characters are 5 pixels wide, display is 128 wide, so we can fit 25 chars

	GLCD_ClearScreen();
 	int pad = song_position + SCREEN_LENGTH - 1 - SONG_LENGTH;
	if (pad < 0) pad = 0;
	for (int i = 0; i < NUM_BUTTONS; i++) {
		for (int j = 0; j <= (SCREEN_LENGTH - 1 - pad); j++) {
			unsigned char notes = song[song_position - j + SCREEN_LENGTH - pad];
			unsigned char note = notes & (1 << i);
			if (note) {
				str[j + pad] = '-';
			} else {
				str[j + pad] = ' ';
			}
		}
		str[SCREEN_LENGTH - 1] = '\0';
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
