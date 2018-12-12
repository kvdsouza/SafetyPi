#include "console.h"
#include "fb.h"
#include "gl.h"
#include "printf.h"
#include <stdarg.h>
#include "strings.h"
#include "malloc.h"

static volatile char* text;
static volatile int cursorx;
static volatile int cursory;

static unsigned int length;
static unsigned int columns;
static unsigned int rows;


#define _WIDTH 640
#define _HEIGHT 512
#define MAX_OUTPUT_LEN 1024


/*
 * Initializes the console.
 * @param nrows the requested number of rows in characters
 * @param ncols the requested number of columns in characters
 */
void console_init(unsigned int nrows, unsigned int ncols)
{
	columns = ncols;
	rows = nrows;
	length = nrows*ncols;
	text = malloc(length*2); 	// in case of scrolling.
	cursorx = 0;
	cursory = 0;
	gl_init(ncols * gl_get_char_width(), nrows * gl_get_char_height(), GL_DOUBLEBUFFER);
	//gl_init(_WIDTH, _HEIGHT, GL_DOUBLEBUFFER);	// Another potential use.
}

/*
 * Clear all console text and resets cursor.
 */
void console_clear(void)
{
	cursorx = 0;
	cursory = 0;
	color_t c = GL_BLACK;
	gl_clear(c);
}

/*
 * Print the formatted string to the console starting at current cursor
 * position.
 */
int console_printf(const char *format, ...)
{
	console_clear();
	int returnLength = 0;
    va_list args;
    va_start(args, format);
	char tempArray[MAX_OUTPUT_LEN];
	int tempLength = vsnprintf(tempArray, MAX_OUTPUT_LEN, format, args);
    va_end(args);
	// finished va_list work.
	int textLength = strlcat((char *)text, tempArray, MAX_OUTPUT_LEN); // check this later.
	cursorx = 0;
	cursory = 0;

	// CHECK FOR NUMBER OF ROWS IN NEW TEXT.
	int rowCounter = 0;	// number of rows.
	int rowChars = 0; 	// number of chars in a row.
	for (int i = 0; i < textLength; i++) {		// columns counter.
		rowChars++;		// CHECK ARITHMETIC.
		if (rowChars > columns || text[i] == '\n') { 		// DISCOUNT BACKSPACE ETC.
			rowCounter++;
			rowChars = 0;
		}
	}

	// DIFFERENCE IN ROWS AVAILABLE AND WHAT IS IN TEXT BUFFER.
	//int difference = rowCounter - rows;

	for (int i = 0; i < textLength; i++) {
		if (text[i] == '\f') {
			console_clear();	// test this.
		} else if (text[i] == '\b') {
			// in wrap case.
			if (cursorx == 0 && cursory != 0) {
				cursory = cursory - gl_get_char_height();
				cursorx = gl_get_char_width() * columns;
			}
			// in normal case.
			cursorx = cursorx - gl_get_char_width();
			gl_draw_rect((int)cursorx, (int)cursory, (int)gl_get_char_width(), (int)gl_get_char_height(), GL_BLACK);
		} else if (text[i] == '\n') {
			cursory = cursory + gl_get_char_height();
			cursorx = 0;
		} else if (text[i] == '\r') { 	// two cases.
			if (cursorx + gl_get_char_width() > columns * gl_get_char_width()) { // wrap case.
				cursory = cursory + gl_get_char_height();
				cursorx = 0;
			}
			gl_draw_char(cursorx, cursory, ' ', GL_GREEN);
			cursorx++;
		} else {
			// if (rowCounter > rows) { 		// SCROLL CASE PHILOSOPHY - plz show mercy.
			// 	if (cursorx + gl_get_char_width() > columns * gl_get_char_width()) { // wrap case.
			// 		cursory = cursory + gl_get_char_height();
			// 		cursorx = 0;
			// 	}
			// 	if (i > length - difference*(columns +1)) {		// one where I would
			// 		gl_draw_char(cursorx, cursory, '\n', GL_GREEN);
			// 		cursorx = cursorx + gl_get_char_width();
			// 		returnLength++;
			// 	} else {
			// 		gl_draw_char(cursorx, cursory, text[i + difference * (columns)], GL_GREEN);
			// 		cursorx = cursorx + gl_get_char_width();
			// 		returnLength++;
			// 	}
			/*} else */{	// WELL-FITTING CASE.
				if (cursorx + gl_get_char_width() > columns * gl_get_char_width()) { // wrap case.
					cursory = cursory + gl_get_char_height();
					cursorx = 0;
				}
				gl_draw_char(cursorx, cursory, text[i], GL_GREEN);
				cursorx = cursorx + gl_get_char_width();
				returnLength++;
			}
		}
	}
	gl_swap_buffer();
	return tempLength;
}


// Acknowledgements/Citations: Spoke with Abla, Elisa, Mary, TA's, Pierce about code. Helped Thakur.
// Explained some concepts to Elisa and Pierce.
// Scratch code on scroll below and restructure - will
// 	// if (textLength > length) { // scroll.
	// 	console_clear();
	// 	for (int i = 0; i < textLength; i++) { // still not there.
	// 		if (i > (textLength - (columns -1))) {
	// 			text[i] = ' ';
	// 			// pad spaces
	// 		} else {
	// 			text[i] = text[i + columns];
	// 		}
	// 	}
	// }
	// printf("%s\n", text);
	// int i = 0;
	// while (i < textLength) {
	// 	if (text[i] == '\f') {
	// 		console_clear();
	// 	} else if (text[i] == '\b') {
	// 		if (i > 0) {
	// 			text[i] = ' ';
	// 			text[i-1] = ' ';
	// 			i--;
	// 			i--;
	// 		} else {
	// 			shell_bell();
	// 		}
	// 	} else if (text[i] == 'r') {
	// 		text[i] = ' ';
	// 		text
	// 	}
	// }
