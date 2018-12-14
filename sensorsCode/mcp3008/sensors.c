#include "gpio.h"
#include "uart.h"
#include "mcp3008.h"
#include "printf.h"
#include "temperature.h"
#include "timer.h"
#include <math.h>
#include "console.h"
#include "sensors.h"

const int TEMPDIFFERENCE = 3;
const int FLAMEDIFFERENCE = 100;
const int GASDIFFERENCE = 50;

// sensor library - just one function with calibration for now.
void printSensors(int temp2Avg, int flameAvg, int gasAvg) {
    // backup temp sensor.
    if (mcp3008_read(1) > TEMPDIFFERENCE + temp2Avg) {
        console_printf("\nSecond Temperature Sensor heating up\n");
    } else {
        console_printf("Second Temperature Sensor stabilized\n");
    }
    // flame sensor.
    if (mcp3008_read(2) > FLAMEDIFFERENCE + flameAvg) {
        console_printf("Flame is on!\n");
    } else {
        console_printf("Flame is off!\n");
    }
    // gas sensor.
    if (mcp3008_read(3) > GASDIFFERENCE + gasAvg) {
        console_printf("Gas detected\n");
    } else {
        console_printf("No Significant Gas Detected\n");
    }

}
