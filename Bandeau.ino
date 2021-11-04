/*
  Bandeau

   Gestion d'un bandeau WS2812 simplifié

   Copyright 20201 Pierre HENRY net23@frdev.com

   Bandeau is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Bandeau is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with betaEvents.  If not, see <https://www.gnu.org/licenses/lglp.txt>.


  History
   V1.0 (30/10/2021)
   - Full rebuild from Triangle et TinyTriangle  01/2021 net234

*************************************************/


#include <Arduino.h>
#include "WS2812.h"

uint8_t div10Hz = 10;
uint8_t div1Hz = 10;
uint8_t divAnime = 250;

// varibale modifiables
const uint8_t  ledsMAX = 16;  // nombre de led sur le bandeau
const uint16_t autoOffDelay = 5;   // delais d'auto extinction en secondes (0 = pas d'autoextinction)
// varibale modifiables (fin)

WS2812rvb_t leds[ledsMAX + 1];

uint16_t delayModeOff = autoOffDelay;   
enum mode_t { modeOff, modeUn, modeDeux, modeTrois, MAXmode}  displayMode = modeUn;
uint8_t displayStep = 0;

#include <EEPROM.h>


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_LIFE, OUTPUT);
  pinMode(pinBP0, INPUT_PULLUP);
  pinMode(PIN_WS2812, OUTPUT);
  for (uint8_t N = 0; N < ledsMAX; N++) {
    leds[N].setcolor(rvb_white, 80, 1000, 1000);
  }

  // lecture de  l'EEPROM pour le choix de l'animation
  // check if a stored value
  if (EEPROM.read(1) == 'B' && EEPROM.read(2) == 'e') ;
  displayMode = EEPROM.read(2);
  if (displayMode >= MAXmode ) displayMode = modeUn;
  EEPROM.write(1, 'B');
  EEPROM.write(2, displayMode);
}


uint32_t milli1 = millis();  // heure du dernier 100Hz obtenus
bool ledLifeStat = true;
bool bp0Stat = false;

// the loop function runs over and over again forever
void loop() {

  // detection de 10milliseconde pour btenir du 100Hz
  uint16_t delta = millis() - milli1;
  if (  delta >= 10 ) {
    milli1 += 10;

    // 100 Hzt rafraichissement bandeau
    jobRefreshLeds(10);

    // diviseur pour avoir du 10Hz
    if (--div10Hz == 0) {
      // 10 Hzt
      div10Hz = 10;

      //10HZ test poussoir
      jobPoussoir();

      // diviseur pour avoir du 1HZ
      if (--div1Hz == 0) {
        div1Hz = 10;

        // job 1 Hz
        ledLifeStat = !ledLifeStat;
        digitalWrite(LED_LIFE, ledLifeStat);   // turn the LED on (HIGH is the voltage level)

        if (delayModeOff) {
          if (--delayModeOff == 0) {
            displayMode = modeOff;
          }
        }


      }  // 1Hz
    } // 10Hz


    // annimation toute les 25 millisec
    if (--divAnime == 0) {
      divAnime = 20;

      // animation
      if (displayStep < ledsMAX) {
        switch (displayMode) {
          case modeOff:
            leds[displayStep].setcolor(rvb_black, 50);
            break;
          case modeUn:
            leds[displayStep].setcolor(rvb_red, 80, 1000, 100);
            break;
          case modeDeux:
            leds[displayStep].setcolor(rvb_blue, 80, 1000, 100);
            break;
          case modeTrois:
            int couleur = (displayStep % 3) + rvb_blouge1;
            leds[displayStep].setcolor(couleur, 80, 1000, 100);
            break;
        }
      }
      displayStep = (displayStep + 1) % (ledsMAX);

    }
  } // 100Hz

  //delay(1);
}



void jobPoussoir() {
  if ( (digitalRead(pinBP0) == LOW) != bp0Stat ) {
    bp0Stat = !bp0Stat;
    if (bp0Stat) {
      displayMode = (mode_t)( (displayMode + 1) % 4 );
      delayModeOff = autoOffDelay;
      displayStep = 0;
    }


  }
}


// 110 HZ
void jobRefreshLeds(const uint8_t delta) {
  for (int8_t N = 0; N < ledsMAX; N++) {
    leds[N].write();
  }
//  option mode mirroir  
//  for (int8_t N = ledsMAX - 1; N > 0; N--) {
//    leds[N].write();
//  }
  leds[0].reset(); // obligatoire

  for (uint8_t N = 0; N < ledsMAX; N++) {
    leds[N].anime(delta);
  }

}
