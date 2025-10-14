//SAUCE
const int buttonPin = 3; // the pin that the pushbutton is attached to = Purple
const int count = 11; //= Blue
const int sound = 12; //= Orange

// Variables will change:
int counter = 0; // counter for the number of button presses
int buttonState = 0;
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
  if (counter >= 4) {
    Serial.println("Sauce counter reached 4, sending signal");
    
    // Send signal to Apple Counter Arduino
    digitalWrite(count, HIGH);
    delay(300); // Longer pulse for better detection
    digitalWrite(count, LOW);
    
    // Sound alert
    digitalWrite(sound, HIGH);
    delay(500);
    digitalWrite(sound, LOW);
    
    // Reset counter
    counter = 0;
    Serial.println("Sauce counter reset");
    
    // Wait before accepting new button presses
    delay(1000);
  }
}