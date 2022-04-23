#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// Include libraries
#include "MS5x.h" // barometer
#include "MPU9250.h" // mpu
#include "SparkFun_u-blox_GNSS_Arduino_Library.h" // GPS
#include "Preprocessing.h"
#include "PID_v1.h"

// Global Constants
#define ARRSIZE 20
#define MAIN_DEBUG 1

// Initialize Sensors
MS5x barometer(&Wire); // barometer
MPU9250 mpu; // mpu
SFE_UBLOX_GNSS myGNSS; SoftwareSerial mySerial(10, 11); // gps

// Initialize sensory data
double pressure = 0;
double temperature = 0;
double altitude = 0;

// Initialize Averagers
Averager AltitudeAverager(ARRSIZE);
Averager YawAverager(ARRSIZE);
Averager PitchAverager(ARRSIZE);
Averager RollAverager(ARRSIZE);

// define Gain Struct
struct PID_Controls{

    float ref;

    int Ki;
    int Kp;
    int Kd;

    double Setpoint;
    double Input;
    double Output;
};

// Initialize Gains
struct PID_Controls yaw;
struct PID_Controls roll;
struct PID_Controls pitch;
struct PID_Controls alt;

void PreProcessData(){
    yaw.ref = YawAverager.getRunningAverage(mpu.getYaw());
    pitch.ref = PitchAverager.getRunningAverage(mpu.getPitch());
    roll.ref = RollAverager.getRunningAverage(mpu.getRoll());
    alt.ref = compFilter(barometer.getAltitude(true), myGNSS.getAltitude()*(1000), 0.90);
}

void setupDecoupledPID(){
    PID yawPID(&(yaw.Setpoint), &(yaw.Output), &(yaw.Setpoint), yaw.Ki, yaw.Kp, yaw.Kd, DIRECT);
    PID rollPID(&(roll.Setpoint), &(roll.Output), &(roll.Setpoint), roll.Ki, roll.Kp, roll.Kd, DIRECT);
    PID pitchPID(&(pitch.Setpoint), &(pitch.Output), &(pitch.Setpoint), pitch.Ki, pitch.Kp, pitch.Kd, DIRECT);
    PID myPID(&(alt.Setpoint), &(alt.Output), &(alt.Setpoint), alt.Ki, alt.Kp, alt.Kd, DIRECT);
}

void setupCoupledPID(){
    // Perform body to world conversion

    // Introduce yaw reference point

    // Add two more PID Controllers for Roll and Pitch 
}

void Motor(){

};

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

void setup(){
    Serial.begin(115200);
    Wire.begin();
    delay(2000);
    while (!Serial);
    //Serial.println("SparkFun u-blox Example");
    do {
      Serial.println("GNSS: trying 38400 baud");
      mySerial.begin(38400);
      if (myGNSS.begin(mySerial) == true) break;
      delay(100);
      Serial.println("GNSS: trying 9600 baud");
      mySerial.begin(9600);
      if (myGNSS.begin(mySerial) == true) {
        Serial.println("GNSS: connected at 9600 baud, switching to 38400");
        myGNSS.setSerialRate(38400);
        delay(100);
      } else {
        delay(2000);
      }
    } while(1);
    Serial.println("GNSS serial connected");
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

void loop(){

    barometer.checkUpdates();
    
    if (mpu.update() && barometer.isReady()) {
        static uint32_t prev_ms = millis();
        temperature = barometer.GetTemp(); // Returns temperature in C
        pressure = barometer.GetPres(); // Returns pressure in Pascals
        if (millis() > prev_ms + 43) {
            PreProcessData();
            setupDecoupledPID();
        

            prev_ms = millis();
        }
    }  
}