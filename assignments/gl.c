#include "gl.h"
#include "printf.h"
#include "fb.h"
#include "font.h"
#include "strings.h"


const unsigned WIDTH = 1024;
const unsigned HEIGHT = 768;
const unsigned DEPTH = 4;

void gl_init(unsigned int width, unsigned int height, unsigned int mode)
{
    fb_init(width, height, DEPTH, mode);
}

void gl_swap_buffer(void)
{
    fb_swap_buffer();
}

unsigned int gl_get_width(void)
{
    // TODO: implement this function
    return fb_get_width();
}

unsigned int gl_get_height(void)
{
    // TODO: implement this function
    return fb_get_height();
}


color_t gl_color(unsigned char r, unsigned char g, unsigned char b)
{
    color_t color;
    // bitshifting to get RGB values.
    color = (( (0xff << 24) |(unsigned int)r<< 16) | ((unsigned int)g << 8) | b);
    return color;
}

// borrowed from lecture code - Chris Gregg.
void gl_clear(color_t c)
{
    unsigned *im = (unsigned *)fb_get_draw_buffer();
    int n = (fb_get_pitch() / 4)* fb_get_height();
    for (int i = 0; i < n; i++) {
        *im++ = c;
    };
}

void gl_draw_pixel(int x, int y, color_t c)
{
    if (x > 0 && x < gl_get_width()) {
        if (y > 0 && y < gl_get_height()) {
            unsigned int* im = (unsigned int *)fb_get_draw_buffer();
            im[y* (fb_get_pitch()/4) + x] = c;
        }
    }
}

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
 */
void gl_draw_rect(int x, int y, int w, int h, color_t c)
{
    for (int i = x; i < (x + w); i++) {
        for (int j = y; j < (y + h); j++) {
            if (i > 0 && i < gl_get_width() && j > 0 && j < gl_get_height()) {
                gl_draw_pixel(i, j, c);
            }
        }
    }
}

/*
 * Draw a single character at location x,y in color c.
 */
void gl_draw_char(int x, int y, int ch, color_t c)
{
    int buflen = font_get_size();
    unsigned char buf[buflen];
    if (x >= 0 && x < gl_get_width() && y >= 0 && y < gl_get_height()) {
        font_get_char(ch, buf, buflen);
        for (int i = 0; i < buflen; i++) {
            if ((x+ (i % font_get_width()) >= gl_get_width()) || (y+ (i / font_get_height()) >= gl_get_height())) {
                break;
            }
            if (buf[i] == 255) {
                gl_draw_pixel(x+ (i % font_get_width()), y+ (i / font_get_height()), c);
            }
        }
    }
}

/*
 * Draws a string at location x,y in color c.
 */
void gl_draw_string(int x, int y, char* str, color_t c)
{
    int strLength = strlen(str);
    for (int i = 0; i < strLength; i++) {
        gl_draw_char(x + (font_get_width()*i), y, str[i], c);
    }
}

unsigned int gl_get_char_height(void)
{
    return font_get_height();
}

unsigned int gl_get_char_width(void)
{
    return font_get_width();
}
