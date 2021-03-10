// ---------------------------------------------------------------------------
// Example NewPing library sketch that does a ping about 20 times per second.
// ---------------------------------------------------------------------------

// include the library code
#include <Wire.h>
#include "Feedback360Servo.h"
#include <NewPing.h>
#include <Arduino.h>



#define TRIGGER_PIN1  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN1     3  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIGGER_PIN1  4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN1     5

#define TRIGGER_PIN1  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN1     7

Feedback360Servo Lmotor();
Feedback360Servo Rmotor();

#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.



NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);// NewPing setup of pins and maximum distance.
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE);

void setup() {

  const int proxValue = 30;
  
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.

Lmotor.begin;
Rmotor.begin;
}

void loop() {

  
  
  delay(1000);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS1 = sonar1.ping(); // Send ping, get ping time in microseconds (uS).
  unsigned int cm1 = uS1 / US_ROUNDTRIP_CM;
  Serial.print("Sensor 1: ");
  Serial.print (cm1) ; // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

   delay(1000);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS2 = sonar2.ping(); // Send ping, get ping time in microseconds (uS).
  unsigned int cm2 = uS2 / US_ROUNDTRIP_CM;
  Serial.print("Sensor 2: ");
  Serial.print (cm2) ; // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

   delay(1000);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS3 = sonar3.ping(); // Send ping, get ping time in microseconds (uS).
  unsigned int cm3 = uS3 / US_ROUNDTRIP_CM;
  Serial.print("Sensor 3: ");
  Serial.print (cm3) ; // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

   bool setSpeed(int16_t speed_rpm);


   while (cm1 < proxValue) {
    Lmotor.setSpeed(-120);
    Rmotor.setSpeed(-120);
    
   } 
   Lmotor.setSpeed(0);
   Rmotor.setSpeed(0);

  while (cm2 < proxValue) {
    Lmotor.setSpeed(-120);
    Rmotor.setSpeed(-120);
   }

  Lmotor.setSpeed(0);
  Rmotor.setSpeed(0);

  while (cm3 < proxValue) {
    Lmotor.setSpeed(120);
    Rmotor.setSpeed(-120);
   }


}