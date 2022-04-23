#include <Arduino.h>
#include <Wire.h>
#include "Preprocessing.h"


// barometer
#include "MS5x.h"
MS5x barometer(&Wire);


//gps
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
SFE_UBLOX_GNSS myGNSS;
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); //RX, TX on Mega

double seaLevelPressure = 0;


void setup(){
    Serial.begin(115200);
    Wire.begin();

    while(barometer.connect()>0) {
        Serial.println(F("Error connecting..."));
        delay(500);
    }
    do {
        Serial.println("GNSS: trying 38400 baud");
        mySerial.begin(9600);
        if (myGNSS.begin(mySerial) == true) break;

        delay(100);
        Serial.println("GNSS: trying 9600 baud");
        mySerial.begin(9600);
        if (myGNSS.begin(mySerial) == true) {
            Serial.println("GNSS: connected at 9600 baud, switching to 9600");
            myGNSS.setSerialRate(9600);
            delay(100);
        } else {
            //myGNSS.factoryReset();
            delay(2000); //Wait a bit before trying again to limit the Serial output
        }
    } while(1);
    Serial.println("GNSS serial connected");
    /*
    int i = 0;
    do{ //Connect to the u-blox module using Wire port
        Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
        i++;
        delay(1000);
     } while (myGNSS.begin(mySerial) == false);
     Serial.println("GNSS connected to I2C.");
    */

    //Serial.println("GNSS serial connected");
    myGNSS.setUART1Output(COM_TYPE_UBX);
    myGNSS.setI2COutput(COM_TYPE_UBX);
    myGNSS.saveConfiguration();

    //Serial.println(F("Connected to Sensor"));
    barometer.setDelay(1000);


}

void loop(){


    //barometer.checkUpdates();
    
    if(barometer.isReady()){
    
        if (seaLevelPressure == 0) seaLevelPressure = barometer.getSeaLevel(29); 
        
        // Altitude from given seaLevelPressure
        Serial.print(barometer.getAltitude(true));
        Serial.print("\t");

    }

    /*
    Serial.print(myGNSS.getLatitude());
    Serial.print("\t");
    Serial.print(myGNSS.getLongitude());
    Serial.print("\t");
    */
    Serial.print(myGNSS.getAltitude());
    Serial.print("\n");

}
