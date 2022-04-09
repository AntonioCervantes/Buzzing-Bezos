#include <Servo.h>

// ESC Variables
Servo ESC1;
Servo ESC2;
Servo ESC3;
Servo ESC4;

// Motor Mixing Variables 
int motor1; // Front/Right
int motor2; // Front/Left
int motor3; // Back/Right
int motor4; // Back/Left

// Motor -> ESC Variables
int m1;
int m2;
int m3;
int m4;

// Control Inputs
int throttle;
int roll;
int pitch;
int yaw;


void setup() {
    Serial.begin(9600);

    // Remote Control Reciever Input Pins
    pinMode(3, INPUT); // Channel 1
    pinMode(4, INPUT); // Channel 2
    pinMode(5, INPUT); // Channel 3
    pinMode(6, INPUT); // Channel 4
    
    // ESC Signal Pins (pin, min pulse width, max pulse width in microseconds)
    ESC1.attach(9,1000,2000);  // ESC1 for Motor 1
    ESC2.attach(10,1000,2000); // ESC2 for Motor 2
    ESC3.attach(11,1000,2000); // ESC3 for Motor 3
    ESC4.attach(12,1000,2000); // ESC4 for Motor 4
}


void loop() {
    //throttle = pulseIn(3, HIGH);
    //roll = pulseIn(3, HIGH);


    // Motor Mixing Algorithm  TODO: Add PID inputs to this
    motor1 = throttle + roll + pitch + yaw;
    motor2 = throttle - roll + pitch - yaw;
    motor3 = throttle - roll - pitch + yaw;
    motor4 = throttle + roll - pitch - yaw;

    // Scale values to write to ESC
    m1 = map(motor1, 1000, 2000, 0, 180);
    m2 = map(motor2, 1000, 2000, 0, 180);
    m3 = map(motor3, 1000, 2000, 0, 180);
    m4 = map(motor4, 1000, 2000, 0, 180);

    // Write values to ESC
    ESC1.write(m1);
    ESC2.write(m2);
    ESC3.write(m3);
    ESC4.write(m4);

    // Print Motor Values
    //Serial.println(m1, m2, m3, m4);
}
