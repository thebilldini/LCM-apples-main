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
unsigned long sequenceTimer = 0;
int sequenceStep = 0;
int blinkCount = 0;

void setup() {
  Serial.begin(9600);
  matrix.begin(0x70);
  
  // Initialize pins
  pinMode(buttonPinA, INPUT_PULLUP); // Pull-up for juice signal
  pinMode(buttonPinB, INPUT_PULLUP); // Pull-up for sauce signal  
  pinMode(buttonPinC, INPUT_PULLUP); // Pull-up for pie signal
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
  
  // Check for button presses with debouncing (now looking for LOW signals)
  if (buttonStateA != lastButtonStateA && buttonStateA == LOW) {
    if ((millis() - lastDebounceTimeA) > debounceDelay) {
      counter++;
      lastDebounceTimeA = millis();
      Serial.print("Juice signal received. Count: ");
      Serial.println(counter);
    }
  }
  if (buttonStateB != lastButtonStateB && buttonStateB == LOW) {
    if ((millis() - lastDebounceTimeB) > debounceDelay) {
      counter++;
      lastDebounceTimeB = millis();
      Serial.print("Sauce signal received. Count: ");
      Serial.println(counter);
    }
  }
  if (buttonStateC != lastButtonStateC && buttonStateC == LOW) {
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
    sequenceTimer = millis();
    sequenceStep = 0;
    blinkCount = 0;
    Serial.println("Starting sequence at count 10");
  }
}

void handleSequence() {
  // Display "10" for 20 seconds
  matrix.print(10);
  matrix.writeDisplay();
  delay(2000); // Wait 20 seconds
  
  // Countdown 3, 2, 1, 0
  matrix.print(3);
  matrix.writeDisplay();
  delay(1000);
  
  matrix.print(2);
  matrix.writeDisplay();
  delay(1000);
  
  matrix.print(1);
  matrix.writeDisplay();
  delay(1000);
  
  matrix.print(0);
  matrix.writeDisplay();
  delay(1000);
  
  // Start sounds and lights
  digitalWrite(light, HIGH);
  digitalWrite(sound, HIGH);
  Serial.println("Sounds and lights on");
  delay(3000); // Wait 3 seconds
  
  // Open basket
  digitalWrite(forwards, LOW);
  digitalWrite(backwards, HIGH);
  Serial.println("Basket opening");
  delay(15000); // Open for 15 seconds
  
  // Turn off lights and sounds, close basket
  digitalWrite(light, LOW);
  digitalWrite(sound, LOW);
  digitalWrite(forwards, HIGH);
  digitalWrite(backwards, LOW);
  Serial.println("Lights/sounds off, basket closing");
  delay(25000); // Close for 25 seconds
  
  // Stop motors and reset
  digitalWrite(forwards, LOW);
  digitalWrite(backwards, LOW);
  counter = 0;
  sequenceActive = false;
  Serial.println("Sequence complete, counter reset");
}