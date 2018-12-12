// temperature - adopted from https://forum.arduino.cc/index.php?topic=209133.0

#include "gpio.h"
#include "uart.h"
#include "mcp3008.h"
#include "printf.h"
#include "temperature.h"
#include "timer.h"

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

     double voltage;
     voltage = (RawADC)*(3.3/4095);
     temp = log(10000*(3.3/voltage - 1));
     //Temp = log(((10240000/RawADC) - 10000));
     temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
     temp = temp - 273.15;// Convert Kelvin to Celcius
     return temp;
}


int calibrate() {
    int totalTemp = 0;
    for (int i = 0; i < 5000; i++) {
        totalTemp += mcp3008_read(0);
        timer
    }
    int numSamples = i + 1;


    // COLLECT FIRST 5 SECONDS AND CALIBRATE.
    // timer delay (secs)
    // read mcp scan and have a counter.
    // divide at the end and get the magic.

    return 0;
}


    // temp = log(((10240000/RawADC) - 10000));
    // temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp ))* temp );
    // temp = temp - 273.15;            // Convert Kelvin to Celcius
    // temp = (temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    // return (int)temp;



// now we want to read on the digital side as well to get gpio pins on.
//int digital pin
