#ifndef Preprocessing_h
#define Preprocessing_h

#include <Arduino.h>


float compFilter(float signal1, float signal2, float alpha);

float rotatingAverage(int arrSize, float *memArray, int *index, float newDataPoint);

class Averager{
  public:
    Averager(int size);
    float getRunningAverage(float newDataPoint);
    float getAveragerSize();

  private:
    int _temp;
    int* _index;
    int _size;
    float _array[];
};

// (1) Update 1 point. (2) Average. (3) Iterate Next Point. -- *for iteration, go to beginning

#endif