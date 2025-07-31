#include <SoftwareSerial.h>

// Software serial port for communication with SIM800L
SoftwareSerial sim800(10, 11); // RX, TX

const int pirPin = 8;
const int relayPin = 7;

bool motionDetected = false;
bool smsSent = false;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(relayPin, LOW);

  Serial.begin(9600);       // for serial monitor
  sim800.begin(9600);       // Communication with SIM800 module

  delay(2000);              // Time for the module to be ready
  Serial.println("System Ready...");
}

void loop() {
  int motion = digitalRead(pirPin);

  if (motion == HIGH && !motionDetected) {
    digitalWrite(9, HIGH);
    Serial.println("Movement Detected!");
    motionDetected = true;
    digitalWrite(relayPin, HIGH);
    
   
    if (!smsSent) {
      sendSMS("+9812345678910", "هشدار: حرکت در منطقه شناسایی شد!");
      smsSent = true;
    }

    delay(10000);
    digitalWrite(relayPin, LOW);
    digitalWrite(9, LOW);
  }

  if (motion == LOW && motionDetected) {
    motionDetected = false;
    smsSent = false;
    Serial.println("Ready for next detection...");
  }
}

void sendSMS(String number, String message) {
  sim800.println("AT+CMGF=1"); 
  delay(1000);
  sim800.print("AT+CMGS=\"");
  sim800.print(number);
  sim800.println("\"");
  delay(1000);
  sim800.print(message);
  sim800.write(26);
  delay(3000);
  Serial.println("SMS Sent");
}