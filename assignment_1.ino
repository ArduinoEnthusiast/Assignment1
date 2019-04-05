#define BLYNK_PRINT Serial

// enter your internet details here so that esp8266 can connect to your wifi
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "YourAuthToken"; //this is how you interact with the blynk app

// Your WiFi credentials.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

//Clock 

#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;

//Arduino 

int pin11 = 11;                 //wifi module
int pin10 = 10;                 //wifi modele aswell
int pin8 = 8;                   //PIR sensor
int pin7 = 7;                   // Relay
int brightness = 0;
int fadeAmount = 5;
int pinA4 = A4;                 // clock module serial data
int pinA5 = A5;                 // clock module serial clock

int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status


 
void setup() {
//clock setup

// Start the serial port
  Serial.begin(57600);

  // Start the I2C interface
  Wire.begin();


  for (int i=0; i<5; i++){
      delay(1000);
      Serial.print(Clock.getYear(), DEC);
      Serial.print("-");
      Serial.print(Clock.getMonth(), DEC);
      Serial.print("-");
      Serial.print(Clock.getDate(), DEC);
      Serial.print(" ");
      Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
      Serial.print(":");
      Serial.print(Clock.getMinute(), DEC);
      Serial.print(":");
      Serial.println(Clock.getSecond(), DEC);
}

  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
 
  Serial.begin(9600);
   Blynk.begin(auth, ssid, pass);
}
 
void loop(){

  //this is the code for dimming the light over time
 analogWrite(led, brightness);

  brightness = brightness + fadeAmount;

    if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;

  // wait for 60 seconds before the dimming effect begins
  delay(60000);
   
   Blynk.run();
   
  val = digitalRead(inputPin);   // read input value
  if (val == HIGH) {             // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    delay(225000);               //  this is 3mins 45secs in milliseconds
    if (pirState == LOW) {       // we have just turned on
      Serial.println("Motion detected!"); // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW);   // turn LED OFF
    if (pirState == HIGH){      // we have just turned of
      Serial.println("Motion ended!");   // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}
