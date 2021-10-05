#pragma once

#define __BOARD__NANO RF__
#define D_println(x) Serial.print(F(#x " => '")); Serial.print(x); Serial.println("'");

//   Nano RF
//#define TX1  31   //!TXD        17  //!RXLED
//#define RX0  30   //!RXD        30  //!TXLED
#define D2     2 //!HI2C_SDA   WS812
#define D3     3 //!HI2C_SCL
#define D4     4 // 
#define D5     5 //
#define D6     6 //
#define D7     7 //           BP1  
#define D8     8 //PB0            Bandeau de led (PIN_WS2812) 
#define D9     9 //PB1 !RF24_CSN
#define D10    10 //PB2 !RF24_CE
#define D11    11 //PB3 !SPI_MOSI
#define D12    12 //PB4 !SPI_MISO 
#define D13    13 //PB5 !SPI_CLK   !LED_BUITIN
//---------------------

// affectation des pattes pour bandeau
#define LED_LIFE  LED_BUILTIN
#define LED_ON  HIGH

#define pinBP0 D7
#define PIN_WS2812 D8  //Uniquement D8..D13
//---------------------


//#define LED_LIFE  D13
//#define BP0       D7
//#define LED_ON  HIGH
//#define PIN_WS2812 PB2
//
//#define LED_LIFE  D13
//#define LED_ON  HIGH
//
//#define pinBP0 8 // D2
//#define PIN_WS2812 5  //2 // D8
