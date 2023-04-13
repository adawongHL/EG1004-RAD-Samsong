#include "Keyboard.h"

//declaring button pins
const int buttonPin = 2;       

//int previousButtonState = HIGH; 

void setup() {
  //declare the buttons as input_pullup
  pinMode(buttonPin, INPUT_PULLUP);  
  Keyboard.begin();
}

void loop() {
  //checking the state of the button
  int buttonState = digitalRead(buttonPin);
  
 //replaces button press with UP arrow
  //if (buttonState == LOW && previousButtonState == HIGH) {
  if (buttonState == LOW) {
      // and it's currently pressed:
    Keyboard.write(' ');
    delay(100);
  }

  //if (buttonState == LOW && previousButtonState == HIGH) {
      // and it's currently released:
  //if (buttonState == LOW) {
    //Keyboard.press('A');
    //delay(50);
  //
 
  //previousButtonState = buttonState;
}

