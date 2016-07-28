#pragma once

#include <Arduino.h>

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project KeyboardIO here

#define TS(X) //Serial.print(micros() );Serial.print("\t");Serial.println(X);

#include <Wire.h>
#include <stdio.h>
#include <math.h>
#include <avr/wdt.h>

#include "KeyboardConfig.h"
#include "keymaps.h"
#include "key_events.h"

extern HARDWARE_IMPLEMENTATION KeyboardHardware;

extern uint8_t primary_keymap;
extern uint8_t temporary_keymap;

#ifndef VERSION
#define VERSION "locally-built"
#endif

