#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include <math.h>
#include "gpio.h"

int thermister(int RawADC);
int calibrate();
void setLights(int average, int tempReading);
void broadcastTemp(int average, int tempReading);


#endif
