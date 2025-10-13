//JUICE
const int buttonPin = 3; // the pin that the pushbutton is attached to
const int count = 11;
const int sound = 12;

// Variables will change:
int counter = 0; // counter for the number of button presses
int buttonState = 0; // current state of the button
int lastButtonState = 0; // previous state of the button

void setup() {
  // Initialize the button pin as input:
  pinMode(buttonPin, INPUT);
  
  // Initialize outputs:
  pinMode(sound, OUTPUT);
  pinMode(count, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // Read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
  
  // Compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // If the state has changed, check if button was pressed
    if (buttonState == HIGH) {
      // Button went from off to on:
      counter++;
      Serial.println(counter);
      // Delay a little bit to avoid bouncing
      delay(50);
    }
    // Save the current state as the last state for next time through the loop
    lastButtonState = buttonState;
  }
  
  // Activate outputs when counter reaches 4
  if (counter >= 6) {
    // Send signal to Apple Counter Arduino
    digitalWrite(count, HIGH);
    delay(180);
    digitalWrite(count, LOW);
    delay(50);
    
    // Sound alert
    digitalWrite(sound, HIGH);
    delay(500);
    digitalWrite(sound, LOW);
    delay(10);
    
    // Reset counter
    counter = 0;
  }
}