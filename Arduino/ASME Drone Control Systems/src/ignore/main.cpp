#include <Arduino.h>
#include "Preprocessing.h"

#define PIN 13
#define DELAY 1000

//   

void setup() {
  pinMode(PIN, OUTPUT);
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  digitalWrite(PIN, HIGH);
  Serial.println("HIGH");
  delay(DELAY);

  digitalWrite(PIN, LOW);
  Serial.println("LOW");
  delay(DELAY);

}



/*

          Sensor Preprocessing

*/

