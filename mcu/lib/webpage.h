// Christian Wu
// chrwu@g.hmc.edu
// 10/21/25

// Header for webpage handling functions

#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <stdint.h>
#include <stm32l432xx.h>

///////////////////////////////////////////////////////////////////////////////
// External webpage string declarations
///////////////////////////////////////////////////////////////////////////////

extern char* webpageStart;
extern char* ledStr;
extern char* tempStr;
extern char* webpageEnd;

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

int inString(char request[], char des[]);
int updateLEDStatus(char request[], int current_status);
void processWebRequest(USART_TypeDef * USART, uint8_t *precision, int *led_status);

#endif