/************************************
    Ws2812  rgb serial led driver

    Copyright 20201 Pierre HENRY net23@frdev.com

    Ws2812 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ws2812 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with betaEvents.  If not, see <https://www.gnu.org/licenses/lglp.txt>.


    A reset is issued as early as at 9 µs???, contrary to the 50 µs mentioned in the data sheet. Longer delays between transmissions should be avoided.
    On tested componant 50µs reset is a mandatory
    The cycle time of a bit should be at least 1.25 µs, the value given in the data sheet, and at most ~50 µs, the shortest time for a reset.
    A “0” can be encoded with a pulse as short as 62.5 ns, but should not be longer than ~500 ns (maximum on WS2812).
    A “1” can be encoded with pulses almost as long as the total cycle time, but it should not be shorter than ~625 ns (minimum on WS2812B).
    from https://cpldcpu.wordpress.com/2014/01/14/light_ws2812-library-v2-0-part-i-understanding-the-ws2812/

    https://roboticsbackend.com/arduino-fast-digitalwrite/



*/


#include "WS2812.h"
// 10 µsec pulse

#define MSK_WS2812 (1 << (PIN_WS2812-8))  //N° pin portB donc D8..D13 sur nano
#define PORT_WS2812 PORTB

inline  void WS2812_LOW() __attribute__((always_inline));
inline  void WS2812_HIGH() __attribute__((always_inline));


void WS2812_LOW() {
  PORT_WS2812 &= ~MSK_WS2812;
}

void WS2812_HIGH() {
  PORT_WS2812 |= MSK_WS2812;
}


void  WS2812rvb_t::reset() {
  interrupts();
  WS2812_LOW();
  //delayMicroseconds(10);
}

//   Arduino Nano
//   Cycle = 1,7µs
//    0 = 0,3µs + 1,4µs
//    1 = 0,95µs + 0,75µs

//timing for nano
void WS2812rvb_t::shift( uint8_t shift) {
  static uint16_t delay1 = 0;

  for (byte n = 8; n > 0; n--, shift = shift << 1) {
    if (shift & 0x80)  {
      WS2812_HIGH();  //0,3µs
      delay1++;       //0,65µs
      WS2812_LOW();
    } else {
      WS2812_HIGH();  //0,3µs
      WS2812_LOW();
      delay1++;
    }
  }

}


void  WS2812rvb_t::write() {
  noInterrupts();
  this->shift(this->green);
  this->shift(this->red);
  this->shift(this->blue);
}



void  rvbLed::setcolor( const e_rvb acolor, const uint8_t alevel, const uint16_t increase , const  uint16_t decrease )  {
  //void  rvb_t::setcolor( e_rvb color,  uint8_t level,  uint16_t steady ,  uint16_t decrease );
  this->maxLevel = alevel;
  this->color = acolor;
  this->red =   0;
  this->green = 0;
  this->blue =  0;
  if (increase == 0) {
    this->red =   (uint16_t)map_color[this->color].red * alevel / 100;
    this->green = (uint16_t)map_color[this->color].green * alevel / 100;
    this->blue =  (uint16_t)map_color[this->color].blue * alevel / 100; 
  }
  this->baseIncDelay = increase;
  this->incDelay = increase;
  this->baseDecDelay = decrease;
  this->decDelay = decrease;
}


void  rvbLed::anime(const uint8_t delta) {
  if (incDelay > 0) {
    if (incDelay > delta) {
      incDelay -= delta;
    } else {
      incDelay = 0;
    }
    // increment
    //uint16_t curLevel = (uint16_t)maxLevel - ( (uint32_t)maxLevel * incDelay / baseIncDelay );
    // decrem
    uint8_t curLevel = (uint32_t)maxLevel * decDelay / baseDecDelay;
    this->red =   (uint16_t)map_color[color].red * curLevel / 100;
    this->green = (uint16_t)map_color[color].green * curLevel / 100;
    this->blue =  (uint16_t)map_color[color].blue * curLevel / 100;
    return;
  }

  if (decDelay > 0) {
    if (decDelay > delta) {
      decDelay -= delta;
    } else {
      decDelay = 0;
    }
    // increment
    uint16_t curLevel = (uint16_t)maxLevel - ( (uint32_t)maxLevel * incDelay / baseIncDelay );
    // decrem
    //uint8_t curLevel = (uint32_t)maxLevel * decDelay / baseDecDelay;
    this->red =   (uint16_t)map_color[color].red * curLevel / 100;
    this->green = (uint16_t)map_color[color].green * curLevel / 100;
    this->blue =  (uint16_t)map_color[color].blue * curLevel / 100;
    return;
  }
}
