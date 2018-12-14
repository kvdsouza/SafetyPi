/* Simple SPI + MCP3008 ADC test. */
#include "gpio.h"
#include "uart.h"
#include "mcp3008.h"
#include "printf.h"
#include "temperature.h"
#include "strings.h"
//#include "gpioextra.h"
//#include "temperature.c"
#include <math.h>       // probs not needed.
#include "timer.h"
#include "keyboard.h"
#include "console.h"
#include "shell.h"
#include "sensors.h"



const unsigned int DIGITAL = GPIO_PIN24;
const unsigned int TEMP2SIGNAL = GPIO_PIN16;
//const unsigned int LIGHT1 = GPIO_PIN05;
volatile unsigned int tracker = 0;

void main(void)
{
    gpio_init();
    gpio_set_input(DIGITAL);        // temp sensor 1
    gpio_set_input(TEMP2SIGNAL);    // DIGITAL TEMP.
    gpio_set_output(GPIO_PIN5);
    gpio_set_output(GPIO_PIN6);
    gpio_set_output(GPIO_PIN13);
    gpio_set_output(GPIO_PIN19);

    uart_init();
    mcp3008_init();
    console_init(20, 40);
    const int tempAverage = calibrate(mcp3008_read(0));
    const int temp2Avg = calibrate(mcp3008_read(1));
    const int flameAvg = calibrate(mcp3008_read(2));
    const int gasAvg = calibrate(mcp3008_read(3));
    //temperature_init();
    // COLLECT FIRST 5 SECONDS AND CALIBRATE.
    // timer delay (secs)
    // read mcp scan and have a counter.
    // divide at the end and get the magic.
    // int counter = 0;
    while (1) {
        // read channel 0
        // uart_flush() potentially?
        //printf("my mcp is %d\n", mcp3008_read(0));
        // printf("my thermister is %d\n", thermister(mcp3008_read(0)));
        // printf("my DIGTEMP is %d\n", mcp3008_read(1));
        console_printf("The logistic temperature is %d.\n", thermister(mcp3008_read(0)));
        printSensors(temp2Avg, flameAvg, gasAvg);
        // console_printf("The other temperature is %d.\n", mcp3008_read(1));
        // console_printf("The flame reader is %d. \n", mcp3008_read(2));      // basically write a quick thing to say whether it's on or not.
        // console_printf("The gas detector is %d. \n", mcp3008_read(3));

        //console_printf("The flame is %s", )
        // gas stuff
        setLights(tempAverage, mcp3008_read(0));
        broadcastTemp(tempAverage, mcp3008_read(0));
        //printf("\n");
        timer_delay_ms(500);
        //gl_swap_buffer();
        console_clear();


        //uart_getLine(); figure out how that goes somewhere.
    }
}
