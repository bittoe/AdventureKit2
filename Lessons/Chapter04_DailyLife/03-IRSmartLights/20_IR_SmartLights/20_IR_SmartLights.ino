/*
 * Day 0 - AI Apocalypse by inventr.io
 * Learn more at https://inventr.io/PLACEHOLDER
 *
 * Now that we have decoded which buttons generate which commands we can use these to
 * control our RGB LED.  Wire up the RGB LED per the attached wiring diagram.  We'll
 * use the power button to turn the light on/off.  Now we can turn the light on and
 * off my pressing the power button on our remote!
 *
 * Alex Eschenauer
 * David Schmidt
 */

#include "Arduino.h"
#include <IRremote.h>

/* Choosing what pin to use.
 * This project only needs digital pins.  So, on the Hero XL we *could* use any digital or analog pin
 * (D0-D53, A0-A15).  However, some pins support specialized functions and in later lessons we will be
 * using multiple parts, some of which will use some of those special functions.  So, let's start out
 * on the right foot by trying to use the most basic pin we can for each of our parts.
 *
 * Skip: A0-A15 (save for Analog),
 *       D0/D1 (used by USB and Serial)
 *       D14-D19 (used by Serial 1-3)
 *       D2/D3, D18-D21 (used for external interrupts)
 *       D13 (this pin controls the build in LED on the HERO XL board (LED_BUILTIN)),
 *       D2-D13, D44-D46 (used for Pulse Width Modulation (PWM))
 *       D50 (MISO), D51 (MOSI), D52 (SCK), D53 (SS).  (used for SPI communication)
 *       D20 (SDA), D21 (SCL).  (used for I2C communication using the Wire library)
 * Recommended for fewest conflicts:
 *    D22-D43, D47-D49, A8-A15
 */

const uint8_t IR_SIGNAL_PIN = 28;  // Signal Pin of IR receiver

// RGB LED control pins
const uint8_t RED_PIN = 22;
const uint8_t GREEN_PIN = 24;
const uint8_t BLUE_PIN = 26;

// Remap HIGH/LOW to ON/OFF for code readability
const uint8_t ON = 255;
const uint8_t OFF = LOW;

// This structure allows us to display a description for each recognized command
struct RemoteButton {
  unsigned int command;     // command generated by IR remote button
  const char* description;  // button description
};

// Define a constant equal to our decoded command for each of our remote's buttons
enum IR_COMMANDS {
  POWER_BUTTON = 69,
  BUTTON_0 = 22,
  BUTTON_1 = 12,
  BUTTON_2 = 24,
  BUTTON_3 = 94,
  BUTTON_4 = 8,
  BUTTON_5 = 28,
  BUTTON_6 = 90,
  BUTTON_7 = 66,
  BUTTON_8 = 82,
  BUTTON_9 = 74,
  VOLUME_UP_BUTTON = 70,
  VOLUME_DOWN_BUTTON = 21,
  FUNC_STOP = 71,
  REWIND = 68,
  PLAY_PAUSE = 64,
  FAST_FORWARD = 67,
  DOWN = 7,
  UP = 9,
  EQ = 25,
  ST_REPT = 13,
};

// Define what command each of our buttons generates.
const RemoteButton BUTTONS[] = {
  // Add more buttons and descriptions as we run the sketch and identify what commands go with each button
  { 69, "Power button" },  // Add new buttons below this button
  { 12, "1 button" },      // comma isn't required on last entry, but it doesn't hurt and makes insertions easier
};

const int BUTTON_COUNT = sizeof(BUTTONS) / sizeof(RemoteButton);

// Initialize our IR Remote library code.
IRrecv receiver(IR_SIGNAL_PIN);

void setup() {
  Serial.begin(9600);
  receiver.enableIRIn();  // Start the receiver

  // LED pins are all outputs
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

// Track whether light is powered on or off, and last status of each color
uint8_t light_is_on = false;

void loop() {
  // Checks received an IR signal
  if (receiver.decode()) {
    unsigned int IRCommand = receiver.decodedIRData.command;
    displayIRCommand(IRCommand);

    if (IRCommand == POWER_BUTTON) {
      if (light_is_on) {  // light is on, turn off
        setColor(OFF, OFF, OFF);
        light_is_on = false;
      } else {
        setColor(ON, ON, ON);
        light_is_on = true;
      }
    }
    delay(50);          // Delay to skip double presses
    receiver.resume();  // Receive the next value
  }
}

void displayIRCommand(unsigned int command) {
  for (int i = 0; i < BUTTON_COUNT; i++) {
    if (BUTTONS[i].command == command) {
      Serial.println(String(BUTTONS[i].description) + "(" + String(int(command)) + ")");
      return;
    }
  }
  Serial.println("Unrecognized command: " + String(command));
}

void setColor(int redValue, int greenValue, int blueValue) {
  digitalWrite(RED_PIN, redValue);
  digitalWrite(GREEN_PIN, greenValue);
  digitalWrite(BLUE_PIN, blueValue);
}
