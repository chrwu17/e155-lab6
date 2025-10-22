// Christian Wu
// chrwu@g.hmc.edu
// 10/17/25

// DS1722.c
// Source code for DS1722 functions

#include "../lib/DS1722.h"
#include "../lib/STM32L432KC.h"
#include "../lib/STM32L432KC_GPIO.h"
#include "../lib/STM32L432KC_SPI.h"
#include "../lib/STM32L432KC_TIM.h"
#include <stdint.h>
#include <stdbool.h>

void initTemperatureSensor(){
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x80);  
    spiSendReceive(0b11100100);
    digitalWrite(SPI_CE, PIO_LOW);
}

double updateTemperature(uint8_t precision){
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x80);  
    spiSendReceive(precision);
    digitalWrite(SPI_CE, PIO_LOW);

    uint8_t lsb;
    uint8_t msb;
    uint8_t fracBits;
    if (precision == 0b11100000)       // 8-bit resolution
        fracBits = 0;
    else if (precision == 0b11100010)  // 9-bit resolution
        fracBits = 1;
    else if (precision == 0b11100100)  // 10-bit resolution
        fracBits = 2;
    else if (precision == 0b11100110)  // 11-bit resolution
        fracBits = 3;
    else if (precision == 0b11101110)  // 12-bit resolution
        fracBits = 4;
    else
        fracBits = 4;  // default to 12-bit


    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x01);
    lsb = spiSendReceive(0x00);
    digitalWrite(SPI_CE, PIO_LOW);

    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x02);
    msb = spiSendReceive(0x00);
    digitalWrite(SPI_CE, PIO_LOW);

    bool negative = (msb & 0x80);  
    int8_t whole = (int8_t)msb;   

    
    double frac = 0.0;
    if (fracBits >= 1 && (lsb & 0x80)) frac += 0.5;
    if (fracBits >= 2 && (lsb & 0x40)) frac += 0.25;
    if (fracBits >= 3 && (lsb & 0x20)) frac += 0.125;
    if (fracBits >= 4 && (lsb & 0x10)) frac += 0.0625;

    if (negative) {
        return (double)whole - frac;  
    } else {
        return (double)whole + frac; 
    }
    }

