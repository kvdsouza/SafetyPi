#include "gpio.h"
#include "gpioextra.h"
#include "keyboard.h"
#include "ps2.h"

const unsigned int CLK  = GPIO_PIN23;
const unsigned int DATA = GPIO_PIN24;
unsigned int LEV0 = 0x20200034;

static unsigned int scrollLock = 0;
static unsigned int numLock = 0;
static unsigned int capsLock = 0;
static unsigned int shift = 0;
static unsigned int alt = 0;
static unsigned int ctrl = 0;

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

/* reads one set of scancodes! */
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
        //READ FIRST BIT.
        wait_for_falling_clock_edge();
        if (gpio_read(DATA) != 0) {    //IF NOT SET TO LOW, LOOP AGAIN.
             continue;
        }
        wait_for_falling_clock_edge();
        // read data bits.
        for (int i = 0 ; i < 8; i++) {
            contents = gpio_read(DATA);
            result = result + (contents << i);
            if (contents == 1) {
                counter++;
            }
            wait_for_falling_clock_edge();
        }
        // comparing parity bit with counter.
        if ((gpio_read(DATA) + counter) % 2 == 0) {
            continue; // LOOP BACK.
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

/* reads sequence - returns number of bytes */
int keyboard_read_sequence(unsigned char seq[])
{
    int numBytes;
    seq[0] = keyboard_read_scancode();

    if (seq[0] == PS2_CODE_EXTEND) { // EXTENDED KEY CASE
        seq[1] = keyboard_read_scancode();
        if (seq[1] == PS2_CODE_RELEASE) { // EXTENDED KEY UP
            seq[2] = keyboard_read_scancode();
            numBytes = 3;
        } else { // EXTENDED KEY DOWN.
            numBytes = 2;
        }
    } else if (seq[0] == PS2_CODE_RELEASE) { // SIMPLE KEY UP
        seq[1] = keyboard_read_scancode();
        numBytes = 2;
    } else { // SIMPLE KEY DOWN
        numBytes = 1;
    }
    return numBytes;
}

/*
 * `keyboard_read_event`: Mid level keyboard interface.
 */
key_event_t keyboard_read_event(void)
{
    key_event_t event;
    event.seq_len = keyboard_read_sequence(event.seq); // NOTATE LENGTH.
    // SET ACTION AND KEY.
    if (event.seq_len == 1) {
        event.action = KEYBOARD_ACTION_DOWN;  // simple key down
        event.key = ps2_keys[event.seq[0]];
    } else if (event.seq_len == 2) {
        if (event.seq[0] == PS2_CODE_EXTEND) { // extended down
            event.action = KEYBOARD_ACTION_DOWN;
            event.key = ps2_keys[event.seq[1]];
        } else if (event.seq[0] == PS2_CODE_RELEASE) { // simple key up.
            event.action = KEYBOARD_ACTION_UP;
            event.key = ps2_keys[event.seq[1]];
        }
    } else {
        event.action = KEYBOARD_ACTION_UP;  // extended key up.
        event.key = ps2_keys[event.seq[2]];
    }

    // check for modifiers. shift/alt/ctrl/scrllock/numLock/capsLock = 1 if on.
    if (event.seq[0] == 0x12 || event.seq[0] == 0x59) {
        shift = 1;
    } else if (event.seq[0] == 0x11 || event.seq[1] == 0x11) { // left and right check.
        alt = 1;
    } else if (event.seq[0] == 0x14 || event.seq[1] == 0x14) {
        ctrl = 1;
    }

    if (event.seq[0] == 0x7e) {
        scrollLock = (scrollLock + 1) % 2;
    } else if (event.seq[0] == 0x77) {
        numLock = (numLock + 1) % 2;
    } else if (event.seq[0] == 0x58) {
        capsLock = (capsLock + 1) % 2;
    }

    // release of codes.
    if (event.seq[0] == PS2_CODE_RELEASE || event.seq[1] == PS2_CODE_RELEASE) {
        // NOW RELEASE SHIFT, ALT, CTRL
        if (event.seq[1] == 0x12 || event.seq[1] == 0x59) {
            shift = 0;
        } else if (event.seq[1] == 0x11 || event.seq[2] == 0x11) {
            alt = 0;
        } else if (event.seq[1] == 0x14 || event.seq[2] == 0x14) {
            ctrl = 0;
        }
    }

    event.modifiers = 0x00; // bit manipulation below to slot into modifiers struct.
    if (scrollLock == 1) {
        event.modifiers = (event.modifiers | KEYBOARD_MOD_SCROLL_LOCK); }
    if (numLock == 1) {
        event.modifiers = (event.modifiers | KEYBOARD_MOD_NUM_LOCK); }
    if (capsLock == 1) {
        event.modifiers = (event.modifiers | KEYBOARD_MOD_CAPS_LOCK); }
    if (shift == 1) {
        event.modifiers = (event.modifiers | KEYBOARD_MOD_SHIFT); }
    if (alt == 1) {
        event.modifiers = (event.modifiers | KEYBOARD_MOD_ALT); }
    if (ctrl == 1) {
        event.modifiers = (event.modifiers | KEYBOARD_MOD_CTRL); }
    return event;
}


/*
 * `keyboard_read_next`: Top level keyboard interface.
 */
unsigned char keyboard_read_next(void)
{
    unsigned char value = 0; // always return this value at the end.
    while (1) {
        key_event_t nextEvent = keyboard_read_event();
        unsigned int modifier = 0; // helper variable.
        unsigned int shiftMod = 0; // keeps track of shift
        unsigned int capsMod = 0;  // keeps track of caps
        // check for modifiers if so: continue and loop again.
        if ( (nextEvent.key.ch == PS2_KEY_SHIFT) || (nextEvent.key.ch == PS2_KEY_ALT) ||
            (nextEvent.key.ch == PS2_KEY_CTRL) || (nextEvent.key.ch == PS2_KEY_CAPS_LOCK) ||
            (nextEvent.key.ch == PS2_KEY_NUM_LOCK) || (nextEvent.key.ch == PS2_KEY_SCROLL_LOCK))
            { continue;}
        // we only want down action - eliminates duplicates.
        if (nextEvent.action == KEYBOARD_ACTION_UP) {
            continue;
        }
        modifier = nextEvent.modifiers;
        shiftMod = modifier & KEYBOARD_MOD_SHIFT; // get specific shift info.
        capsMod = modifier & KEYBOARD_MOD_CAPS_LOCK; // same deal for caps

        if (capsMod == KEYBOARD_MOD_CAPS_LOCK ||shiftMod == KEYBOARD_MOD_SHIFT) {
            if (capsMod == KEYBOARD_MOD_CAPS_LOCK) {
                unsigned char ch = nextEvent.key.ch;
                if (ch <= 'z' && ch >= 'a') {   // set of keys that get caps modified.
                    value = nextEvent.key.other_ch;
                } else {
                    value = nextEvent.key.ch;
                }
            }
            if (shiftMod == KEYBOARD_MOD_SHIFT) {
                unsigned char ch = nextEvent.key.other_ch;
                if (ch != 0) {  // these keys get modified.
                    value = nextEvent.key.other_ch;
                } else {
                    value = nextEvent.key.ch;
                }
            }
        } else {
            value = nextEvent.key.ch;       // otherwise just revert to normal.
        }
        break;
    }
    return value;
}
