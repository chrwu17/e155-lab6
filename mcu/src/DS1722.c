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
    delay_millis(TIM15, 10);
    digitalWrite(SPI_CE, PIO_LOW);
    spiSendReceive(0x80);  
    spiSendReceive(0b11100100);
    digitalWrite(SPI_CE, PIO_HIGH);
}

double updateTemperature(uint8_t precision){
    digitalWrite(SPI_CE, PIO_LOW);
    spiSendReceive(0x80);  
    spiSendReceive(precision);
    digitalWrite(SPI_CE, PIO_HIGH);

    uint8_t lsb;
    uint8_t msb;
    uint8_t fracBits;
    if (precision == 0b11101110)
        fracBits = 4;
    else
        fracBits = (precision & 0b11); // RS1:RS0 → 0..3

  
    digitalWrite(SPI_CE, PIO_LOW);
    spiSendReceive(0x01);
    lsb = spiSendReceive(0x00);
    digitalWrite(SPI_CE, PIO_HIGH);

    digitalWrite(SPI_CE, PIO_LOW);
    spiSendReceive(0x02);
    msb = spiSendReceive(0x00);
    digitalWrite(SPI_CE, PIO_HIGH);

    bool negative = (msb >> 7) == 1;
    int8_t whole = negative ? (-128 + (msb & 0x7F)) : (msb & 0x7F);

    double frac = 0.0;
    if (fracBits >= 1 && (lsb & 0x80)) frac += 0.5;
    if (fracBits >= 2 && (lsb & 0x40)) frac += 0.25;
    if (fracBits >= 3 && (lsb & 0x20)) frac += 0.125;
    if (fracBits >= 4 && (lsb & 0x10)) frac += 0.0625;

    if (negative) frac = -frac;

    return (double)whole + frac;
    }

