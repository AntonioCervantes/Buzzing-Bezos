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
int throttle2;
int roll;
int roll2;
int pitch;
int pitch2;
int yaw;
int yaw2;
int type;
int eStop;

byte last_channel_1, last_channel_2, last_channel_3, last_channel_4, last_channel_5, last_channel_6;
unsigned long timer_roll, timer_channel_2, timer_channel_3, timer_channel_4, timer_channel_5, timer_channel_6;
unsigned long timer_1, timer_2, timer_3, timer_4, timer_5, timer_6, current_time;


void setup ()
{
  // ESC Signal Pins (pin, min pulse width, max pulse width in microseconds)
  ESC1.attach(9,1000,2000);  // ESC1 for Motor 1
  ESC2.attach(10,1000,2000); // ESC2 for Motor 2
  ESC3.attach(11,1000,2000); // ESC3 for Motor 3
  ESC4.attach(12,1000,2000); // ESC4 for Motor 4

  // Test LEDs
  //pinMode(9, OUTPUT);
  //pinMode(10, OUTPUT);
  //pinMode(11, OUTPUT);
  //pinMode(12, OUTPUT);
  
  //Set PCIE0 to enable PCMSK2 scan.
  PCICR |= (1 << PCIE2);
  
  // Reciever Pins
  PCMSK2 |= (1 << PCINT16); //Set PCINT16 (Analog input A8) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT17); //Set PCINT17 (Analog input A9) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT18); //Set PCINT18 (Analog input A10) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT19); //Set PCINT19 (Analog input A11) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT20); //Set PCINT20 (Analog input A12) to trigger an interrupt on state change.
  PCMSK2 |= (1 << PCINT21); //Set PCINT20 (Analog input A13) to trigger an interrupt on state change.

  Serial.begin(9600);
}

void loop ()
{

  

  // Motor Mixing Algorithm  TODO: Add PID inputs to this
  if(eStop < 1500) {
    motor1 = throttle + yaw + pitch + roll-270;
    motor2 = throttle - yaw + pitch - roll+90+22;
    motor3 = throttle - yaw - pitch + roll+90+22;
    motor4 = throttle + yaw - pitch - roll+90+22;
  }
  else {
    motor1=motor2=motor3=motor4=0;
  }
  // Scale values to write to ESC
  //m1 = map(motor1, 0, 2000, 0, 180);
  //m2 = map(motor2, 0, 2000, 0, 180);
  //m3 = map(motor3, 0, 2000, 0, 180);
  //m4 = map(motor4, 0, 2000, 0, 180);

  // Write values to ESC
  ESC1.write(motor1);
  ESC2.write(motor2);
  ESC3.write(motor3);
  ESC4.write(motor4);
  //ESC1.write(throttle);
  //ESC2.write(throttle);
  //ESC3.write(throttle);
  //ESC4.write(throttle);

  //Test LEDs
  //analogWrite(9, m1);
  //analogWrite(10, m2);
  //analogWrite(11, m3);
  //analogWrite(12, m4);

  // debug

  /*/ Debug reciever inputs
  Serial.print("roll = ");
  Serial.print(roll);
  Serial.print(" - ");
  Serial.print("pitch = ");
  Serial.print(pitch);
  Serial.print(" - ");
  Serial.print("throttle = ");
  Serial.print(throttle);
  Serial.print(" - ");
  Serial.print("yaw = ");
  Serial.print(yaw);
  Serial.print(" - ");
  Serial.print("type = ");
  Serial.print(type);
  Serial.print(" - ");
  Serial.print("eStop = ");
  Serial.println(eStop);*/
  
  // debug motor mixing
  Serial.print("Motor 1 = ");
  Serial.print(motor1);
  Serial.print(" - ");
  Serial.print("Motor 2 = ");
  Serial.print(motor2);
  Serial.print(" - ");
  Serial.print("Motor 3 = ");
  Serial.print(motor3);
  Serial.print(" - ");
  Serial.print("Motor 4 = ");
  Serial.println(motor4);
  
  
  /*/ debug motor pwm signal
  Serial.print("Motor 1 = ");
  Serial.print(m1);
  Serial.print(" - ");
  Serial.print("Motor 2 = ");
  Serial.print(m2);
  Serial.print(" - ");
  Serial.print("Motor 3 = ");
  Serial.print(m3);
  Serial.print(" - ");
  Serial.print("Motor 4 = ");
  Serial.println(m4);*/
 

}


ISR(PCINT2_vect)
{
  current_time = micros();
  //Channel 1=========================================
  if(PINK & B00000001)
  {                                        //Is input 8 high?
  if(last_channel_1 == 0)
  {                                   //Input 8 changed from 0 to 1
    last_channel_1 = 1;                                      //Remember current input state
    timer_1 = current_time;                                  //Set timer_1 to current_time
  }
  }
  else if(last_channel_1 == 1)
  {                                //Input 8 is not high and changed from 1 to 0
    last_channel_1 = 0;                                        //Remember current input state
    roll2 = current_time - timer_1;         //Channel 1 is current_time - timer_1
    if (roll2 > 1475 && roll2 < 1525) {
      roll = 90;
    }
    else {
      roll = map(roll2, 1000, 2000, 0, 180);
    }
    
  }
  //Channel 2=========================================
  if(PINK & B00000010 )
  {                                       //Is input 9 high?
    if(last_channel_2 == 0)
    {                                   //Input 9 changed from 0 to 1
      last_channel_2 = 1;                                      //Remember current input state
      timer_2 = current_time;                                  //Set timer_2 to current_time
    }
  }
  else if(last_channel_2 == 1)
  {                                //Input 9 is not high and changed from 1 to 0
    last_channel_2 = 0;                                        //Remember current input state
    pitch2 = current_time - timer_2;         //Channel 2 is current_time - timer_2
    if (pitch2 > 1475 && pitch2 < 1525) {
      pitch = 90;
    }
    else {
      pitch = map(pitch2, 1000, 2000, 0, 180);
    }
    
  }
  //Channel 3=========================================
  if(PINK & B00000100 )
  {                                       //Is input 10 high?
    if(last_channel_3 == 0)
    {                                   //Input 10 changed from 0 to 1
      last_channel_3 = 1;                                      //Remember current input state
      timer_3 = current_time;                                  //Set timer_3 to current_time
    }
  }
  else if(last_channel_3 == 1)
  {                                //Input 10 is not high and changed from 1 to 0
    last_channel_3 = 0;                                        //Remember current input state
    throttle2 = current_time - timer_3;         //Channel 3 is current_time - timer_3
    if (throttle2 > 1050) {
      throttle = map(throttle2, 1000, 2000, 0, 180);
    }
    else {
      throttle = 0;
    }
    }
  
  //Channel 4=========================================
  if(PINK & B00001000 )
  {                                       //Is input 11 high?
    if(last_channel_4 == 0)
    {                                   //Input 11 changed from 0 to 1
      last_channel_4 = 1;                                      //Remember current input state
      timer_4 = current_time;                                  //Set timer_4 to current_time
    }
  }
  else if(last_channel_4 == 1)
  {                                //Input 11 is not high and changed from 1 to 0
    last_channel_4 = 0;                                        //Remember current input state
    yaw2 = current_time - timer_4;         //Channel 4 is current_time - timer_4
    if (yaw2 > 1475 && yaw2 < 1525) {
      yaw = 90;
    }
    else {
      yaw = map(yaw2, 1000, 2000, 0, 180);
    }
    
  }
  //Channel 5=========================================
  if(PINK & B00010000)
  {
    if(last_channel_5 == 0)
    {
      last_channel_5 = 1;
      timer_5 = current_time;        
    }    
  }
  else if(last_channel_5 == 1)
  {
      last_channel_5 = 0;
      type = current_time - timer_5;
  }

  //Channel 6=========================================
  if(PINK & B00100000)
  {
    if(last_channel_6 == 0)
    {
      last_channel_6 = 1;
      timer_6 = current_time;        
    }    
  }
  else if(last_channel_6 == 1)
  {
      last_channel_6 = 0;
      eStop = current_time - timer_6;
  }
 
}
