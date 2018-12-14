// temperature - adopted from https://forum.arduino.cc/index.php?topic=209133.0

#include "gpio.h"
#include "uart.h"
#include "mcp3008.h"
#include "printf.h"
#include "temperature.h"
#include "timer.h"
#include <math.h>


int thermister(int RawADC) {
    double temp;
    //temp = RawADC / 10;
    //printf("%d\n", RawADC);
    //printf("helllloo\n");
    //printf("%d\n", temp);
    // temp = (10000.0*((1024.0/RawADC-1)));
    // //temp = log(((10240000/RawADC) - 10000));
    // temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
    // temp = temp - 273.15;// Convert Kelvin to Celcius
    // //temp = (temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    //return temp;
    temp = -51.1*(log(RawADC) / log(2.72)) + 338.54;   // trendline.
    temp = (temp * 1.8) + 32;     // checker
    // double voltage;
    // voltage = (RawADC)*(3.3/4095);
    // temp = log(10000*(3.3/voltage - 1));
    //  //Temp = log(((10240000/RawADC) - 10000));
    // temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
    // temp = temp - 273.15;// Convert Kelvin to Celcius
    return (int)temp;
}


int calibrate() {
    int totalTemp = 0;
    int numReps = 5000;
    for (int i = 0; i < numReps; i++) {
        totalTemp += mcp3008_read(0);
        timer_delay_ms(1);
    }
    return totalTemp / numReps;
}

void setLights(int average, int tempReading) {
    if (tempReading >= average - 20 || tempReading == 0) {
        gpio_write(GPIO_PIN5, 1);
        gpio_write(GPIO_PIN6, 0);
        gpio_write(GPIO_PIN13, 0);
        gpio_write(GPIO_PIN19, 0);
    } else if (tempReading <= average - 20 && tempReading >= average - 40 && tempReading != 0) {
        gpio_write(GPIO_PIN5, 1);
        gpio_write(GPIO_PIN6, 1);
        gpio_write(GPIO_PIN13, 0);
        gpio_write(GPIO_PIN19, 0);
    } else if (tempReading < average - 40 && tempReading >= average - 60 && tempReading != 0) {
        gpio_write(GPIO_PIN5, 1);
        gpio_write(GPIO_PIN6, 1);
        gpio_write(GPIO_PIN13, 1);
        gpio_write(GPIO_PIN19, 0);
    } else if (tempReading < average - 60 && tempReading != 0) {
        gpio_write(GPIO_PIN5, 1);
        gpio_write(GPIO_PIN6, 1);
        gpio_write(GPIO_PIN13, 1);
        gpio_write(GPIO_PIN19, 1);
    }
}
    // COLLECT FIRST 5 SECONDS AND CALIBRATE.
    // timer delay (secs)
    // read mcp scan and have a counter.
    // divide at the end and get the magic.
    //return 0;
void broadcastTemp(int average, int tempReading) {
    char tempString[4];
    char *ch = tempString;
    signed_to_base(ch, 10, tempReading, 10, 0);
    // we could do something with sending that over with char *
    // so we want a reading for when this happened: stove is safe.
    if (tempReading >= average - 20) {
        uart_putchar('a');
    // we also want a rating for when stove is heating up.
    } else if (tempReading <= average - 20 && tempReading >= average - 40) {
        uart_putchar('b');
    // rating for when stove is hot.
    } else if (tempReading < average - 40 && tempReading >= average - 60) {
        uart_putchar('c');
    // stove supervision is necessary.
    } else if (tempReading < average - 60) {
        uart_putchar('d');
    }
}


    // temp = log(((10240000/RawADC) - 10000));
    // temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
    // temp = temp - 273.15;            // Convert Kelvin to Celcius
    // temp = (temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    // return (int)temp;



// now we want to read on the digital side as well to get gpio pins on.
//int digital pin
