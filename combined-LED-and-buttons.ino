#include "Keyboard.h"
#include <Adafruit_NeoPixel.h>  //Library to simplify interacting with the LED strand
#ifdef __AVR__
#include <avr/power.h>   //Includes the library for power reduction registers if your chip supports them.
#endif                   //More info: http://www.nongnu.org/avr-libc/user-manual/group__avr__power.htlm


// KEYBOARD 
// declare button pins
const int buttonPinA = 5;
const int buttonPinB = 4;   
const int buttonPinC = 3;   
const int buttonPinD = 2;  

// declare vibration motor pins
const int buzzerPinA = 6; 
const int buzzerPinB = 7; 
const int buzzerPinC = 8; 
const int buzzerPinD = 9; 

// declare vars to store button states for comparison between iterations of the loop
int prevButtonStateA = HIGH;
int prevButtonStateB = HIGH;
int prevButtonStateC = HIGH;
int prevButtonStateD = HIGH;


// LED Music Visualization
//Constants (change these as necessary)
#define LED_PIN   A5  //Pin for the pixel strand. Does not have to be analog.
#define LED_TOTAL 300  //Change this to the number of LEDs in your strand.
#define LED_HALFb  30
#define LED_HALFg  60
#define LED_HALFy  90
#define LED_HALFr  120


#define AUDIO_PIN A0  //Pin for the envelope of the sound detector
#define KNOB_PIN  A1  //Pin for the trimpot 10K


//<Globals>
//  These values either need to be remembered from the last pass of loop() or
//  need to be accessed by several functions in one pass, so they need to be global.

Adafruit_NeoPixel strand = Adafruit_NeoPixel(LED_TOTAL, LED_PIN, NEO_GRB + NEO_KHZ800);  //LED strand objetc

uint16_t gradient = 0; //Used to iterate and loop through each color palette gradually

uint8_t volume = 0;    //Holds the volume level read from the sound detector.
uint8_t last = 0;      //Holds the value of volume from the previous loop() pass.

float maxVol = 10;     //Holds the largest volume recorded thus far to proportionally adjust the visual's responsiveness.
float knob = 1023.0;   //Holds the percentage of how twisted the trimpot is. Used for adjusting the max brightness.
float avgVol = 0;      //Holds the "average" volume-level to proportionally adjust the visual experience.
float avgBump = 0;     //Holds the "average" volume-change to trigger a "bump."

bool bump = false;     //Used to pass if there was a "bump" in volume





void setup() {
 Serial.begin(9600); //Sets data rate for serial data transmission.
 strand.begin(); //Initialize the LED strand object.
 strand.show();  //Show a blank strand, just to get the LED's ready for use. 

  // declare the buttons as input_pullup so there's no floating pins - when not pressed, button input pins are attached to 5V; when pressed, 0V
  // hence the inverted logic (pressed is LOW; not pressed is HIGH)
  
  pinMode(buttonPinA, INPUT_PULLUP);  
  pinMode(buttonPinB, INPUT_PULLUP); 
  pinMode(buttonPinC, INPUT_PULLUP); 
  pinMode(buttonPinD, INPUT_PULLUP);

  pinMode(buzzerPinA, OUTPUT);
  pinMode(buzzerPinB, OUTPUT);
  pinMode(buzzerPinC, OUTPUT);
  pinMode(buzzerPinD, OUTPUT);

  Keyboard.begin();   // actually does nothing. Treat this line as a visual marker that a keyboard is now started. 
}






void loop() {

 volume = analogRead(AUDIO_PIN);       //Record the volume level from the sound detector
 knob = analogRead(KNOB_PIN) / 1023.0; //Record how far the trimpot is twisted
 avgVol = (avgVol + volume) / 2.0;     //Take our "average" of volumes.

 //Sets a threshold for volume.
 //  In practice I've found noise can get up to 15, so if it's lower, the visual thinks it's silent.
 //  Also if the volume is less than average volume / 2 (essentially an average with 0), it's considered silent.
 if (volume < avgVol / 2.0 || volume < 15) volume = 0;

 //If the current volume is larger than the loudest value recorded, overwrite
 if (volume > maxVol) maxVol = volume;

 //This is where "gradient" is reset to prevent overflow.
 if (gradient > 1529) {
   gradient %= 1530;

   //Everytime a palette gets completed is a good time to readjust "maxVol," just in case
   //  the song gets quieter; we also don't want to lose brightness intensity permanently
   //  because of one stray loud sound.
   maxVol = (maxVol + volume) / 2.0;
 }



  // checking the state of each button
  int buttonStateA = digitalRead(buttonPinA);
  int buttonStateB = digitalRead(buttonPinB);
  int buttonStateC = digitalRead(buttonPinC);
  int buttonStateD = digitalRead(buttonPinD);
  

  // Button A is currently pressed and previously not pressed --> Press the button
  if (buttonStateA == LOW && prevButtonStateA == HIGH) {
    Keyboard.press('a');  // Use .press not .write to achieve continuous key press; otherwise, button will release right after. 
    // Since we use .press, we need a way to release the key. See below 
    digitalWrite(buzzerPinA, HIGH);
    delay(30);
    digitalWrite(buzzerPinA, LOW);
  }

  // Button A is currently not pressed and was previously pressed --> Release the button
  if (buttonStateA == HIGH && prevButtonStateA == LOW) {
    Keyboard.release('a'); // without this, if this button was previously pressed, it would forever keep pressing without releasing
    delay(30);
  }

  if (buttonStateB == LOW && prevButtonStateB == HIGH) {
    Keyboard.press('b');
    digitalWrite(buzzerPinB, HIGH);
    delay(30);
    digitalWrite(buzzerPinB, LOW);
  }

  if (buttonStateB == HIGH && prevButtonStateB == LOW) {
    Keyboard.release('b');
    delay(30);
  }
  
  if (buttonStateC == LOW && prevButtonStateC == HIGH) {
    Keyboard.press('c');
    digitalWrite(buzzerPinC, HIGH);
    delay(30);
    digitalWrite(buzzerPinC, LOW);
  }

  if (buttonStateC == HIGH && prevButtonStateC == LOW) {
    Keyboard.release('c');
    delay(100);
  }

  if (buttonStateD == LOW && prevButtonStateD == HIGH) {
    Keyboard.press('d');
    digitalWrite(buzzerPinD, HIGH);
    delay(30);
    digitalWrite(buzzerPinD, LOW);
  }

  if (buttonStateD == HIGH && prevButtonStateD == LOW) {
    Keyboard.release('d');
    delay(30);
  }

  // Update button states such that the next iteration can compare to button states of this iteration
  prevButtonStateA = buttonStateA;
  prevButtonStateB = buttonStateB;
  prevButtonStateC = buttonStateC;
  prevButtonStateD = buttonStateD;

// delay(10);




 if (volume - last > avgVol - last && avgVol - last > 0) avgBump = (avgBump + (volume - last)) / 2.0;


 //if there is a notable change in volume, trigger a "bump"
 bump = (volume - last) > avgBump;


 Pulse(LED_HALFb);   //Calls the visual to be displayed with the globals as they are.
 Pulse(LED_HALFg);   //Calls the visual to be displayed with the globals as they are.
 Pulse(LED_HALFy);   //Calls the visual to be displayed with the globals as they are.
 Pulse(LED_HALFr);   //Calls the visual to be displayed with the globals as they are.


 gradient++;    //Increments gradient

 last = volume; //Records current volume for next pass

 delay(30);   //Paces visuals so they aren't too fast to be enjoyable
}


//PULSE
//Pulse from center of the strand
void Pulse(int half) {
 fade(0.75);   //Listed below, this function simply dims the colors a little bit each pass of loop()
 //Advances the gradient to the next noticeable color if there is a "bump"
 if (bump) gradient += 64;

 //If it's silent, we want the fade effect to take over, hence this if-statement
 if (volume > 0) {
   uint32_t col = Rainbow(gradient); //Our retrieved 32-bit color

   //These variables determine where to start and end the pulse since it starts from the middle of the strand.
   //  The quantities are stored in variables so they only have to be computed once.
   int start = half - (15 * (volume / maxVol));
   int finish = half + (15 * (volume / maxVol)) + strand.numPixels() % 2;
   //int finish = half + (half * (volume / maxVol));
   //Listed above, LED_HALF is simply half the number of LEDs on your strand. ↑ this part adjusts for an odd quantity.




   for (int i = start; i < finish; i++) {
     //"damp" creates the fade effect of being dimmer the farther the pixel is from the center of the strand.
     //  It returns a value between 0 and 1 that peaks at 1 at the center of the strand and 0 at the ends.
     float damp = float(
                    ((finish - start) / 2.0) -
                    abs((i - start) - ((finish - start) / 2.0))
                  )
                  / float((finish - start) / 2.0);

     //Sets the each pixel on the strand to the appropriate color and intensity
     //  strand.Color() takes 3 values between 0 & 255, and returns a 32-bit integer.
     //  Notice "knob" affecting the brightness, as in the rest of the visuals.
     //  Also notice split() being used to get the red, green, and blue values.
     strand.setPixelColor(i, strand.Color(
                            split(col, 0) * pow(damp, 2.0) * knob,
                            split(col, 1) * pow(damp, 2.0) * knob,
                            split(col, 2) * pow(damp, 2.0) * knob
                          ));
   }
   //Sets the max brightness of all LEDs. If it's loud, it's brighter.
   //  "knob" was not used here because it occasionally caused minor errors in color display.
   strand.setBrightness(255.0 * pow(volume / maxVol, 2));
 }


 //This command actually shows the lights. If you make a new visualization, don't forget this!
 strand.show();
}


//Fades lights by multiplying them by a value between 0 and 1 each pass of loop().
void fade(float damper) {
 //"damper" must be between 0 and 1, or else you'll end up brightening the lights or doing nothing.
 if (damper >= 1) damper = 0.99;
 
 for (int i = 0; i < strand.numPixels(); i++) {
   //Retrieve the color at the current position.
   uint32_t col = (strand.getPixelColor(i)) ? strand.getPixelColor(i) : strand.Color(0, 0, 0);

   //If it's black, you can't fade that any further.
   if (col == 0) continue;

   float colors[3]; //Array of the three RGB values

   //Multiply each value by "damper"
   for (int j = 0; j < 3; j++) colors[j] = split(col, j) * damper;

   //Set the dampened colors back to their spot.
   strand.setPixelColor(i, strand.Color(colors[0] , colors[1], colors[2]));
 }
}

uint8_t split(uint32_t color, uint8_t i ) {
 //0 = Red, 1 = Green, 2 = Blue
 if (i == 0) return color >> 16;
 if (i == 1) return color >> 8;
 if (i == 2) return color >> 0;
 return -1;
}

//This function simply take a value and returns a gradient color
//  in the form of an unsigned 32-bit integer

//The gradient returns a different, changing color for each multiple of 255
//  This is because the max value of any of the 3 LEDs is 255, so it's
//  an intuitive cutoff for the next color to start appearing.
//  Gradients should also loop back to their starting color so there's no jumps in color.

uint32_t Rainbow(unsigned int i) {
 if (i > 1529) return Rainbow(i % 1530);
 if (i > 1274) return strand.Color(255, 0, 255 - (i % 255));   //violet -> red
 if (i > 1019) return strand.Color((i % 255), 0, 255);         //blue -> violet
 if (i > 764) return strand.Color(0, 255 - (i % 255), 255);    //aqua -> blue
 if (i > 509) return strand.Color(0, 255, (i % 255));          //green -> aqua
 if (i > 255) return strand.Color(255 - (i % 255), 255, 0);    //yellow -> green
 return strand.Color(255, i, 0);                               //red -> yellow
}




// Testing space: 