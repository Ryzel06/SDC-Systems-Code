int data;
#define relaypin 10



void setup() {
  Serial.begin(9600);
  pinMode(relaypin, OUTPUT);
  digitalWrite(relaypin, LOW);
  
  Serial.println("hihihihhfuisdhiufsad");
}

void loop() {
  while (Serial.available()){
    data = Serial.read() ;
  }
  
  if (data == '1') {
    digitalWrite (relaypin, HIGH);
    
  } else if (data == '0') {
      digitalWrite (relaypin, LOW);
    }
  }

