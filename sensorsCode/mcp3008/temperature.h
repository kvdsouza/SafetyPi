#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include <math.h>

int thermister(int RawADC);
int calibrate();
void setLights(int average, int tempReading);


#endif
