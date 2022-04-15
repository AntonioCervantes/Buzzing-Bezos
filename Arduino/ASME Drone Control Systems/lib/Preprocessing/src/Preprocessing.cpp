
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

/* ROLLING MEMORY
 *  This function takes in an array of fixed size, and then continuously replaces 
 * the current index with a new data point. this creates a moving average.
 */
float rotatingAverage(int arrSize, float *memArray, int *index, float newDataPoint){
  memArray[*index] = newDataPoint; //assign current index the newdatapp
  
  // Calculate Average
  float sum = 0;
  for(int i = 0; i < arrSize; i++) {// Sum of Array
      
      sum += memArray[i];
      if(DEBUG){

        Serial.print("i: ");
        Serial.print(i);
        Serial.print("\t");

        Serial.print("*index: ");
        Serial.print(*index);
        Serial.print("\t");


        Serial.print("Sum: ");
        Serial.print(sum);
        Serial.print("\t");

        Serial.print("Point: ");
        Serial.print(memArray[i]);
        Serial.println("\t");
      }
    }
 
  float average = sum / arrSize;
  
  if(DEBUG){

    Serial.print("Average: ");
    Serial.print(average);
    Serial.print("\t");

    Serial.print("Index*: ");
    Serial.print(*index);
    Serial.println("\t");
  }

  //Iterate 
  *index = *index + 1;

  if(*index == (arrSize)){

    *index = 0; //if index exceeds max size of array, go back to 0 
  }
  return average;
}

// (1) Update 1 point. (2) Average. (3) Iterate Next Point. -- *for iteration, go to beginning