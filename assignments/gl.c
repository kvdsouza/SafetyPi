#include "gl.h"
#include "printf.h"
#include "fb.h"
#include "font.h"
#include "strings.h"


const unsigned WIDTH = 1024;
const unsigned HEIGHT = 768;
const unsigned DEPTH = 4;
/*
 * Initialize the graphic library. This function will call fb_init in turn
 * to initialize the framebuffer. The framebuffer will be initialzed to
 * 4-byte depth (32 bits per pixel).
 *
 * @param width  the requested width in pixels of the framebuffer
 * @param height the requested height in pixels of the framebuffer
 * @param mode   whether the framebuffer should be
 *                  single buffered (GL_SINGLEBUFFER)
 *                  or double buffered (GL_DOUBLEBUFFER)
 */
void gl_init(unsigned int width, unsigned int height, unsigned int mode)
{
    fb_init(width, height, DEPTH, mode);
    // buffer is single or double?
}


void gl_swap_buffer(void)
{
    fb_swap_buffer();
}

/*
 * Get the current width in pixels of the framebuffer.
 *
 * @return    the width in pixels
 */
unsigned int gl_get_width(void)
{
    // TODO: implement this function
    return fb_get_width();
}

/*
 * Get the current height in pixels of the framebuffer.
 *
 * @return    the height in pixels
 */
unsigned int gl_get_height(void)
{
    // TODO: implement this function
    return fb_get_height();
}

/*
 * Returns a color composed of the specified red, green, and
 * blue components. The alpha component of the color will be
 * set to 0xff (fully opaque).
 *
 * @param r  the red component of the color  0xFFFF0000
 * @param g  the green component of the color 0xFF00FF00
 * @param b  the blue component of the color  0xFF0000FF
 *
 * @return   the color as a single value of type color_t
 */
color_t gl_color(unsigned char r, unsigned char g, unsigned char b)
{
    color_t color;
    color = (( (0xff << 24) |(unsigned int)r<< 16) | ((unsigned int)g << 8) | b);
    // TODO: implement this function
    // probably looks something like making a bit and doing some bitwise operations
    // such that we run this stuff 0x00000000
    // bit shifting
    printf("%x\n", color);
    return color;
}


void gl_clear(color_t c)
{
    // check out clear.c - a lot of the code is given here.
    unsigned *im = (unsigned *)fb_get_draw_buffer(); // check this.
    //printf("fb.pitch: %d\n",fb.pitch);
    //printf("fb.height: %d\n",fb.height);

    int n = (fb_get_pitch() / 4)* fb_get_height(); // words not bytes
    for (int i = 0; i < n; i++) {
        *im++ = c; // blue!!
    };
}

// 2d and 1d versions
void gl_draw_pixel(int x, int y, color_t c)
{
    // want the pitch depth as well potentially?
    // check out fourcolor.c and grid.c
    // bound checking if x and y is between 0 and width or height respectively.
    if (x > 0 && x < gl_get_width()) {
        if (y > 0 && y < gl_get_height()) {
            unsigned int* im = (unsigned int *)fb_get_draw_buffer();
            im[y* (fb_get_pitch()/4) + x] = c;
        }
    }

    // unsigned (*im)[/*WIDTH */] = (unsigned (*)[/*WIDTH */])fb_get_draw_buffer();
    // im[y][x] = c;
}

/* 1D
unsigned *im = (unsigned *)fb_get_draw_buffer();
*(im + y*WIDTH + x) = c;
*/

/*
 * Return the color of the pixel at location x,y. Returns 0 if the
 * location is outside the bounds of the framebuffer.
 *
 * @param x  the x location of the pixel
 * @param y  the y location of the pixel
 *
 * @return   the color at that location
 */
color_t gl_read_pixel(int x, int y)
{
    if (x > 0 && x < gl_get_width()) {
        if (y > 0 && y < gl_get_height()) {
            unsigned int* im = (unsigned int *)fb_get_draw_buffer();
            return im[y* (fb_get_pitch()/4) + x];
        }
    }
    return 0;
}

/*
 * Draw a filled rectangle at location x,y with size w,h filled with color c.
 * All pixels in the rectangle that lie within the bounds of the
 * framebuffer are drawn. Any pixel that lies outside is clipped (i.e. not drawn).
 *
 * @param x  the x location of the upper left corner of the rectangle
 * @param y  the y location of the upper left corner of the rectangle
 * @param w  the width of the rectangle
 * @param h  the height of the rectangle
 * @param c  the color of the rectangle

 For a grid:
 for( int y = 0; y < HEIGHT; y += 16 )
      draw_hline(y, GL_WHITE);
  for( int x = 0; x < WIDTH; x += 16 )
      draw_vline(x, GL_WHITE);
 */
void gl_draw_rect(int x, int y, int w, int h, color_t c)
{
    // for int i = x.
    // for int y
    for (int i = x; i < (x + w); i++) {
        for (int j = y; j < (y + h); j++) {
            if (i > 0 && i < gl_get_width() && j > 0 && j < gl_get_height()) {
                gl_draw_pixel(i, j, c);
            }
        }// do some fun stuff.
    }
    // checkout fourcolor.c
    // TODO: implement this function
}

/*
 * Draw a single character at location x,y in color c.
 * Only those pixels of the character that lie within the bounds
 * of the framebuffer are drawn. Any pixel that lies outside is
 * clipped (i.e. not drawn).
 *
 * @param x   the x location of the upper left corner of the character glyph
 * @param y   the y location of the upper left corner of the character glyph
 * @param ch  the character to be drawn, e.g. 'a'. If this character has no glyph
 *            in the current font, nothing is drawn(refer to font_get_char())
 * @param c   the color of the character
 */
void gl_draw_char(int x, int y, int ch, color_t c)
{
    int buflen = font_get_size();   // do i need to pass in some stuff here?
    printf("%d\n", buflen);
    printf("%d\n", gl_get_width);
    printf("%d\n", gl_get_height);
    printf("%d\n", x);
    printf("%d\n", y);

    unsigned char buf[buflen];
    if (x >= 0 && x < gl_get_width() && y >= 0 && y < gl_get_height()) {
        int test = font_get_char(ch, buf, buflen);
        printf("%d\n", test);
        for (int i = 0; i < buflen; i++) {
            // insert error checks.
            if ((x+ (i % font_get_width()) >= gl_get_width()) || (y+ (i / font_get_height()) >= gl_get_height())) {
                break;
            }
            printf("%d\n", buf[i]);
            if (buf[i] == 255) {
                gl_draw_pixel(x+ (i % font_get_width()), y+ (i / font_get_height()), c);
                printf("%d\n", buf[i]);
            }
        }
    }
}

/*
 * Draw a string at location x,y in color c. The characters are drawn
 * left to right in a single line. Only the pixels of the characters
 * that lie within the bounds of the framebuffer are drawn. Any pixel
 * that lies outside is clipped (i.e. not drawn).
 *
 * @param x    the x location of the upper left corner of the first char of string
 * @param y    the y location of the upper left corner of the first char of string
 * @param str  the null-terminated string to be drawn
 * @param c    the color of the string
 */
void gl_draw_string(int x, int y, char* str, color_t c)
{
    int strLength = strlen(str);
    for (int i = 0; i < strLength; i++) {
        gl_draw_char(x + (font_get_width()*i), y /*+ (font_get_height()*i)*/, str[i], c);
    }
    // same concept but with a string (i.e. first character and last character?)
    // write drawchar with the right position.
    // ok - first get string length.

}

unsigned int gl_get_char_height(void)
{
    // check out font_getchar.
    return font_get_height();
}

unsigned int gl_get_char_width(void)
{
    // TODO: implement this function
    return font_get_width();
}
