// WOKWI SIMULATION - All 4 Arduino Apple Counter System
// This simulates juice, sauce, pie, and main Arduinos in one sketch

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

// ========== PIN DEFINITIONS ==========
// Main Arduino (simulated) pins
const int main_buttonPinA = 3;  // from juice
const int main_buttonPinB = 4;  // from sauce 
const int main_buttonPinC = 5;  // from pie
const int main_light = 11;
const int main_sound = 12;
const int main_forwards = 7;
const int main_backwards = 6;

// Sub-Arduino button pins (actual physical buttons in Wokwi)
const int juice_button = A0;    // Juice button
const int sauce_button = A1;    // Sauce button  
const int pie_button = A2;      // PIE button

// Sub-Arduino output pins (to main Arduino)
const int juice_signal = 8;     // Juice pin 11 simulation
const int sauce_signal = 9;     // Sauce pin 11 simulation
const int pie_signal = 10;      // PIE pin 11 simulation

// Sub-Arduino sound pins
const int juice_sound = A3;
const int sauce_sound = A4; 
const int pie_sound = A5;

// ========== VARIABLES ==========
// Main Arduino variables
int main_counter = 0;
int main_buttonStateA = 0, main_lastButtonStateA = 0;
int main_buttonStateB = 0, main_lastButtonStateB = 0;
int main_buttonStateC = 0, main_lastButtonStateC = 0;

// Juice Arduino variables
int juice_counter = 0;
int juice_buttonState = 0, juice_lastButtonState = 0;

// Sauce Arduino variables  
int sauce_counter = 0;
int sauce_buttonState = 0, sauce_lastButtonState = 0;

// PIE Arduino variables
int pie_counter = 0;
int pie_buttonState = 0, pie_lastButtonState = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("=== APPLE COUNTER SYSTEM SIMULATION ===");
  
  // Initialize 7-segment display
  matrix.begin(0x70);
  matrix.print(0);
  matrix.writeDisplay();
  
  // Main Arduino pin setup
  pinMode(main_buttonPinA, INPUT);
  pinMode(main_buttonPinB, INPUT);
  pinMode(main_buttonPinC, INPUT);
  pinMode(main_light, OUTPUT);
  pinMode(main_sound, OUTPUT);
  pinMode(main_forwards, OUTPUT);
  pinMode(main_backwards, OUTPUT);
  
  // Sub-Arduino pin setup
  pinMode(juice_button, INPUT);
  pinMode(sauce_button, INPUT);
  pinMode(pie_button, INPUT);
  pinMode(juice_signal, OUTPUT);
  pinMode(sauce_signal, OUTPUT);
  pinMode(pie_signal, OUTPUT);
  pinMode(juice_sound, OUTPUT);
  pinMode(sauce_sound, OUTPUT);
  pinMode(pie_sound, OUTPUT);
  
  Serial.println("System ready - Press buttons to count apples!");
}

void loop() {
  // Update 7-segment display with main counter
  matrix.print(main_counter);
  matrix.writeDisplay();
  
  // ========== SIMULATE JUICE ARDUINO ==========
  juice_buttonState = digitalRead(juice_button);
  if (juice_buttonState != juice_lastButtonState) {
    if (juice_buttonState == HIGH) {
      juice_counter++;
      Serial.print("JUICE: Count ");
      Serial.println(juice_counter);
      delay(50);
    }
    juice_lastButtonState = juice_buttonState;
  }
  
  // Juice Arduino reaches 4 - send signal
  if (juice_counter >= 4) {
    Serial.println("JUICE: Sending signal to main!");
    digitalWrite(juice_sound, HIGH);
    digitalWrite(juice_signal, HIGH);
    delay(180);
    digitalWrite(juice_signal, LOW);
    digitalWrite(juice_sound, LOW);
    juice_counter = 0;
    delay(500);
  }
  
  // ========== SIMULATE SAUCE ARDUINO ==========
  sauce_buttonState = digitalRead(sauce_button);
  if (sauce_buttonState != sauce_lastButtonState) {
    if (sauce_buttonState == HIGH) {
      sauce_counter++;
      Serial.print("SAUCE: Count ");
      Serial.println(sauce_counter);
      delay(50);
    }
    sauce_lastButtonState = sauce_buttonState;
  }
  
  // Sauce Arduino reaches 4 - send signal
  if (sauce_counter >= 4) {
    Serial.println("SAUCE: Sending signal to main!");
    digitalWrite(sauce_sound, HIGH);
    digitalWrite(sauce_signal, HIGH);
    delay(180);
    digitalWrite(sauce_signal, LOW);
    digitalWrite(sauce_sound, LOW);
    sauce_counter = 0;
    delay(500);
  }
  
  // ========== SIMULATE PIE ARDUINO ==========
  pie_buttonState = digitalRead(pie_button);
  if (pie_buttonState != pie_lastButtonState) {
    if (pie_buttonState == HIGH) {
      pie_counter++;
      Serial.print("PIE: Count ");
      Serial.println(pie_counter);
      delay(50);
    }
    pie_lastButtonState = pie_buttonState;
  }
  
  // PIE Arduino reaches 4 - send signal
  if (pie_counter >= 4) {
    Serial.println("PIE: Sending signal to main!");
    digitalWrite(pie_sound, HIGH);
    digitalWrite(pie_signal, HIGH);
    delay(180);
    digitalWrite(pie_signal, LOW);
    digitalWrite(pie_sound, LOW);
    pie_counter = 0;
    delay(500);
  }
  
  // ========== SIMULATE MAIN ARDUINO ==========
  // Read signals from sub-Arduinos
  main_buttonStateA = digitalRead(juice_signal);
  main_buttonStateB = digitalRead(sauce_signal);
  main_buttonStateC = digitalRead(pie_signal);
  
  // Check for signals from juice
  if (main_buttonStateA != main_lastButtonStateA && main_buttonStateA == HIGH) {
    main_counter++;
    Serial.print("MAIN: Total count ");
    Serial.println(main_counter);
  }
  
  // Check for signals from sauce
  if (main_buttonStateB != main_lastButtonStateB && main_buttonStateB == HIGH) {
    main_counter++;
    Serial.print("MAIN: Total count ");
    Serial.println(main_counter);
  }
  
  // Check for signals from pie
  if (main_buttonStateC != main_lastButtonStateC && main_buttonStateC == HIGH) {
    main_counter++;
    Serial.print("MAIN: Total count ");
    Serial.println(main_counter);
  }
  
  // Update main button states
  main_lastButtonStateA = main_buttonStateA;
  main_lastButtonStateB = main_buttonStateB;
  main_lastButtonStateC = main_buttonStateC;
  
  // ========== MAIN ARDUINO ACTION AT 20 ==========
  if (main_counter >= 20) {
    Serial.println("========================================");
    Serial.println("MAIN: 20 APPLES REACHED! STARTING SEQUENCE!");
    Serial.println("========================================");
    
    // Activate main outputs
    Serial.println("MAIN: Turning on lights and sound");
    digitalWrite(main_light, HIGH);
    digitalWrite(main_sound, HIGH);
    
    // Motor forward sequence
    Serial.println("MAIN: Motor forward for 15 seconds");
    digitalWrite(main_forwards, HIGH);
    digitalWrite(main_backwards, LOW);
    
    // Simulate 15 seconds with progress updates
    for (int i = 1; i <= 15; i++) {
      Serial.print("Motor forward: ");
      Serial.print(i);
      Serial.println("/15 seconds");
      delay(1000);
    }
    
    // Stop motor
    digitalWrite(main_forwards, LOW);
    digitalWrite(main_backwards, LOW);
    
    // Turn off alerts
    Serial.println("MAIN: Turning off lights and sound");
    digitalWrite(main_light, LOW);
    digitalWrite(main_sound, LOW);
    
    // Motor backward sequence
    Serial.println("MAIN: Motor backward for 15 seconds");
    digitalWrite(main_forwards, LOW);
    digitalWrite(main_backwards, HIGH);
    
    // Simulate 15 seconds with progress updates
    for (int i = 1; i <= 15; i++) {
      Serial.print("Motor backward: ");
      Serial.print(i);
      Serial.println("/15 seconds");
      delay(1000);
    }
    
    // Stop motor completely
    digitalWrite(main_forwards, LOW);
    digitalWrite(main_backwards, LOW);
    
    // Reset system
    Serial.println("MAIN: Sequence complete - Resetting counter");
    Serial.println("========================================");
    main_counter = 0;
    
    delay(2000); // Pause before next cycle
  }
  
  delay(10); // Small delay for stability
}