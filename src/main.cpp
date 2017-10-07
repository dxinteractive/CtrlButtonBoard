/*
 * Main program file for CtrlButtonBoard 
 * external controller for the Ctrl pedal
 * (CV generator / metronome / tuner).
 * Copyright (c) 2017 Damien Clarke
 * damienclarke.me | github.com/dxinteractive/CtrlButtonBoard
 *  _____ _    
 * /   | |_)|  
 * \__ | | \|__
 *
 * BUTTON BOARD
 */

// TODO
// rewire switches

#include <Arduino.h>
#include <Wire.h>

// TLC5940
#include "Tlc5940.h"

// SSD1306
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// inputs
#include <AnalogMultiButton.h>
#include <ResponsiveAnalogRead.h>

// pins
const int PIN_3WAYSWITCH_UP = 0;
const int PIN_MOMSWITCH = 1;
const int PIN_3WAYSWITCH_DOWN = 1;
const int PIN_TLC5940_0 = 3;
const int PIN_TLC5940_1 = 4;
const int PIN_TLC5940_2 = 5;
const int PIN_TLC5940_3 = 6;
const int PIN_TLC5940_4 = 7;
const int PIN_CS = 8;
const int PIN_RST = 9;
const int PIN_DC = 10;
const int PIN_SCLK = 11;
const int PIN_SID = 12;
// const int PIN_LED = 13;
const int PIN_ANALOG_7 = A0;
const int PIN_ANALOG_6 = A1;
const int PIN_ANALOG_5 = A2;
const int PIN_ANALOG_4 = A3;
// const int PIN_I2C_SDA = 18;
// const int PIN_I2C_SCL = 19;
const int PIN_ANALOG_3 = A6;
const int PIN_ANALOG_2 = A7;
const int PIN_ANALOG_1 = A8;
const int PIN_ANALOG_0 = A9;
const int PIN_BUTTONSET_0 = A10;
const int PIN_BUTTONSET_1 = A11;

// const int PIN_LED_DOWN = 20;
// const int PIN_LED_RIGHT = 21;
// const int PIN_LED_LEFT = 22;
// const int PIN_LED_UP = 23;

Adafruit_SSD1306 screen(PIN_SID, PIN_SCLK, PIN_DC, PIN_RST, PIN_CS);

ResponsiveAnalogRead analog0(PIN_ANALOG_0, true);
ResponsiveAnalogRead analog1(PIN_ANALOG_1, true);
ResponsiveAnalogRead analog2(PIN_ANALOG_2, true);
ResponsiveAnalogRead analog3(PIN_ANALOG_3, true);
ResponsiveAnalogRead analog4(PIN_ANALOG_4, true);
ResponsiveAnalogRead analog5(PIN_ANALOG_5, true);
ResponsiveAnalogRead analog6(PIN_ANALOG_6, true);
ResponsiveAnalogRead analog7(PIN_ANALOG_7, true);

const int BUTTONS_DEBOUNCE = 20;
const int BUTTONS_RESOLUTION = 960;
const int BUTTONS_TOTAL = 8;
const int BUTTONS_VALUES[BUTTONS_TOTAL] = {0, 130, 270, 450, 550, 680, 800, 870};
AnalogMultiButton buttonSet0(PIN_BUTTONSET_0, BUTTONS_TOTAL, BUTTONS_VALUES, BUTTONS_DEBOUNCE, BUTTONS_RESOLUTION);
AnalogMultiButton buttonSet1(PIN_BUTTONSET_1, BUTTONS_TOTAL, BUTTONS_VALUES, BUTTONS_DEBOUNCE, BUTTONS_RESOLUTION);

void setup() {
  // Wire.begin();
  Serial.begin(9600);

  // leds
  Tlc.init();

  // screen
  screen.begin(SSD1306_SWITCHCAPVCC);
  screen.clearDisplay();
  screen.setTextColor(WHITE);
  screen.setTextSize(2);
  screen.println(":) Hello");
  // screen.println("RRRRRRRRRR");
  // screen.println("RRRRRRRRRR");
  // screen.println("RRRRRRRRRR");
  screen.display();

  // switches
  // pinMode(PIN_MOMSWITCH, INPUT_PULLUP);
  // pinMode(PIN_3WAYSWITCH_UP, INPUT_PULLUP);
  // pinMode(PIN_3WAYSWITCH_DOWN, INPUT_PULLUP);

  // analog
  // analog0.setSnapMultiplier(0.001);
  // analog1.setSnapMultiplier(0.001);
  // analogExp.setSnapMultiplier(0.001);
}

int led = 0;

void loop()
{
  Serial.println(":) ?");
  screen.clearDisplay();
  screen.setCursor(0,0);
  screen.println(":) ?");
  screen.println(millis());
  screen.display();
  // Serial.println(digitalRead(PIN_MOMSWITCH));
  // Serial.println(digitalRead(PIN_3WAYSWITCH_UP));
  // Serial.println(digitalRead(PIN_3WAYSWITCH_DOWN));

  Tlc.clear();
  Tlc.set(led, 0);
  led++;
  if(led > 15) {
    led = 0;
  }
  Tlc.set(led, 700);
  Tlc.update();

  // analog0.update();
  // analog1.update();
  // analog2.update();
  // analog3.update();
  // analog4.update();
  // analog5.update();
  // analog6.update();
  // analog7.update();

  // Serial.println("Analog 0");
  // Serial.println(analog0.getValue());
  // Serial.println("Analog 1");
  // Serial.println(analog1.getValue());
  // Serial.println("Analog 2");
  // Serial.println(analog2.getValue());
  // Serial.println("Analog 3");
  // Serial.println(analog3.getValue());
  // Serial.println("Analog 4");
  // Serial.println(analog4.getValue());
  // Serial.println("Analog 5");
  // Serial.println(analog5.getValue());
  // Serial.println("Analog 6");
  // Serial.println(analog6.getValue());
  // Serial.println("Analog 7");
  // Serial.println(analog7.getValue());

  buttonSet0.update();
  buttonSet1.update();

  if(buttonSet0.isPressed(0)) {
    Serial.println("0.0");
  }
   if(buttonSet0.isPressed(1)) {
    Serial.println("0.1");
  }
   if(buttonSet0.isPressed(2)) {
    Serial.println("0.2");
  }
   if(buttonSet0.isPressed(3)) {
    Serial.println("0.3");
  }
   if(buttonSet0.isPressed(4)) {
    Serial.println("0.4");
  }
   if(buttonSet0.isPressed(5)) {
    Serial.println("0.5");
  }
   if(buttonSet0.isPressed(6)) {
    Serial.println("0.6");
  }
   if(buttonSet0.isPressed(7)) {
    Serial.println("0.7");
  }
  if(buttonSet1.isPressed(0)) {
    Serial.println("1.0");
  }
   if(buttonSet1.isPressed(1)) {
    Serial.println("1.1");
  }
   if(buttonSet1.isPressed(2)) {
    Serial.println("1.2");
  }
   if(buttonSet1.isPressed(3)) {
    Serial.println("1.3");
  }
   if(buttonSet1.isPressed(4)) {
    Serial.println("1.4");
  }
   if(buttonSet1.isPressed(5)) {
    Serial.println("1.5");
  }
   if(buttonSet1.isPressed(6)) {
    Serial.println("1.6");
  }
   if(buttonSet1.isPressed(7)) {
    Serial.println("1.7");
  }

  delay(10);

}

// void loop() {

//   // analog
//   // analog0.update();
//   // analog1.update();
//   // analogExp.update();

//   // int analogValue = 1024 - analog0.getValue();

//   // Serial.println();
//   // Serial.println(analog1.getValue());
//   // Serial.println(analogExp.getValue());

//   // // button
//   // buttons.update();
//   // if(buttons.isPressed(BUTTON_UP)) {
//   //   digitalWrite(PIN_RELAY_0, HIGH);
//   //   digitalWrite(PIN_RELAY_1, HIGH);
//   //   digitalWrite(PIN_RELAY_2, HIGH);
//   //   digitalWrite(PIN_RELAY_3, HIGH);
//   //   digitalWrite(PIN_RELAY_4, HIGH);
//   // } else {
//   //   digitalWrite(PIN_RELAY_0, LOW);
//   //   digitalWrite(PIN_RELAY_1, LOW);
//   //   digitalWrite(PIN_RELAY_2, LOW);
//   //   digitalWrite(PIN_RELAY_3, LOW);
//   //   digitalWrite(PIN_RELAY_4, LOW);
//   // }
//   delay(1);
// }