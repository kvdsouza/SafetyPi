/*
timer.c has all functions related to timers.
*/

#include "timer.h"
unsigned int CLO = 0x20003004; // lower 32 bits

void timer_init(void) {
}

// timer_get_ticks collects the number of ticks.
unsigned int timer_get_ticks(void) {
   volatile unsigned int ticks = *(unsigned int *)(CLO);
   return ticks;  // TODO: Your code goes here.
}

void timer_delay_us(unsigned int usecs) {
    unsigned int start = timer_get_ticks();
    while (timer_get_ticks() - start < usecs) { /* spin */ }
}

void timer_delay_ms(unsigned int msecs) {
    timer_delay_us(1000*msecs);
}

void timer_delay(unsigned int secs) {
    timer_delay_us(1000000*secs);
}
