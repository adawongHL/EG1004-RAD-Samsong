#include "Keyboard.h"

// declare button pins
const int buttonPinA = 5;
const int buttonPinB = 4;   
const int buttonPinC = 3;   
const int buttonPinD = 2;      

// declare vars to store button states for comparison between iterations of the loop
int prevButtonStateA = HIGH;
int prevButtonStateB = HIGH;
int prevButtonStateC = HIGH;
int prevButtonStateD = HIGH;


void setup() {
  // declare the buttons as input_pullup so there's no floating pins - when not pressed, button input pins are attached to 5V; when pressed, 0V
  // hence the inverted logic (pressed is LOW; not pressed is HIGH)
  
  pinMode(buttonPinA, INPUT_PULLUP);  
  pinMode(buttonPinB, INPUT_PULLUP); 
  pinMode(buttonPinC, INPUT_PULLUP); 
  pinMode(buttonPinD, INPUT_PULLUP); 
  Keyboard.begin();   // actually does nothing. Treat this line as a visual marker that a keyboard is now started. 
}

void loop() {
  // checking the state of each button
  int buttonStateA = digitalRead(buttonPinA);
  int buttonStateB = digitalRead(buttonPinB);
  int buttonStateC = digitalRead(buttonPinC);
  int buttonStateD = digitalRead(buttonPinD);
  

  // Button A is currently pressed and previously not pressed --> Press the button
  if (buttonStateA == LOW && prevButtonStateA == HIGH) {
    Keyboard.press('a');  // Use .press not .write to achieve continuous key press; otherwise, button will release right after. 
    // Since we use .press, we need a way to release the key. See below 
    delay(50);
  }

  // Button A is currently not pressed and was previously pressed --> Release the button
  if (buttonStateA == HIGH && prevButtonStateA == LOW) {
    Keyboard.release('a'); // without this, if this button was previously pressed, it would forever keep pressing without releasing
    delay(50);
  }

  if (buttonStateB == LOW && prevButtonStateB == HIGH) {
    Keyboard.press('b');
    delay(50);
  }

  if (buttonStateB == HIGH && prevButtonStateB == LOW) {
    Keyboard.release('b');
    delay(50);
  }
  
  if (buttonStateC == LOW && prevButtonStateC == HIGH) {
    Keyboard.press('c');
    delay(50);
  }

  if (buttonStateC == HIGH && prevButtonStateC == LOW) {
    Keyboard.release('c');
    delay(50);
  }

  if (buttonStateD == LOW && prevButtonStateD == HIGH) {
    Keyboard.press('d');
    delay(50);
  }

  if (buttonStateD == HIGH && prevButtonStateD == LOW) {
    Keyboard.release('d');
    delay(50);
  }

  // Update button states such that the next iteration can compare to button states of this iteration
  prevButtonStateA = buttonStateA;
  prevButtonStateB = buttonStateB;
  prevButtonStateC = buttonStateC;
  prevButtonStateD = buttonStateD;

delay(10);

}

// Testing space: 