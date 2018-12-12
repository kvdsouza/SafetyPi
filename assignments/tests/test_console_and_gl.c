#include "assert.h"
#include "timer.h"
#include "fb.h"
#include "gl.h"
#include "console.h"
#include "printf.h"

/* Note that to use the console, one should only have to
 * call console_init. To use the graphics library, one
 * should only have to call gl_init. If your main() requires
 * more than this, then your code will not pass tests and
 * will likely have many points deducted. Our GL tests
 * will call gl_init then invoke operations; our console
 * tests will call console_init then invoke operations.
 * To guarantee that tests will pass, make sure to also
 * run tests for each component separately.
 */

#define _WIDTH 640
#define _HEIGHT 512

#define _NROWS 10
#define _NCOLS 20

static void test_console(void)
{
    console_init(_NROWS, _NCOLS);
    console_clear();
    timer_delay(2);
    console_printf("Hippopotamus\n");
    timer_delay(5);
    console_printf("aloha");
    timer_delay(5);
    console_printf("coronado lane");
    timer_delay(5);
    console_clear();
    timer_delay(3);
    console_printf("Hello, Joshua. Strange game. The only winning move is not to play. How about a nice game of chess?\n");
    timer_delay(5);
    console_printf("HELLO\r\n");
    timer_delay(5);
    console_printf("watching Gandhi 'til I'm chargedThen writing in my book of rhymes, all the words past the margin. To hold the mic I'm throbbin', mechanical movement\n");
    timer_delay(5);
    console_printf("Instead of simply incrementing a counter in the interrupt handler, call rb_enqueue with the counter’s value on each update. Whereas main used to directly read the counter and compare to previous value, change the code to instead call rb_dequeue to get each update.");
    timer_delay(5);
    console_printf("hi again");
    timer_delay(5);
    console_printf("to be or not to be, that is the question");

    timer_delay(3);
    console_clear();
    timer_delay(3);
    console_printf("amazing gra\fce");
    timer_delay(3);
    console_printf("My name is Stev\be.");
    timer_delay(2);

    for (int i = 0; i < 26; i++) {
        console_printf("%c\n", 'a' + i);
        timer_delay(1);
    } // Elisa provided design for this.

}


static void test_gl(void)
{
    gl_init(_WIDTH, _HEIGHT, GL_DOUBLEBUFFER);

    gl_clear(gl_color(0xFF, 0, 0xFF)); // Background should be purple.

    // Draw an amber pixel at an arbitrary spot.
    gl_draw_pixel(_WIDTH/3, _HEIGHT/3, GL_AMBER);
    //printf()
    assert(gl_read_pixel(_WIDTH/3, _HEIGHT/3) == GL_AMBER);
    //
    // // Basic rectangle should be blue in center of screen
    //gl_draw_rect(_WIDTH/2 - 20, _HEIGHT/2 - 20, 40, 40, GL_BLUE);
    //
    // // Should write a single character
    gl_draw_char(60, 10, 'A', GL_BLUE);

    gl_draw_string(120, 40, "abacus", GL_BLUE);
    //
    gl_swap_buffer();

    timer_delay(5);

    gl_clear(gl_color(0, 0, 0xFF));
    gl_draw_string(120, 40, "another buffer", GL_AMBER);
    gl_swap_buffer();


}

void main(void)
{
    test_console();
    test_gl();

    /* TODO: Add tests here to test your graphics library and console.
       For the framebuffer and graphics libraries, make sure to test
       single & double buffering and drawing/writing off the right or
       bottom edge of the frame buffer.
       For the console, make sure to test wrap-around and scrolling.
    */
}
