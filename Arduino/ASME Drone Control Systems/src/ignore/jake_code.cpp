
#include <Arduino.h>
//mpu
#include "MPU9250.h"
MPU9250 mpu;

//barometer
#include <Wire.h>
#include <MS5x.h>
MS5x barometer(&Wire);
uint32_t prevTime;
double prevPressure=0;
double prevTemperature=0;
double seaLevelPressure = 0;

//gps
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
SFE_UBLOX_GNSS myGNSS;
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
long lastTime = 0;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    delay(2000);
    while (!Serial);
    //Serial.println("SparkFun u-blox Example");
    do {
      //Serial.println("GNSS: trying 38400 baud");
      mySerial.begin(38400);
      if (myGNSS.begin(mySerial) == true) break;
      delay(100);
      //Serial.println("GNSS: trying 9600 baud");
      mySerial.begin(9600);
      if (myGNSS.begin(mySerial) == true) {
        //Serial.println("GNSS: connected at 9600 baud, switching to 38400");
        myGNSS.setSerialRate(38400);
        delay(100);
      } else {
        delay(2000);
      }
    } while(1);
    //Serial.println("GNSS serial connected");
    myGNSS.setUART1Output(COM_TYPE_UBX);
    myGNSS.setI2COutput(COM_TYPE_UBX);
    myGNSS.saveConfiguration();

    if (!mpu.setup(0x68)) {  
        while (1) {
            //Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }
    
    while(barometer.connect()>0) {
      //Serial.println(F("Error connecting..."));
      delay(500);
  }
    //Serial.println(F("Connected to Sensor"));
    barometer.setDelay(1000);
}

void loop() {

    double pressure = 0;
    double temperature = 0;
    double altitude = 0;
    barometer.checkUpdates();
    
    if (mpu.update() && barometer.isReady()) {
        static uint32_t prev_ms = millis();
        temperature = barometer.GetTemp(); // Returns temperature in C
        pressure = barometer.GetPres(); // Returns pressure in Pascals
        if (millis() > prev_ms + 43) {
            print_gps_mpu_barometer();
            prev_ms = millis();
        }
    }    
}

void print_gps_mpu_barometer() {
    Serial.print(mpu.getYaw());
    Serial.print("\t");
    Serial.print(mpu.getPitch());
    Serial.print("\t");
    Serial.print(mpu.getRoll());
    Serial.print("\t");
    Serial.print(barometer.GetPres());
    Serial.print("\t");
    Serial.print(barometer.GetTemp());
    Serial.print("\t");
    Serial.print(barometer.getSeaLevel(217.3));
    Serial.print("\t");
    Serial.print(barometer.getAltitude());
    Serial.print("\t");
    Serial.print(barometer.getAltitude(true));
    Serial.print("\t");
    Serial.print(myGNSS.getLatitude());
    Serial.print("\t");
    Serial.print(myGNSS.getLongitude());
    Serial.print("\t");
    Serial.print(myGNSS.getAltitude());
    Serial.print("\t");
    Serial.print(myGNSS.getSIV());
    Serial.println();
}