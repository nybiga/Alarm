#include <Arduino.h>
#include "SevSeg.h"

// Segments and corresponding pin
int D4 = 22, G = 23, C = 24, DP = 25, D = 26, E = 27,
    B = 28, D2 = 29, D3 = 30, F = 31, A = 32, D1 = 33;
// Buzzer
int buzzer = 34;

// Timer
unsigned long timer = -1;

// SevSeg object
SevSeg sevseg; // Instantiate a seven segment controller object
byte numDigits = 4; // Number of digits
byte digitPins[] = {D1, D2, D3, D4}; // Digital pins
byte segmentPins[] = {A, B, C, D, E, F, G, DP}; // Segement pins
bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
byte hardwareConfig = COMMON_CATHODE; // See README.md for options

void setup() {
  // put your setup code here, to run once:
  // Sets up Sevseg
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);
  // Buzzer
  pinMode(buzzer, OUTPUT);

  // Begins serial monitor
  Serial.begin(9600);
}

void loop() {
  static int seconds = 0;

  // Make pick a number
  if (seconds <= 0){
    Serial.println("Pick a number from 1 - 9999 for timer");
    // Program doesn't move on until there is user input
    while (Serial.available() == 0);
    seconds = Serial.parseInt();
    Serial.println(seconds);
    timer = millis();
  } 

  // Sets number
  if (seconds >= 0) sevseg.setNumber(seconds);
  
  // Timer controller
  if (millis() - timer >= 1000) {
    timer += 1000; // 1000 milliSeconds is equal to 1 second 
    seconds--;
  }

  // Refreshes display
  sevseg.refreshDisplay(); // Must run repeatedly

  // Plays sounds when timer is up
  if (seconds == 0 && timer != -1) { 
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
  }
}