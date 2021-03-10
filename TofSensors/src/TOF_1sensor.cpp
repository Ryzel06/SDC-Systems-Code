 
 #include <Adafruit_VL6180X.h>


// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32
#define LOX4_ADDRESS 0x33 
#define LOX5_ADDRESS 0x34 
#define LOX6_ADDRESS 0x35 

// set the pins to shutdown
#define SHT_LOX1 3
#define SHT_LOX2 5
#define SHT_LOX3 7
#define SHT_LOX4 9
#define SHT_LOX5 11
#define SHT_LOX6 13


// Optional define GPIO pins to check to see if complete
#define GPIO_LOX1 2
#define GPIO_LOX2 4
#define GPIO_LOX3 6
#define GPIO_LOX4 8
#define GPIO_LOX5 10
#define GPIO_LOX6 12


#define TIMING_PIN 14

// objects for the VL6180X
Adafruit_VL6180X lox1 = Adafruit_VL6180X();
Adafruit_VL6180X lox2 = Adafruit_VL6180X();
Adafruit_VL6180X lox3 = Adafruit_VL6180X();
Adafruit_VL6180X lox4 = Adafruit_VL6180X();
Adafruit_VL6180X lox5 = Adafruit_VL6180X();
Adafruit_VL6180X lox6 = Adafruit_VL6180X();


// Setup mode for doing reads
typedef enum {RUN_MODE_DEFAULT} runmode_t;

runmode_t run_mode = RUN_MODE_DEFAULT;
uint8_t show_command_list = 1;

//==========================================================================
// Define some globals used in the continuous range mode
// Note: going to start table drive this part, may back up and do the rest later
Adafruit_VL6180X *sensors[] = {&lox1,&lox2,&lox3,&lox4,&lox5,&lox6};
const uint8_t COUNT_SENSORS = sizeof(sensors) / sizeof(sensors[0]);
const int sensor_gpios[COUNT_SENSORS] = {GPIO_LOX1, GPIO_LOX2, GPIO_LOX3, GPIO_LOX4, GPIO_LOX5, GPIO_LOX6}; // if any are < 0 will poll instead

uint8_t         sensor_ranges[COUNT_SENSORS];
uint8_t         sensor_status[COUNT_SENSORS];
// Could do with uint8_t for 8 sensors, but just in case...
const uint16_t  ALL_SENSORS_PENDING = ((1 << COUNT_SENSORS) - 1);
uint16_t        sensors_pending = ALL_SENSORS_PENDING;
uint32_t        sensor_last_cycle_time;


/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
*/
void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);
  digitalWrite(SHT_LOX6, LOW);
  delay(10);

  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  digitalWrite(SHT_LOX5, HIGH);
  digitalWrite(SHT_LOX6, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);
  digitalWrite(SHT_LOX6, LOW);

  // initing LOX1
 
 // initing LOX1
  if (!lox1.begin()) {
    Serial.println(F("Failed to boot first VL6180X"));
    while (1);
  }
  lox1.setAddress(LOX1_ADDRESS);
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin()) {
    Serial.println(F("Failed to boot second VL6180X"));
    while (1);
  }
  lox2.setAddress(LOX2_ADDRESS);

  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX3
  if (!lox3.begin()) {
    Serial.println(F("Failed to boot third VL6180X"));
    while (1);
  }
  lox3.setAddress(LOX3_ADDRESS);
/////////////
  // activating LOX4
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  //initing LOX4
  if (!lox4.begin()) {
    Serial.println(F("Failed to boot fourth VL6180X"));
    while (1);
  }
  lox4.setAddress(LOX4_ADDRESS);

 digitalWrite(SHT_LOX5, HIGH);
  delay(10);

if (!lox5.begin()) {
    Serial.println(F("Failed to boot fourth VL6180X"));
    while (1);
  }
  lox5.setAddress(LOX5_ADDRESS);

   digitalWrite(SHT_LOX6, HIGH);
  delay(10);

if (!lox6.begin()) {
    Serial.println(F("Failed to boot fourth VL6180X"));
    while (1);
  }
  lox6.setAddress(LOX6_ADDRESS);
  
}








void readSensor(Adafruit_VL6180X &vl) {
  Serial.print(" Addr: ");
  Serial.print(vl.getAddress(), HEX);


  uint8_t range1 = vl.readRange();
  uint8_t status = vl.readRangeStatus();

  if (status == VL6180X_ERROR_NONE) {
    Serial.print(" Range: "); Serial.print(range1);

  
   
  }

  // Some error occurred, print it out!

  if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.print("(System error)");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.print("(ECE failure)");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.print("(Off Surface)");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.print("(Ignoring range)");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.print("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.print("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.print("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.print("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.print("Range reading overflow");
  }
}

void read_sensors() {
  readSensor(lox1);
  readSensor(lox2);
  readSensor(lox3);
  readSensor(lox4);
  readSensor(lox5);
  readSensor(lox6);
  Serial.println();
}

void timed_read_sensors() {
  digitalWrite(TIMING_PIN, HIGH);
  uint32_t start_time = millis();
  uint8_t range_lox1 = lox1.readRange();
  uint8_t status_lox1 = lox1.readRangeStatus();
  uint8_t range_lox2 = lox2.readRange();
  uint8_t status_lox2 = lox2.readRangeStatus();
  uint8_t range_lox3 = lox3.readRange();
  uint8_t status_lox3 = lox3.readRangeStatus();
  uint8_t range_lox4 = lox4.readRange();
  uint8_t status_lox4 = lox4.readRangeStatus();
  uint8_t range_lox5 = lox5.readRange();
  uint8_t status_lox5 = lox5.readRangeStatus();
  uint8_t range_lox6 = lox6.readRange();
  uint8_t status_lox6 = lox6.readRangeStatus();
  uint32_t delta_time = millis() - start_time;
  digitalWrite(TIMING_PIN, LOW);
  Serial.print(delta_time, DEC);
  Serial.print(" : ");
  if (status_lox1 == VL6180X_ERROR_NONE) Serial.print(range_lox1, DEC);
  else Serial.print("###");
  Serial.print(" : ");
  if (status_lox2 == VL6180X_ERROR_NONE) Serial.print(range_lox2, DEC);
  else Serial.print("###");
  Serial.print(" : ");
  if (status_lox3 == VL6180X_ERROR_NONE) Serial.print(range_lox3, DEC);
  else Serial.print("###");
  if (status_lox4 == VL6180X_ERROR_NONE) Serial.print(range_lox4, DEC);
  else Serial.print("###");
  if (status_lox5 == VL6180X_ERROR_NONE) Serial.print(range_lox5, DEC);
  else Serial.print("###"); 
  if (status_lox6 == VL6180X_ERROR_NONE) Serial.print(range_lox6, DEC);
  else if ( status_lox6 == VL6180X_ERROR_NOCONVERGE)
    Serial.print("###");

  

  Serial.println();
}

void timed_async_read_sensors() {
  digitalWrite(TIMING_PIN, HIGH);
  uint32_t start_time = millis();

  // lets start up all six
  lox1.startRange();
  lox2.startRange();
  lox3.startRange();
  lox4.startRange();
  lox5.startRange();
  lox6.startRange();



  // wait for each of them to complete
  lox1.waitRangeComplete();
  lox2.waitRangeComplete();
  lox3.waitRangeComplete();
  lox4.waitRangeComplete();
  lox5.waitRangeComplete();
  lox6.waitRangeComplete();


  uint8_t range_lox1 = lox1.readRangeResult();
  uint8_t status_lox1 = lox1.readRangeStatus();
  uint8_t range_lox2 = lox2.readRangeResult();
  uint8_t status_lox2 = lox2.readRangeStatus();
  uint8_t range_lox3 = lox3.readRangeResult();
  uint8_t status_lox3 = lox3.readRangeStatus();
  uint8_t range_lox4 = lox4.readRangeResult();
  uint8_t status_lox4 = lox4.readRangeStatus();
  uint8_t range_lox5 = lox5.readRangeResult();
  uint8_t status_lox5 = lox5.readRangeStatus();
  uint8_t range_lox6 = lox6.readRangeResult();
  uint8_t status_lox6 = lox6.readRangeStatus();


  uint32_t delta_time = millis() - start_time;
  digitalWrite(TIMING_PIN, LOW);
  Serial.print(delta_time, DEC);
  Serial.print(" : ");
  if (status_lox1 == VL6180X_ERROR_NONE) Serial.print(range_lox1, DEC);
  else Serial.print("###");
  Serial.print(" : ");
   if (status_lox2 == VL6180X_ERROR_NONE) Serial.print(range_lox2, DEC);
  else Serial.print("###");
  Serial.print(" : ");
   if (status_lox3 == VL6180X_ERROR_NONE) Serial.print(range_lox3, DEC);
  else Serial.print("###");
  Serial.print(" : ");
   if (status_lox4 == VL6180X_ERROR_NONE) Serial.print(range_lox4, DEC);
  else Serial.print("###");
  Serial.print(" : ");
   if (status_lox5 == VL6180X_ERROR_NONE) Serial.print(range_lox5, DEC);
  else Serial.print("###");
  Serial.print(" : ");
   if (status_lox6 == VL6180X_ERROR_NONE) Serial.print(range_lox6, DEC);
  else Serial.print("###");
  Serial.print(" : ");


  Serial.println();

}

void timed_async_read_gpio() {
#ifdef GPIO_LOX1
  uint8_t range_lox1 = 0;
  uint8_t status_lox1 = VL6180X_ERROR_NONE;

  uint8_t range_lox2 = 0;
  uint8_t status_lox2 = VL6180X_ERROR_NONE;

   uint8_t range_lox3 = 0;
  uint8_t status_lox3 = VL6180X_ERROR_NONE;

   uint8_t range_lox4 = 0;
  uint8_t status_lox4 = VL6180X_ERROR_NONE;

   uint8_t range_lox5 = 0;
  uint8_t status_lox5 = VL6180X_ERROR_NONE;

   uint8_t range_lox6 = 0;
  uint8_t status_lox6 = VL6180X_ERROR_NONE;
  
uint8_t tof [6]={ status_lox1, status_lox2 , status_lox3 ,status_lox4 ,status_lox5, status_lox6};

while ( tof != VL6180X_ERROR_NONE) {
   if( tof == VL6180X_ERROR_NOCONVERGE) {
    for ( i = 0; i < 6; i++ ) {
    
   }  
   }
   
   
  
}


  digitalWrite(TIMING_PIN, HIGH);
  uint8_t pending_sensors = 0x7;
  uint32_t start_time = millis();

  // lets start up all three
  lox1.startRange();
  lox2.startRange();
  lox3.startRange();
  lox4.startRange();
  lox5.startRange();
  lox6.startRange();


  while (pending_sensors && ((millis() - start_time) < 1000)) {
    if ((pending_sensors & 0x1) && !digitalRead(GPIO_LOX1)) {
      range_lox1 = lox1.readRangeResult();
      status_lox1 = lox1.readRangeStatus();
      pending_sensors ^= 0x1;
    }

 if ((pending_sensors & 0x2) && !digitalRead(GPIO_LOX2)) {
      range_lox2 = lox2.readRangeResult();
      status_lox2 = lox2.readRangeStatus();
      pending_sensors ^= 0x2;
    }

     if ((pending_sensors & 0x3) && !digitalRead(GPIO_LOX3)) {
      range_lox3 = lox3.readRangeResult();
      status_lox3 = lox3.readRangeStatus();
      pending_sensors ^= 0x3;
    }

     if ((pending_sensors & 0x4) && !digitalRead(GPIO_LOX4)) {
      range_lox4 = lox4.readRangeResult();
      status_lox4 = lox4.readRangeStatus();
      pending_sensors ^= 0x4;
    }

     if ((pending_sensors & 0x5) && !digitalRead(GPIO_LOX5)) {
      range_lox5 = lox5.readRangeResult();
      status_lox5 = lox5.readRangeStatus();
      pending_sensors ^= 0x5;
    }

     if ((pending_sensors & 0x6) && !digitalRead(GPIO_LOX6)) {
      range_lox6 = lox6.readRangeResult();
      status_lox6 = lox6.readRangeStatus();
      pending_sensors ^= 0x6;
    }

  
  }
  uint32_t delta_time = millis() - start_time;
  digitalWrite(TIMING_PIN, LOW);
  Serial.print(delta_time, DEC);
  Serial.print("(");
  Serial.print(pending_sensors, DEC);
  Serial.print(") : ");
  if (status_lox1 == VL6180X_ERROR_NONE) Serial.print(range_lox1, DEC);
  else Serial.print("###");
  Serial.print(" : ");
    if (status_lox2 == VL6180X_ERROR_NONE) Serial.print(range_lox2, DEC);
  else Serial.print("###");
  Serial.print(" : ");
    if (status_lox3 == VL6180X_ERROR_NONE) Serial.print(range_lox3, DEC);
  else Serial.print("###");
  Serial.print(" : ");
    if (status_lox4 == VL6180X_ERROR_NONE) Serial.print(range_lox4, DEC);
  else Serial.print("###");
  Serial.print(" : ");  
  if (status_lox5 == VL6180X_ERROR_NONE) Serial.print(range_lox5, DEC);
  else Serial.print("###");
  Serial.print(" : ");
    if (status_lox6 == VL6180X_ERROR_NONE) Serial.print(range_lox6, DEC);
  else Serial.print("###");
  Serial.print(" : ");

  Serial.println();

  
#endif
}






//===============================================================
// Setup
//===============================================================
void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  pinMode(SHT_LOX4, OUTPUT);
  pinMode(SHT_LOX5, OUTPUT);
  pinMode(SHT_LOX6, OUTPUT);

  // Enable timing pin so easy to see when pass starts and ends
  pinMode(TIMING_PIN, OUTPUT);

#ifdef GPIO_LOX1
  // If we defined GPIO pins, enable them as PULL UP
  pinMode(GPIO_LOX1, INPUT_PULLUP);
  pinMode(GPIO_LOX2, INPUT_PULLUP);
  pinMode(GPIO_LOX3, INPUT_PULLUP);
  pinMode(GPIO_LOX4, INPUT_PULLUP);
  pinMode(GPIO_LOX5, INPUT_PULLUP);
  pinMode(GPIO_LOX6, INPUT_PULLUP);

#endif

  Serial.println("Shutdown pins inited...");

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  digitalWrite(SHT_LOX5, LOW);
  digitalWrite(SHT_LOX6, LOW);

  digitalWrite(TIMING_PIN, LOW);
  Serial.println("All in reset mode...(pins are low)");


  Serial.println("Starting...");
  setID();

}

//===============================================================
// Loop
//===============================================================
void loop() {
  

  uint8_t range_lox1 = lox1.readRange();
  
  
  if (Serial.available()) {
    uint8_t ch = Serial.read();
    while (Serial.read() != -1) ; // remove the rest
    runmode_t prev_run_mode = run_mode;
    // See what the user typed in
    switch (ch) {
      case 'd':
      case 'D':
     
        break;
     

      default:
        show_command_list = 1;
        run_mode = RUN_MODE_DEFAULT;
    }
    
  }
 
  switch (run_mode) {
    case RUN_MODE_DEFAULT:
      read_sensors();
      break;
  }
  delay(100);

    
}
