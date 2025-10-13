#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

// Button pins
const int buttonPinA = 3; // from juice 11
const int buttonPinB = 4; // from sauce 11
const int buttonPinC = 5; //from pie 11

// Output pins
const int light = 11; 
const int sound = 12;
const int forwards = 7;  // basket 
const int backwards = 6;

// Variables will change:
int counter = 0; // counter for the number of button presses
int buttonStateA = 0, lastButtonStateA = 0;
int buttonStateB = 0, lastButtonStateB = 0;
int buttonStateC = 0, lastButtonStateC = 0;

void setup() {
  Serial.begin(9600);
  matrix.begin(0x70);
  
  // Initialize pins
  pinMode(buttonPinA, INPUT);
  pinMode(buttonPinB, INPUT);
  pinMode(buttonPinC, INPUT);
  pinMode(light, OUTPUT);
  pinMode(sound, OUTPUT);
  pinMode(forwards, OUTPUT);
  pinMode(backwards, OUTPUT);
}

void loop() {
  // Display counter
  matrix.print(counter);
  matrix.writeDisplay();
  
  // Read buttons
  buttonStateA = digitalRead(buttonPinA);
  buttonStateB = digitalRead(buttonPinB);
  buttonStateC = digitalRead(buttonPinC);
  
  // Check for button presses
  if (buttonStateA != lastButtonStateA && buttonStateA == HIGH) {
    counter++;
    delay(50);
  }
  if (buttonStateB != lastButtonStateB && buttonStateB == HIGH) {
    counter++;
    delay(50);
  }
  if (buttonStateC != lastButtonStateC && buttonStateC == HIGH) {
    counter++;
    delay(50);
  }
  
  // Update button states
  lastButtonStateA = buttonStateA;
  lastButtonStateB = buttonStateB;
  lastButtonStateC = buttonStateC;
  
  // Trigger at 20 count
  if (counter >= 20) {
    // Alert sequence
    for (int i = 0; i < 4; i++) {
      matrix.print(i % 2 == 0 ? 0 : 20);
      matrix.writeDisplay();
      delay(500);
    }
    
    // Activate outputs
    digitalWrite(light, HIGH);
    digitalWrite(sound, HIGH);
    
    // Motor forward
    digitalWrite(forwards, LOW);
    digitalWrite(backwards, HIGH);
    delay(15000);
    
    // Motor stop
    digitalWrite(forwards, HIGH);
    digitalWrite(backwards, HIGH);
    
    // Turn off alerts
    digitalWrite(light, LOW);
    digitalWrite(sound, LOW);
    
    // Motor backward
    digitalWrite(forwards, HIGH);
    digitalWrite(backwards, LOW);
    delay(15000);
    
    // Motor off
    digitalWrite(forwards, LOW);
    digitalWrite(backwards, LOW);
    
    // Reset
    counter = 0;
  }
}