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

// Debouncing variables
unsigned long lastDebounceTimeA = 0;
unsigned long lastDebounceTimeB = 0;
unsigned long lastDebounceTimeC = 0;
unsigned long debounceDelay = 200; // 200ms debounce delay

// Sequence control variables
bool sequenceActive = false;
unsigned long sequenceStartTime = 0;
int sequenceStep = 0;

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
  
  // Handle sequence if active
  if (sequenceActive) {
    handleSequence();
    return; // Don't process button inputs during sequence
  }
  
  // Read buttons
  buttonStateA = digitalRead(buttonPinA);
  buttonStateB = digitalRead(buttonPinB);
  buttonStateC = digitalRead(buttonPinC);
  
  // Check for button presses with debouncing
  if (buttonStateA != lastButtonStateA && buttonStateA == HIGH) {
    if ((millis() - lastDebounceTimeA) > debounceDelay) {
      counter++;
      lastDebounceTimeA = millis();
      Serial.print("Juice signal received. Count: ");
      Serial.println(counter);
    }
  }
  if (buttonStateB != lastButtonStateB && buttonStateB == HIGH) {
    if ((millis() - lastDebounceTimeB) > debounceDelay) {
      counter++;
      lastDebounceTimeB = millis();
      Serial.print("Sauce signal received. Count: ");
      Serial.println(counter);
    }
  }
  if (buttonStateC != lastButtonStateC && buttonStateC == HIGH) {
    if ((millis() - lastDebounceTimeC) > debounceDelay) {
      counter++;
      lastDebounceTimeC = millis();
      Serial.print("Pie signal received. Count: ");
      Serial.println(counter);
    }
  }
  
  // Update button states
  lastButtonStateA = buttonStateA;
  lastButtonStateB = buttonStateB;
  lastButtonStateC = buttonStateC;
  
  // Trigger at 10 count
  if (counter >= 10) {
    sequenceActive = true;
    sequenceStartTime = millis();
    sequenceStep = 0;
    Serial.println("Starting sequence at count 10");
  }
}

void handleSequence() {
  unsigned long currentTime = millis();
  unsigned long elapsed = currentTime - sequenceStartTime;
  
  switch (sequenceStep) {
    case 0: // Flash display
      if (elapsed < 2000) { // Flash for 2 seconds
        matrix.print((elapsed / 250) % 2 == 0 ? 0 : 20);
        matrix.writeDisplay();
      } else {
        sequenceStep = 1;
        sequenceStartTime = currentTime;
        // Activate outputs
        digitalWrite(light, HIGH);
        digitalWrite(sound, HIGH);
        // Motor forward
        digitalWrite(forwards, LOW);
        digitalWrite(backwards, HIGH);
        Serial.println("Motor forward, lights/sound on");
      }
      break;
      
    case 1: // Motor forward
      if (elapsed >= 15000) { // 15 seconds forward
        sequenceStep = 2;
        sequenceStartTime = currentTime;
        // Motor stop
        digitalWrite(forwards, HIGH);
        digitalWrite(backwards, HIGH);
        // Turn off alerts
        digitalWrite(light, LOW);
        digitalWrite(sound, LOW);
        Serial.println("Motor stopped, lights/sound off");
      }
      break;
      
    case 2: // Motor backward
      if (elapsed >= 1000) { // 1 second pause
        sequenceStep = 3;
        sequenceStartTime = currentTime;
        // Motor backward
        digitalWrite(forwards, HIGH);
        digitalWrite(backwards, LOW);
        Serial.println("Motor backward");
      }
      break;
      
    case 3: // Complete sequence
      if (elapsed >= 15000) { // 15 seconds backward
        // Motor off
        digitalWrite(forwards, LOW);
        digitalWrite(backwards, LOW);
        // Reset
        counter = 0;
        sequenceActive = false;
        Serial.println("Sequence complete, counter reset");
      }
      break;
  }
}