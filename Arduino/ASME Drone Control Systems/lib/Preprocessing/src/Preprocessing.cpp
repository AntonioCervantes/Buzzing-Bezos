
#include "Arduino.h"
#include "Preprocessing.h"

#define DEBUG 0

/* COMPLIMENTARY FILTER
 * This function takes two different signals and then performs
 * a weighted sum based on value 'alpha' to produce a filtered signal.
 * Note: The higher the 'alpha' term, the more closely the filtered 
 * signal is to 'signal1'.
*/
float compFilter(float signal1, float signal2, float alpha){
  float filtered_signal = signal1*alpha + signal2*(1-alpha);
  return filtered_signal;
}


/* AVERAGER
  This class accepts the size of the running filter for the average data.
*/
Averager::Averager(int size){
  _size = size;
  _temp = 0;
  _index = &_temp;
  _array[_size];

}

/* ROLLING MEMORY
*  This function returns the running averarge based on the array size
*/
float Averager::getRunningAverage(float newDataPoint){
  _array[*_index] = newDataPoint; //assign current index the newdatapp
  
  // Calculate Average
  float sum = 0;
  for(int i = 0; i < _size; i++) {// Sum of Array
      
      sum += _array[i];
    }

  float average = sum / _size;
  

  //Iterate 
  *_index = *_index + 1;

  //if index exceeds max size of array, go back to 0 
  if(*_index == (_size)){

    *_index = 0; 
  }

  return average;
}

float Averager::getAveragerSize(){
  return _size;
}

// (1) Update 1 point. (2) Average. (3) Iterate Next Point. -- *for iteration, go to beginning