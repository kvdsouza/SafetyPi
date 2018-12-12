/* Simple SPI + MCP3008 ADC test. */
#include "gpio.h"
#include "uart.h"
#include "mcp3008.h"
#include "printf.h"
#include "temperature.h"
//#include "gpioextra.h"
//#include "temperature.c"
#include <math.h>       // probs not needed.


const unsigned int DIGITAL = GPIO_PIN24;
//const unsigned int LIGHT1 = GPIO_PIN05;

void main(void)
{
    gpio_init();
    gpio_set_input(DIGITAL);
    gpio_set_output(GPIO_PIN5);
    gpio_set_output(GPIO_PIN6);
    gpio_set_output(GPIO_PIN13);
    gpio_set_output(GPIO_PIN19);
    uart_init();
    mcp3008_init();
    const int average = calibrate();
    //temperature_init();
    // COLLECT FIRST 5 SECONDS AND CALIBRATE.
    // timer delay (secs)
    // read mcp scan and have a counter.
    // divide at the end and get the magic.

    while (1) {
        // read channel 0
        printf("my mcp is %d\n", mcp3008_read(0));
        printf("my thermister is %d\n", thermister(mcp3008_read(0)));
        printf("my gpio is %d\n", gpio_read(DIGITAL));
        setLights(average, mcp3008_read(0));
        //uart_getLine(); figure out how that goes somewhere.
    }
}
