// Christian Wu 
// chrwu@g.hmc.edu
// 10/17/2025

// Header for DS1722 functions

#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

void initTemperatureSensor(void);

float updateTemperature(char request[]);

int inString(char request[], char des[]);

#endif