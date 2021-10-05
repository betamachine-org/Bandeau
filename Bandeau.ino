/*
  Bandeau

    Gestion d'un bandeau WS2812 simplifié

    Copyright 20201 Pierre HENRY net23@frdev.com 

    BetaPorteV2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    BetaPorteV2 is distributed in the hope that it will be useful,
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

const uint8_t ledsMAX = 9;
WS2812rvb_t leds[ledsMAX];

uint8_t delayModeOff = 5;
enum mode_t { modeOff, modeSearch, modeGood, modeBad}  displayMode = modeSearch;
uint8_t displayStep = 0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_LIFE, OUTPUT);
  pinMode(pinBP0, INPUT_PULLUP);
  for (uint8_t N = 0; N < ledsMAX; N++) {
    leds[N].setcolor(rvb_white, 80, 1000, 1000);
  }
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
      divAnime = 25;

      // animation
      if (displayStep < ledsMAX) {
        switch (displayMode) {
          case modeOff:
            leds[displayStep].setcolor(rvb_black, 50);
            break;
          case modeSearch:
            leds[displayStep].setcolor(rvb_lightblue, 100, 1000, 1000);
            break;
          case modeGood:
            leds[displayStep].setcolor(rvb_green, 80,  400, 400);
            break;
          case modeBad:
            leds[displayStep].setcolor(rvb_orange, 80, 1400, 1400);
            break;
        }
      }
      displayStep = (displayStep + 1) % (ledsMAX + 6);

    }
  }
  //delay(1);
}



void jobPoussoir() {
  if ( (digitalRead(pinBP0) == LOW) != bp0Stat ) {
    bp0Stat = !bp0Stat;
    if (bp0Stat) {
      displayMode = (mode_t)( (displayMode + 1) % 4 );
      delayModeOff = 30;
      displayStep = 0;
    }


  }
}


// 110 HZ
void jobRefreshLeds(const uint8_t delta) {
  leds[0].write();
  leds[1].write();
  leds[2].write();
  leds[3].write();
  leds[4].write();
  leds[5].write();
  leds[6].write();
  leds[7].write();
  leds[8].write();
  leds[0].reset(); // obligatoire

  leds[0].anime(delta);
  leds[1].anime(delta);
  leds[2].anime(delta);
  leds[3].anime(delta);
  leds[4].anime(delta);
  leds[5].anime(delta);
  leds[6].anime(delta);
  leds[7].anime(delta);
  leds[8].anime(delta);

}
