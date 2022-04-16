#include <Arduino.h>
#include <Wire.h>
#include "Preprocessing.h"
#include "MS5x.h"

MS5x barometer(&Wire);

double pressure = 0;
double temperature = 0;
double altitude = 0;

void setup(){
    Serial.begin(115200);
    Wire.begin();

    while(barometer.connect()>0) {
      //Serial.println(F("Error connecting..."));
      delay(500);
  }
    //Serial.println(F("Connected to Sensor"));
    barometer.setDelay(1000);

}

void loop(){

    barometer.checkUpdates();

    if(barometer.isReady()){
        temperature = barometer.GetTemp(); // Returns temperature in C
        pressure = barometer.GetPres(); // Returns pressure in Pascals

        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.print("\t");


        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print("\t");

        Serial.println(barometer.getSeaLevel(true));
    }

    /*
    delay(1000);
    /*
    int result = MS5611.read();   // uses default OSR_ULTRA_LOW  (fastest)
    Serial.println(MS5611.getPressure(), 2);

    delay(200);
    */
    
    /*
    start = micros();
    int result = MS5611.read();   // uses default OSR_ULTRA_LOW  (fastest)
    stop = micros();


    if (count % 20 == 0)
    {
    Serial.println();
    Serial.println("CNT\tDUR\tRES\tTEMP\tPRES");
    }

    Serial.print(count);
    count++;
    Serial.print("\t");
    Serial.print(stop - start);
    Serial.print("\t");
    Serial.print(result);
    Serial.print("\t");
    Serial.print(MS5611.getTemperature(), 2);
    Serial.print("\t");
    Serial.print(MS5611.getPressure(), 2);
    Serial.println();
    */
}

