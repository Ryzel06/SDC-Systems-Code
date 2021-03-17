
/*
  controller-intepreter -- Andrew R Gross -- 2017-11-23 -- andrew@shrad.org

  This sketch uses values sent over a serial connection to control servos or other motor controllers.
  It reads a string from serial; parses it into a list of integers; scales the inegers to the correct range
  for the motor controller, and then outputs the signal to control a motor.

*/
////////////////////////////////////
/// 1: Header

#include <Servo.h>       // Import servo libraries
#include <Feedback360Servo.h>
#include <PID_v1.h>
int left_motor = 1;
Servo esc_left;          // Create a servo object named esc_left
Servo esc_right;         // Create another servo object for the right esc

String inString = " ";    // define a string to hold received serial characters
int inChar = 0;          // define a variable to hold the latest character from serial
//int thruster_left;       // define a variable to hold the latest signal to the left thruster
//int thruster_right;      // define a variable to hold the latest signal to the right thruster
int save_as_second = false;// define a logical element that tracks if values are part of the second value in the string

#define Feedback360Servo Lmotor();
#define Feedback360Servo Rmotor(); 

////////////////////////////////////
/// 2: Setup

void setup() {
  Serial.begin(2400);    // Open a serial channel to Arduino at a baudrate of 9600
//  Lmotor.attach(2,3);    // Assign a servo object to pin 9
 // Rmotor.attach(4,5);  // Assign a servo to pin 10
//  Lmotor.begin;
  //Rmotor.begin;
 // Lmotor.write(0);        //Resets Left motor to absolute position 0 degrees via Arduino
 // Rmotor.write(0);        //Resets Right motor to absolute position 0 degrees via Arduino
}

////////////////////////////////////
/// 3: Main loop

void loop() {

//bool setSpeed(int16_t speed_rpm);

//int forward = setSpeed(120);

  /// 3.1 : Read serial data
if (Serial.available()) {  // If serial data is available...
    inChar = Serial.read();  // Save the latest byte
    inString += char(inChar);// Join the latest character to a growing string of recent chracters
  
    if(inChar = "Hello!"){
      //Lmotor.setSpeed(120)
      
      Serial.print("left motor");
      
      //break;
      //inString = "";
   
    }

  }
    /// 3.2 When a comma is detected, save the current string as a number and assign it to one of the output variables
    if (inChar == ',') {
      if (save_as_second == false) { // If the most recent chracter is a comma, confirm that the current string is the first value.
      // thruster_left = inString.toInt(); // Save the string as an integer in one of our two output variables
      //  inString = "";               // Reset the string
        save_as_second == true;      // Indicate that the next value detected is for the second thruster
      }
    }
    /// 3.3 When a semicolon is detected, save the current string as a number and assign it to the other output variables
    if (inChar == ';') {     // If the latest chracter is the end-of-line character...
     //  thruster_right = inString.toInt();// Assign the most recent number string to the second thruster
     //  inString = "";        // Reset the string
      save_as_second = false;// Indicate that the next value detected is for the first thruster

      /// 3.4 Write out the new values to the servos and to serial
      //Serial.print("Left Thruster:"); // Print the values of each thruster to serial
      //Serial.println(thruster_left);
      //Serial.print("Right string: ");
      //Serial.println(thruster_right);
      
      //esc_left.write(thruster_left);  // Write the signal value to one of the servo objects
      //esc_right.write(thruster_right);
    }
  }