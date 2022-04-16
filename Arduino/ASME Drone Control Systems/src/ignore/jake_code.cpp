#include <Arduino.h>
//mpu
#include "MPU9250.h"
MPU9250 mpu;

//barometer
#include <Wire.h>
#include <MS5x.h>
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
... (7 lines left)
Collapse
mpu_bar_gps_v2.ino
3 KB
banditgawd — 03/30/2022
also, where are the BB sensors?
jfulton — 03/30/2022
i saw someone messing with them today in 141
i think it may have been gordon
also might have been antonio
or tyler
i don't remember lol
banditgawd — 03/31/2022
dawgy dawg, lol, so like, if even if i managed to get a Tower ID, i wouldn't be in the srac system?
jfulton — 03/31/2022
it boils down to if you pay the student union fee or not
if you are doing an off-campus degree, i'd be willing to bet that you wouldn't pay the student union fee
banditgawd — 04/05/2022
aye i just realized: since the base is essentially an empty box, won't the base collapse from the weight of the robot?
jfulton — 04/05/2022
not necessarily
if it can't hold though i'll just create supports which should be an easy fix
i'll do a static test on solid works right now to see what we're working with
jfulton — 04/05/2022
if i did this right (lol), with 5 lbs applied there's a factor of safety of 13 for yield stress and a max displacement of 0.163 mm
at 10 lbs of force there's a 6.5 factor of safety and 0.325 displacement
banditgawd — 04/08/2022
almost forgot: happy birthday bruh bruh. glad to be ur engr partner and i can honestly call u a good friend too 🙃
let’s try hanging out outside engr stuff sometime again w antonio
jfulton — 04/09/2022
thanks my guy. yeah that'd be cool!
banditgawd — 04/11/2022
aye man, thoughts on this role? 
https://jobs.lever.co/dexterity/7f8dd46a-eb8e-49f4-8c2f-88817d45e9ea 
Dexterity - Product Operations Engineer - Hardware
As a Product Operations Engineer you will assist with deployment and provisioning of Dexterity’s robotic systems. You will bridge the gap between design engineers and assembly technicians. Everyday you will identify opportunities for process improvement in engineering, operations, documentation, procurement, supply chain. In this role you will b...
Image
(for context, i applied for other roles such as Robotics Software Engineer, and they play a reverse uno card by asking me if i was interested in this role instead)
jfulton — 04/11/2022
it looks like a good entry level job
but it doesn't seem like you'd be working on code development
it also requires kind of a lot of travel so make sure you take that into consideration
but it would be cool to work hands on with everythig
i approve lol
banditgawd — 04/11/2022
thx man
jfulton — 04/11/2022
👍
banditgawd — 04/12/2022
ayo did anything important happen in ME 100 yesterday ?
jfulton — 04/12/2022
we just reviewed past portfolios
she posted an assignment on canvas where she wants us to start our pitch videos
banditgawd — 04/12/2022
thank you
jfulton — 04/13/2022
are you in 135?
banditgawd — 04/13/2022
yee
jfulton — Yesterday at 5:19 PM
do you have an album of the pictures youve taken while weve worked on scar in person?
banditgawd — Yesterday at 5:20 PM
it's on the drive
jfulton — Yesterday at 5:20 PM
got it thanks
jfulton — Today at 11:33 AM
how did the interviews yesterday go?
﻿

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