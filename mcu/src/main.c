/*
File: main.c
Author: Christian Wu
Email: chrwu@g.hmc.edu
Date: 10/17/25
*/

#include "../lib/main.h"

int main(void) {
  configureFlash();
  configureClock();

  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  pinMode(LED_PIN, GPIO_OUTPUT);
  
  initTIM(TIM15);
  
  USART_TypeDef * USART = initUSART(USART1_ID, 125000);

  initSPI(0b111, 0, 1);
  initTemperatureSensor();

  int led_status = 0;
  uint8_t precision = 0b11100100;

  while(1) {
    processWebRequest(USART, &precision, &led_status);
  }
}