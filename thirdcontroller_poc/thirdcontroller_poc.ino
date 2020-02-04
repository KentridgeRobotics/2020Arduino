#include "Joystick.h"

Joystick_ Joystick(0x06, JOYSTICK_TYPE_JOYSTICK, 8, 1, true, true, true, true, false, false, false, false, false, false, false);

// Function declarations
void initButton(struct Buttons *button, uint8_t initPin, uint8_t initValue);
void checkButton(struct Buttons *button);

unsigned long debounceDelay = 25;
unsigned long minLatchStateTime = 1000;

// Structure for managing momentary latching button config and state
struct LatchingMomentaryButtons {
  uint8_t pin = 0;
  bool latchState = false;
  bool lastButtonState = false;
  uint8_t value;
  unsigned long lastDebounceTime = 0;
  unsigned long lastLatchStateChangeTime = 0;
};

struct MomentaryButtons {
  uint8_t pin = 0;
  bool lastButtonState = false;
  uint8_t value;
  unsigned long lastDebounceTime = 0;
};

// Allocate buttons
struct LatchingMomentaryButtons zeroButton;
struct MomentaryButtons oneButton;

void setup() {
  // Initialize the button structs
  initLatchingMomentaryButton(&zeroButton, 2, 0);
  initMomentaryButton(&oneButton, 3, 1);
  Joystick.begin();
  Serial.begin(9600);
}

void loop() {
  checkLatchingMomentaryButton(&zeroButton);
  checkMomentaryButton(&oneButton);
}

void initLatchingMomentaryButton(struct LatchingMomentaryButtons *button, uint8_t initPin, uint8_t initValue) {
  button->pin = initPin;
  button->value = initValue;
  pinMode(button->pin, INPUT); // Using pulldown resitors outside of the Arduino so basic INPUT is all we need
}

void initMomentaryButton(struct MomentaryButtons *button, uint8_t initPin, uint8_t initValue) {
  button->pin = initPin;
  button->value = initValue;
  pinMode(button->pin, INPUT); // Using pulldown resitors outside of the Arduino so basic INPUT is all we need
}

/*
  Function:  checkLatchingMomentaryButton
  Use this function to check the state of a momentary button, debounce its state and flip the latch state.
    Example:
      Push the button -> latchState = HIGH, then
      Push the button again (after minLatchStateTime) -> latchState = LOW
  Think of a latching momentary button as a momentary button that acts like a toggle switch.  But instead of flipping a 
  switch you are pushing the button to flip the switch.  Each press of the button flips the state.
*/
void checkLatchingMomentaryButton(struct LatchingMomentaryButtons *button) {
  String logString = String("Button: ");
  logString.concat((String)button->value);
  int currentButtonState = digitalRead(button->pin);
  logString.concat(" currentButtonState: ");
  logString.concat((String)currentButtonState);
  logString.concat(" lastButtonState: ");
  logString.concat((String)button->lastButtonState);
  logString.concat(" latchState: ");
  logString.concat((String)button->latchState);
  logString.concat(" - millis: ");
  logString.concat((String)millis());
  //Serial.println(logString);
  if (currentButtonState != button->lastButtonState) {
    Serial.println("\tResetting lastDebounceTime...");
    button->lastDebounceTime = millis();
  }

  // debounce the button state to guard against transient state changes...also wait a minimum amount of time before 
  // flipping the latchState so it doesn't keep flipping if you hold the button too long
  if (((millis() - button->lastDebounceTime) > debounceDelay) && ((millis() - button->lastLatchStateChangeTime) > minLatchStateTime)) {
    if (currentButtonState) {
      //Serial.println("\tFlipping latchState...");
      Joystick.setButton(button->value, !button->latchState);
      button->latchState = !button->latchState;
      //button->lastButtonState = !button->lastButtonState;
      button->lastLatchStateChangeTime = millis();
    }
    
  } else {
    button->lastButtonState = currentButtonState;
    //Serial.println("\tWaiting for debounceDelay...");
  }
}

/*
  Function:  checkLatchingMomentaryButton
  Use this function to check the state of a momentary button, debounce its state and flip the latch state.
    Example:
      Push the button -> latchState = HIGH, then
      Push the button again (after minLatchStateTime) -> latchState = LOW
  Think of a latching momentary button as a momentary button that acts like a toggle switch.  But instead of flipping a 
  switch you are pushing the button to flip the switch.  Each press of the button flips the state.
*/
void checkMomentaryButton(struct MomentaryButtons *button) {
  String logString = String("Button: ");
  logString.concat((String)button->value);
  int currentButtonState = digitalRead(button->pin);
  logString.concat(" currentButtonState: ");
  logString.concat((String)currentButtonState);
  logString.concat(" lastButtonState: ");
  logString.concat((String)button->lastButtonState);
  logString.concat(" - millis: ");
  logString.concat((String)millis());
  //Serial.println(logString);
  if (currentButtonState != button->lastButtonState) {
    Serial.println("\tResetting lastDebounceTime...");
    button->lastDebounceTime = millis();
  }

  // debounce the button state to guard against transient state changes...also wait a minimum amount of time before 
  // flipping the latchState so it doesn't keep flipping if you hold the button too long
  if ((millis() - button->lastDebounceTime) > debounceDelay) {
      Joystick.setButton(button->value, currentButtonState);
    
  } else {
    button->lastButtonState = currentButtonState;
    //Serial.println("\tWaiting for debounceDelay...");
  }
}
