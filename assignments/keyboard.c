#include "gpio.h"
#include "gpioextra.h"
#include "keyboard.h"
#include "ps2.h"

const unsigned int CLK  = GPIO_PIN23;
const unsigned int DATA = GPIO_PIN24;
unsigned int LEV0 = 0x20200034;


void wait_for_falling_clock_edge() {
    while (gpio_read(CLK) == 0) {}
    while (gpio_read(CLK) == 1) {}
}

void keyboard_init(void)
{
    gpio_set_input(CLK);
    gpio_set_pullup(CLK);

    gpio_set_input(DATA);
    gpio_set_pullup(DATA);
}

unsigned char keyboard_read_scancode(void)
{
    unsigned char contents;
    unsigned char counter;
    unsigned char result;
    // start loop
    while (1) {
        counter = 0;
        contents = 0;
        result = 0;
        //read first bit
        wait_for_falling_clock_edge();
        if (gpio_read(DATA) != 0) {    // if not set to low.
             continue;
        }
        wait_for_falling_clock_edge();
        // read the data bits.
        for (int i = 0 ; i < 8; i++) {
            contents = gpio_read(DATA);
            result = result + (contents << i); // try bitwise OR (|) for this guy.
            if (contents == 1) {
                counter++;
            }
            wait_for_falling_clock_edge();
        }
        // now check parity bit with what we have in our counter.
        if ((gpio_read(DATA) + counter) % 2 == 0) {
            continue;
        } else {
            wait_for_falling_clock_edge();
        }
        // last bit check.
        if (gpio_read(DATA) == 0) {
            continue;
        }
        break;
    }
    return result;
}

/*
 * `keyboard_read_sequence`: Low level keyboard interface.
 *
 * Reads the sequence of scan codes corresponding to next key action.
 * A key action is either a key down or key up. A sequence can
 * 1, 2, or 3 bytes, as follows:
 *    1 byte:  simple key down
 *    2 bytes: simple key up or extended key down
 *    3 bytes: extended key up
 * The function stores the scan code bytes into the `seq` array and
 * returns the number of bytes written to the array. This function
 * calls `keyboard_read_scancode` to read each scan code.
 */
int keyboard_read_sequence(unsigned char seq[])
{
    // try both
    int numBytes;
    seq[0] = keyboard_read_scancode();

    if (seq[0] == 0xe0) { // extended key (either up if 3 bytes) or down (2 bytes)
        seq[1] = keyboard_read_scancode();
        if (seq[1] == 0xf0) { // EXTENDED KEY UP
            seq[2] = keyboard_read_scancode();
            numBytes = 3;
            //keyboard_action_t = KEYBOARD_ACTION_UP;
        } else { // EXTENDED KEY DOWN.
            numBytes = 2;
            //keyboard_action_t = KEYBOARD_ACTION_DOWN;
        }
    } else if (seq[0] == 0xf0) { // SIMPLE KEY UP
        seq[1] = keyboard_read_scancode();
        numBytes = 2;
        //keyboard_action_t = KEYBOARD_ACTION_UP;
    } else { // SIMPLE KEY DOWN
        //keyboard_action_t = KEYBOARD_ACTION_DOWN;
        numBytes = 1;
    }
    return numBytes;
}

/*
 * `keyboard_read_event`: Mid level keyboard interface.
 *
 * The function reads (blocking) the next key action from the keyboard.
 * Returns a `key_event_t` struct that represents the key event.  The
 * struct includes the sequence of raw scan codes, the action (up or down),
 * and the state of the modifier flags in effect. If this event is a
 * a key down or up for a modifier key (CTRL, ALT, SHIFT, etc.), the
 * modifiers field in the event contains the state of the modifiers
 * after having incorporated this key action.
 * This function calls `keyboard_read_sequence`.
 stuff i need to work with.
    unsigned char seq[3];     // sequence of raw scan code bytes
    int seq_len;              // number of bytes in sequence
    ps2_key_t key;            // entry taken from ps2_keys table (see ps2.h)
    keyboard_action_t action; // either KEYBOARD_ACTION_UP or KEYBOARD_ACTION_DOWN
    keyboard_modifiers_t modifiers;   // modifiers in effect, composed of above bit flags
 */


key_event_t keyboard_read_event(void)
{
    key_event_t event;
    // add info on each one of these.
    // call keyboard_read_sequence.
    keyboard_read_sequence();
    // build the struct.


    return event;
}


unsigned char keyboard_read_next(void)
{
    // TODO: Your code here
    return '!';
}
