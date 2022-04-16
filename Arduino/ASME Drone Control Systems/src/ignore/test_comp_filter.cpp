#include <Arduino.h>
#include "Preprocessing.h"
#include "MS5611.h"
#include "Wire.h"

MS5611 MS5611(0x77);
uint32_t start, stop, count;


void setup(){
    Serial.begin(115200);
    Wire.begin();
    MS5611.begin();

    count = 0;
}

void loop(){

    delay(1000);

    start = micros();
    int result = MS5611.read();   // uses default OSR_ULTRA_LOW  (fastest)
    stop = micros();

    if (count % 20 == 0)
    {
    Serial.println();
    Serial.println("CNT\tDUR\tRES\tTEMP\tPRES");
    }

    Serial.print("\t");
    Serial.print(stop - start);
    Serial.print("\t");
    Serial.print(result);
    Serial.print("\t");
    Serial.print(MS5611.getTemperature(), 2);
    Serial.print("\t");
    Serial.print(MS5611.getPressure(), 2);
    Serial.println();
}

