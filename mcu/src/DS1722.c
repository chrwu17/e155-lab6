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

void initTemperatureSensor(){
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x80);  
    spiSendReceive(0b11100100);
    digitalWrite(SPI_CE, 0);
}

float updateTemperature(char request[]){
    uint8_t lsb;
    uint8_t msb;

    if (inString(request, "8bit")==1) {
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x80);  
    spiSendReceive(0b11100000);
    digitalWrite(SPI_CE, PIO_LOW);
  }
  else if (inString(request, "9bit")==1) {
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x80);  
    spiSendReceive(0b11100010);
    digitalWrite(SPI_CE, PIO_LOW);
  }
  else if (inString(request, "10bit")==1) {
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x80);  
    spiSendReceive(0b11100100);
    digitalWrite(SPI_CE, PIO_LOW);
  }
  else if (inString(request, "11bit")==1) {
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x80);  
    spiSendReceive(0b11100110);
    digitalWrite(SPI_CE, PIO_LOW);
  }
  else if (inString(request, "12bit")==1) {
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x80);  
    spiSendReceive(0b11101110);
    digitalWrite(SPI_CE, PIO_LOW);
  }
  
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x01);
    lsb = spiSendReceive(0x45);
    digitalWrite(SPI_CE, PIO_LOW);

    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(0x02);
    msb = spiSendReceive(0x44);
    digitalWrite(SPI_CE, PIO_LOW);

    float temperature = msb + (lsb / 256.0);

    return temperature;
}
