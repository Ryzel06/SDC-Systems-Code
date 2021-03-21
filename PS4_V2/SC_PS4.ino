#include "Feedback360Servo.h"
#include <Servo.h>
#include <PID_v1.h>
#include <AccelStepper.h>

using namespace feedback_servo;
 uint8_t begin(int attach_pin , int feedback_pin);
char receivedPos;

#define FULLSTEP 4
#define motorPin1 4
#define motorPin2 5
#define motorPin3 6
#define motorPin4 7


#define motorPin5 8
#define motorPin6 9
#define motorPin7 10
#define motorPin8 11



#define attach_pin1 2
#define feedback_pin1 3
#define attach_pin2 4
#define feedback_pin2 5

Feedback360Servo Lmotor;
Feedback360Servo Rmotor;

AccelStepper stepper1(FULLSTEP, motorPin1,motorPin2,motorPin3,motorPin4);
AccelStepper stepper2(FULLSTEP, motorPin5,motorPin6,motorPin7,motorPin8);


char receivedButton;// connect output to push button   /RELAYYYYYYYYYYYYYYYY
int relayPin = 10;// Connected to relay (LED)
int val = 0; // push value from pin 2
int lightON = 0;//light status
int pushed = 0;//push status

void setup() {

//int  Lmotor = Feedback360Servo1;
 Serial.begin(9600);
// myservo.attach(9); // attaches the servo on pin 9 to the servo object
// myservo.write(90); // on power on, move servo to central position
 Serial.println("Welcome!"); // banner on connection with instructions
 Serial.println("Use following keys to control Feedback 360 Servo Motor");
 Serial.println("Left Joy Up: moves to absolute position 180 dregrees");
 Serial.println("Left Joy Down: moves to absolute position 90 dregrees");
 Serial.println("Right Joy Up: moves to absolute position 0 dregrees");
 Serial.println("Right Joy Down: moves to absolute position 0 dregrees");
 Serial.println("Trianlge Button: moves to absolute position 0 dregrees");
 Serial.println("Circle Button: moves to absolute position 0 dregrees");
 Serial.println("D-Pad Up: moves to absolute position 0 dregrees");
 Serial.println("D-Pad Down: moves to absolute position 0 dregrees");
 Serial.println("D-Pad Left: moves to absolute position 0 dregrees");
 Serial.println("D-Pad Right: no moves");

 uint8_t begin(int attach_pin , int feedback_pin);
   bool setSpeed(int16_t speed_rpm);
 stepper1.setMaxSpeed(1000.0);
 stepper1.setAcceleration(75.0);
 stepper1.setSpeed(200);
 stepper1.moveTo(2048);

 stepper2.setMaxSpeed(1000.0);
 stepper2.setAcceleration(75.0);
 stepper2.setSpeed(200);
 stepper2.moveTo(2048);

 pinMode(receivedButton, INPUT_PULLUP);  //RELAYYYYYYYYYYYYY
 pinMode(relayPin, OUTPUT);
 digitalWrite(relayPin, HIGH);// keep the load OFF at the begining. If you wanted to be ON, change the HIGH to LOW
}

void loop() {

  bool setSpeed(int16_t speed_rpm); 
  
   
 while(Serial.available()) {
  String commands = Serial.readString(); // read the incoming data as string
  Serial.println("Read String: "+ commands); // feedback of data received
  receivedPos = commands.charAt(0); // convert to char to enable Switch Statement use
  receivedButton = commands.charAt(0); // convert to char to enable Switch Statement use
  val = receivedButton;
 } switch (receivedPos) {
   
  
 case 'A':  Lmotor.setSpeed(120); break;   //left motor moving forward
 case 'B':  Rmotor.setSpeed(-120); break;  //right motor moving forward
 case 'C':  Lmotor.setSpeed(-120); break;  //left motor moving backward
 case 'D':  Rmotor.setSpeed(120); break;   //right motor moving backward
//   
//   

 case 'HIGH': if(val == HIGH && lightON == LOW){ 
              pushed = 1-pushed;
              delay(100);
               }    lightON = val;
                    if(pushed == HIGH){
                    Serial.println("Vac Pump Power ON");
                    digitalWrite(relayPin, LOW); 
                    }else{
                      Serial.println("Vac Pump Power OFF");
                      digitalWrite(relayPin, HIGH);
                    }
              delay(100); break; //circle - Turn on/off Relay w/ Vacuum Pump

 
  
 //case 'F': myservo.write(0); break;   //d-pad up - Ascend Transducer
 //case 'G': myservo.write(0); break;   //d-pad down - Descends Transducer

  //case 'H': myservo.write(0); break;   //d-pad left - Ascend Gel deployment system
 //case 'I': myservo.write(0); break;   //d-pad right - Descends Gel deployment system

   default: break;
}
}
 
