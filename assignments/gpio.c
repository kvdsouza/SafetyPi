/*
gpio.c sets pins, gets pin values, reads, and writes. Implementation here.
Kyle D'Souza

*/
#include "gpio.h"

// constants
unsigned int FSEL0 = 0x20200000;
unsigned int FSEL1 = 0x20200004;
unsigned int FSEL2 = 0x20200008;
unsigned int FSEL3 = 0x2020000C;
unsigned int FSEL4 = 0x20200010;
unsigned int FSEL5 = 0x20200014;
unsigned int SET0 = 0x2020001C;
unsigned int SET1 = 0x20200020;
unsigned int CLR0 = 0x20200028;
unsigned int CLR1 = 0x2020002C;
unsigned int LEV0 = 0x20200034;
unsigned int LEV1 = 0x7E200038;

void gpio_init(void) {
}

//my helper functions here.
/*
readContents function dereferences and reads FSEL data.
*/
unsigned int readContents(unsigned int pin) {
    unsigned int contents;
    if (pin < 0 || pin > 53) {
	     return GPIO_INVALID_REQUEST; // check for errors.
    }
    if (pin >= 0 && pin <= 9) {
        contents = *(unsigned int *)(FSEL0);
    } else if (pin >= 10 && pin <= 19) {
        contents = *(unsigned int *)(FSEL1);
    } else if (pin >= 20 && pin <= 29) {
        contents = *(unsigned int *)(FSEL2);
    } else if (pin >= 30 && pin <= 39) {
        contents = *(unsigned int *)(FSEL3);
    } else if (pin >= 40 && pin <= 49) {
        contents = *(unsigned int *)(FSEL4);
    } else if (pin >= 50 && pin <= 53) {
        contents = *(unsigned int *)(FSEL5);
    } else {
        contents = 0; // just to get rid of warnings on compiler - make it happy :)
    }
    return contents;
}

/*
readSetClr dereferences, reads, and writes SETCLR data.
*/
void read_Set_Clr(unsigned int pin, unsigned int setOrClr) {
  unsigned int contents = *(unsigned int *)(setOrClr);
  unsigned int mask = (1 << ( pin %32)); // masking.
  contents = (contents | mask);
  *(unsigned int *)(setOrClr) = mask;
}

// sets a GPIO pin to a certain function.
void gpio_set_function(unsigned int pin, unsigned int function) {

    unsigned int contents = readContents(pin);

    // first two statements are error checks.
    if (pin < 0 || pin > 53 || (contents == GPIO_INVALID_REQUEST)) { // pin test
    } else if (function < 0 || function > 7) { // function test
    } else {
    // what GPIO pin location is this.
      unsigned int location = (3*(pin%10));
    // newMask puts 1s in pin's location, and 0s elsewhere.
      unsigned int newMask = (7 << location);
    // now, newMask is modified so that all pins except selected pin are 1s (i.e. flip pins).
      newMask = ~(newMask);
    // contents is combined with newMask s.t. all pins revert back to existing, except for pin location (all 0's).
      contents = (contents & newMask);
    // mask contents s.t. pin reflects the function passed through.
      contents = (contents | (function << location));

    // writes function back into memory location.
      if (pin >= 0 && pin <= 9) {
          *(unsigned int *)(FSEL0) = contents;
      } else if (pin >= 10 && pin <= 19) {
          *(unsigned int *)(FSEL1) = contents;
      } else if (pin >= 20 && pin <= 29) {
          *(unsigned int *)(FSEL2) = contents;
      } else if (pin >= 30 && pin <= 39) {
          *(unsigned int *)(FSEL3) = contents;
      } else if (pin >= 40 && pin <= 49) {
          *(unsigned int *)(FSEL4) = contents;
      } else if (pin >= 50 && pin <= 53) {
          *(unsigned int *)(FSEL5) = contents;
      }
    }
}

/*
gpio_get_function takes in a pin value and returns its value (i.e. input, output, etc.)
*/
unsigned int gpio_get_function(unsigned int pin) {
    unsigned int contents = readContents(pin);
    if (contents == GPIO_INVALID_REQUEST) {
      return GPIO_INVALID_REQUEST; // return failure.
    }
    // determines location
    unsigned int location = 3*(pin%10);

    // right shift to get to correct pin.
    unsigned int pinContents = (contents >> location);

    unsigned int value = pinContents & 7;
    // then we store and submit
    return value;
}

/*
  gpio_set_input sets function to input.
*/
void gpio_set_input(unsigned int pin) {
    gpio_set_function(pin, 0);
}

/*
  gpio_set_output sets function to output.
*/
void gpio_set_output(unsigned int pin) {
    gpio_set_function(pin, 1);
}

/*
  gpio_write allows one to set or clear values.
*/
void gpio_write(unsigned int pin, unsigned int value) {
    // error checks.
    if (pin < 0 || pin > 53) { // error check
    } else if (value == 0) { // for Clear

// CLR0 case.
      if (pin <= 31) {
          read_Set_Clr(pin, CLR0);
// CLR1 case.
	    } else {
          read_Set_Clr(pin, CLR1);
	    }
// SET case.
    } else if (value == 1) {
      // SET1
        if (pin <= 31) {
            read_Set_Clr(pin, SET0);
      // SET2
	      } else {
            read_Set_Clr(pin, SET1);
	      }
    }
}

/*
gpio_read reads pin and returns function contents.
*/
unsigned int gpio_read(unsigned int pin) {
    // do the check first for GPIO-INVALID_REQUEST
    if (pin < 0 || pin > 53) {
	     return GPIO_INVALID_REQUEST;
    } else if (pin <= 31) {
   	   volatile unsigned int contents = *(volatile unsigned int *)(LEV0);
	     contents = (contents >> pin);
	     contents = (contents & 0x1);
	     return contents;
    } else {
	     volatile unsigned int contents1 = *(volatile unsigned int *)(LEV1);
	     contents1 = (contents1 >> (pin%32));
	     contents1 = (contents1 & 0x1);
	     return contents1;
    }
}
