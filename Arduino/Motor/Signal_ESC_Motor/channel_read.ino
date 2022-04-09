unsigned long timer[4]; //used in pin interrupt
byte last_channel[4];
int input[4];
int channel[4];

//#include <Servo.h>

//Servo ESC;     // create servo object to control the ESC

//int controllerValue;  // value from the analog pin

void setup() {
  PCICR |= (1<<PCIE0); //tells code there will be an interrupt
  PCMSK0 |= (1<<PCINT0); //address of pin interrupt
  PCMSK0 |= (1<<PCINT1);
  PCMSK0 |= (1<<PCINT2);
  PCMSK0 |= (1<<PCINT3);
  
  Serial.begin(9600);
  // Attach the ESC on pin 9
  //ESC.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
}


void loop() {
  //potValue = analogRead(A0);   // reads the value of the potentiometer (value between 0 and 1023)
  
  channel[0] = map(input[0], 1000, 2000, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  Serial.print(channel[0]);
  Serial.print('-');
  //ESC.write(controllerValue);    // Send the signal to the ESC

  channel[1] = map(input[1], 1000, 2000, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  Serial.print(channel[1]);
  Serial.print('-');
  //ESC.write(controllerValue);    // Send the signal to the ESC

  channel[2] = map(input[2], 1000, 2000, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  Serial.print(channel[2]);
  Serial.print('-');
  //ESC.write(controllerValue);    // Send the signal to the ESC

  channel[2] = map(input[3], 1000, 2000, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
  Serial.println(channel[3]);
  //ESC.write(controllerValue);    // Send the signal to the ESC
}


ISR(PCINT0_vect) { //code to run when interrupt occurs
  timer[0] = micros(); //timestamp when interrupt occurs

  //channel 1
  if(last_channel[0] == 0 && PINB & B00000001) { //last channel value is low and new channel value is high
    last_channel[0] = 1;
    timer[1] = timer [0];
    }
  else if(last_channel[0] == 1 && !(PINB & B00000001)) {
    last_channel[0] = 0;
    input[0] = timer[0] - timer[1];
  }

  //channel 2
  if(last_channel[1] == 0 && PINB & B00000010) { //last channel value is low and new channel value is high
    last_channel[1] = 1;
    timer[2] = timer [0];
    }
  else if(last_channel[1] == 1 && !(PINB & B00000010)) {
    last_channel[1] = 0;
    input[1] = timer[0] - timer[2];
  }

  //channel 3
  if(last_channel[2] == 0 && PINB & B00000100) { //last channel value is low and new channel value is high
    last_channel[2] = 1;
    timer[3] = timer [0];
    }
  else if(last_channel[2] == 1 && !(PINB & B00000100)) {
    last_channel[2] = 0;
    input[2] = timer[0] - timer[3];
  }

  //channel 4
  if(last_channel[3] == 0 && PINB & B00001000) { //last channel value is low and new channel value is high
    last_channel[3] = 1;
    timer[4] = timer [0];
    }
  else if(last_channel[3] == 1 && !(PINB & B00001000)) {
    last_channel[3] = 0;
    input[3] = timer[0] - timer[4];
  }
}
