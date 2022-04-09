#include <Servo.h>
double channel;

Servo ESC;     // create servo object to control the ESC

int controllerValue;  // value from the analog pin

void setup() {
  pinMode(3, INPUT);
  Serial.begin(9600);
  // Attach the ESC on pin 9
  ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
}

void loop() {
  channel = pulseIn(3, HIGH);
  //potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  controllerValue = map(channel, 1000, 2000, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  
  Serial.println(controllerValue);
  ESC.write(controllerValue);    // Send the signal to the ESC
}
