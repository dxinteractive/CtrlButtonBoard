/*
 * Main program file for CtrlButtonBoard 
 * external controller for the Ctrl pedal
 * (CV generator / metronome / tuner).
 * Copyright (c) 2017-2021 Damien Clarke
 * damienclarke.me | github.com/dxinteractive/CtrlButtonBoard
 *  _____ _    
 * /   | |_)|  
 * \__ | | \|__
 *
 * BUTTON BOARD
 */

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
#define PIN_3WAYSWITCH_UP   0
#define PIN_MOMSWITCH       1
#define PIN_3WAYSWITCH_DOWN 2
#define PIN_TLC5940_0       3
#define PIN_TLC5940_1       4
#define PIN_TLC5940_2       5
#define PIN_TLC5940_3       6
#define PIN_TLC5940_4       7
#define PIN_CS              8
#define PIN_RST             9
#define PIN_DC              10
#define PIN_SCLK            11
#define PIN_SID             12
#define PIN_LED             13
#define PIN_ANALOG_7        A0
#define PIN_ANALOG_6        A1
#define PIN_ANALOG_5        A2
#define PIN_ANALOG_4        A3
// #define PIN_I2C_SDA      18
// #define PIN_I2C_SCL      19
#define PIN_ANALOG_3        A6
#define PIN_ANALOG_2        A7
#define PIN_ANALOG_1        A8
#define PIN_ANALOG_0        A9
#define PIN_BUTTONSET_0     A10
#define PIN_BUTTONSET_1     A11

#define BB_I2C_CHANNEL      8
#define WIRE_RECEIVE_MAX    10

#define SET_0LED            7
#define SET_LED_UPPER       8
#define SET_LED_LOWER       9

Adafruit_SSD1306 screen(PIN_SID, PIN_SCLK, PIN_DC, PIN_RST, PIN_CS);

ResponsiveAnalogRead analog0(PIN_ANALOG_0, true);
ResponsiveAnalogRead analog1(PIN_ANALOG_1, true);
ResponsiveAnalogRead analog2(PIN_ANALOG_2, true);
ResponsiveAnalogRead analog3(PIN_ANALOG_3, true);
ResponsiveAnalogRead analog4(PIN_ANALOG_4, true);
ResponsiveAnalogRead analog5(PIN_ANALOG_5, true);
ResponsiveAnalogRead analog6(PIN_ANALOG_6, true);
ResponsiveAnalogRead analog7(PIN_ANALOG_7, true);

const int BUTTONS_DEBOUNCE = 1;
const int BUTTONS_RESOLUTION = 960;
const int BUTTONS_PER_SET = 8;
const int BUTTONS_VALUES[BUTTONS_PER_SET] = {0, 130, 270, 450, 550, 680, 800, 870};

const int BUTTONS_TOTAL = 16;
const int BUTTONS_NUMBERS[BUTTONS_TOTAL] = {0,0,1,1,2,2,3,3,7,7,6,6,5,5,4,4};
const int BUTTONS_NUMBERS_SETS[BUTTONS_TOTAL] = {0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0};

AnalogMultiButton buttonSet0(PIN_BUTTONSET_0, BUTTONS_PER_SET, BUTTONS_VALUES, BUTTONS_DEBOUNCE, BUTTONS_RESOLUTION);
AnalogMultiButton buttonSet1(PIN_BUTTONSET_1, BUTTONS_PER_SET, BUTTONS_VALUES, BUTTONS_DEBOUNCE, BUTTONS_RESOLUTION);

//
// LEDs
//

void setLed(int led, int brightness) {
  if(led == 16) {
    analogWrite(PIN_LED, brightness);
    return;
  }
  int tlcLed = led < 8 ? 7 - led : 15 - led;
  Tlc.set(tlcLed, brightness * 2);
  Tlc.update();
}

//
// Wire send
//

// int wireSendMemory[SEND_TOTAL] = {};

// void wireSend(int id, int value) {
//   if(value == wireSendMemory[id]) return;
//   Serial.print(id);
//   Serial.print(':');
//   Serial.println(value);
//   wireSendMemory[id] = value;
// }

//
// Wire response
//

void wireReceive(const char * data) {
  int target = data[0] - '0';
  int subtarget = data[1] - '0';

  const char * valueString = data + 3;

  switch(target) {
    case SET_LED_UPPER:
      if(subtarget == 8) {
        subtarget = 16;
      }
      setLed(subtarget, atoi(valueString));
      break;

    case SET_LED_LOWER:
      setLed(subtarget + 8, atoi(valueString));
      break;

    default:
      break;
  }
}

void wireReceiveByte(const byte inByte) {
  static char inputLine[WIRE_RECEIVE_MAX];
  static unsigned int inputPos = 0;

  switch(inByte) {
    case 0:
    case '\n':
      inputLine[inputPos] = 0;
      wireReceive(inputLine);
      inputPos = 0;
      break;

    case '\r':
      break;

    default:
      if(inputPos < (WIRE_RECEIVE_MAX - 1)) {
        inputLine[inputPos++] = inByte;
      }
      break;
  }
}

void receiveWireEvent(int howMany) {
  while(Wire.available() > 0) {
    wireReceiveByte(Wire.read());
  }
}

void requestWireEvent() {
  Wire.write("hello ");
}

//
// Setup
//

void setup() {
  Wire.begin(BB_I2C_CHANNEL);
  Wire.onReceive(receiveWireEvent);
  Wire.onRequest(requestWireEvent);

  // leds
  Tlc.init();
  Tlc.clear();
  Tlc.update();

  // LEDs
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // screen
  screen.begin(SSD1306_SWITCHCAPVCC);
  screen.clearDisplay();
  screen.setTextColor(WHITE);
  screen.setTextSize(2);
  screen.println(":) Hello");
  screen.display();

  // switches
  pinMode(PIN_MOMSWITCH, INPUT_PULLUP);
  pinMode(PIN_3WAYSWITCH_UP, INPUT_PULLUP);
  pinMode(PIN_3WAYSWITCH_DOWN, INPUT_PULLUP);
}

int led = 0;
int activeButton = -1;

unsigned long time;
unsigned long prevTime;
unsigned long ticks;

void loop() {
  time = millis();
  if(time == prevTime) return;

  // main loop once per 1ms = 1000Hz
  prevTime = time;
  ticks++;

  // once per 8ms = ~125fps
  if(ticks % 8 == 0) {

  }

  screen.clearDisplay();
  screen.setCursor(0,0);
  screen.println(":) ?");
  screen.setTextSize(1);

  screen.println(ticks);

  screen.print(digitalRead(PIN_MOMSWITCH) == LOW);
  screen.print(digitalRead(PIN_3WAYSWITCH_UP) == LOW);
  screen.println(digitalRead(PIN_3WAYSWITCH_DOWN) == LOW);

  analog0.update();
  analog1.update();
  analog2.update();
  analog3.update();
  analog4.update();
  analog5.update();
  analog6.update();
  analog7.update();

  // screen.print(analog0.getValue());
  // screen.print(analog1.getValue());
  // screen.print(analog2.getValue());
  // screen.print(analog3.getValue());
  // screen.print(analog4.getValue());
  // screen.print(analog5.getValue());
  // screen.print(analog6.getValue());
  // screen.println(analog7.getValue());

  analogRead(PIN_BUTTONSET_0);
  buttonSet0.update();

  analogRead(PIN_BUTTONSET_1);
  buttonSet1.update();

  activeButton = -1;
  for(int i = 0; i < BUTTONS_TOTAL; i++) {
    if(BUTTONS_NUMBERS_SETS[i]
        ? buttonSet1.isPressed(BUTTONS_NUMBERS[i])
        : buttonSet0.isPressed(BUTTONS_NUMBERS[i])
    ) {
      activeButton = i;
      break;
    }
  }

  screen.print("activeButton: ");
  screen.println(activeButton);

  screen.display();
  delay(10);

}