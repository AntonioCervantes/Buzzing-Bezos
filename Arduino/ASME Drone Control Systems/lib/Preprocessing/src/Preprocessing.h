#ifndef Preprocessing_h
#define Preprocessing_h

#include <Arduino.h>


float compFilter(float signal1, float signal2, float alpha);

float rotatingAverage(int arrSize, float *memArray, int *index, float newDataPoint);

#endif