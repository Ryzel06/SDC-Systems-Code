/*
 * This is the Arduino code for push button to push ON and Push OFF a relay and AC bulb
it toggles the bulb to ON and OFF
 * the output pen 10 is connected to relay 
 * watch video instruction on video https://youtu.be/7CCSRs5bvH0
 * Be carefull working with AC is dangrous. Disconnect from AC power when working and wear protective gloves when touch the AC components.
 
 * Full explanation of this code and wiring diagram is available at
 * my Arduino Course at Udemy.com here: http://robojax.com/L/?id=62

 * Written by Ahmad Shamshiri on Dec 14, 2017 in Ajax, Ontario, Canada
 * in Ajax, Ontario, Canada. www.robojax.com
 * 

 * Get this code and other Arduino codes from Robojax.com
Learn Arduino step by step in structured course with all material, wiring diagram and library
all in once place. Purchase My course on Udemy.com http://robojax.com/L/?id=62

If you found this tutorial helpful, please support me so I can continue creating 
content like this. You can support me on Patreon http://robojax.com/L/?id=63

or make donation using PayPal http://robojax.com/L/?id=64

 *  * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


*/


char receivedButton;// connect output to push button
int relayPin = 10;// Connected to relay (LED)

int val = 0; // push value from pin 2
int lightON = 0;//light status
int pushed = 0;//push status


void setup() {
  // Robojax.com code and video tutorial for push button ON and OFF//
  //Using serial available to obtain value from pi serial comms
  Serial.begin(9600);
  pinMode(receivedButton, INPUT_PULLUP); 
  pinMode(relayPin, OUTPUT);
 digitalWrite(relayPin, HIGH);// keep the load OFF at the begining. If you wanted to be ON, change the HIGH to LOW
}

void loop() {
// Robojax.com code and video tutorial for push button ON and OFF
 while(Serial.available()) {
  String commands = Serial.readString(); // read the incoming data as string
  //Serial.println("I read string: "+ commands); // feedback of data received
  receivedButton = commands.charAt(0); // convert to char to enable Switch Statement use
  val = receivedButton;
 }
  if(val == HIGH && lightON == LOW){

    pushed = 1-pushed;
    delay(100);
  }    

  lightON = val;

      if(pushed == HIGH){
        Serial.println("Vac Pump Power ON");
        digitalWrite(relayPin, LOW); 
       
      }else{
        Serial.println("Vac Pump Power OFF");
        digitalWrite(relayPin, HIGH);
   
      }     

// Robojax.com code and video tutorial for push button ON and OFF

  delay(100);
}
