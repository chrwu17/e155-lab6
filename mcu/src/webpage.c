// Christian Wu
// chrwu@g.hmc.edu
// 10/21/25

// Source code for webpage handling

#include "../lib/webpage.h"
#include "../lib/STM32L432KC.h"
#include "../lib/STM32L432KC_GPIO.h"
#include "../lib/STM32L432KC_USART.h"
#include "../lib/DS1722.h"
#include "../lib/main.h"
#include <string.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Webpage HTML strings
///////////////////////////////////////////////////////////////////////////////

char* webpageStart = "<!DOCTYPE html><html><head><title>E155 Web Server Demo Webpage</title>\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	</head>\
	<body><h1>E155 Web Server Demo Webpage</h1>";

char* ledStr = "<p>LED Control:</p><form action=\"ledon\"><input type=\"submit\" value=\"Turn the LED on!\"></form>\
	<form action=\"ledoff\"><input type=\"submit\" value=\"Turn the LED off!\"></form>";

char* tempStr =
"<p>Temperature Precision Control:</p>"
"<form action=\"8bit\"><input type=\"submit\" value=\"8 Bit Resolution\"></form>"
"<form action=\"9bit\"><input type=\"submit\" value=\"9 Bit Resolution\"></form>"
"<form action=\"10bit\"><input type=\"submit\" value=\"10 Bit Resolution\"></form>"
"<form action=\"11bit\"><input type=\"submit\" value=\"11 Bit Resolution\"></form>"
"<form action=\"12bit\"><input type=\"submit\" value=\"12 Bit Resolution\"></form>";

char* webpageEnd = "</body></html>";

///////////////////////////////////////////////////////////////////////////////
// Helper Functions
///////////////////////////////////////////////////////////////////////////////

int inString(char request[], char des[]) {
	if (strstr(request, des) != NULL) {return 1;}
	return -1;
}
int updateLEDStatus(char request[], int current_status)
{
	if (inString(request, "ledoff")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		return 0;
	}
	else if (inString(request, "ledon")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		return 1;
	}
	return current_status;
}

///////////////////////////////////////////////////////////////////////////////
// Main Web Request Processing Function
///////////////////////////////////////////////////////////////////////////////

void processWebRequest(USART_TypeDef * USART, uint8_t *precision, int *led_status)
{
	// Receive web request from the ESP
	char request[BUFF_LEN] = "                  ";
	int charIndex = 0;

	// Keep going until you get end of line character
	while(inString(request, "\n") == -1) {
		// Wait for a complete request to be transmitted before processing
		while(!(USART->ISR & USART_ISR_RXNE));
		request[charIndex++] = readChar(USART);
	}

	// Check for precision change requests
	if (inString(request, "8bit")==1) {
		*precision = 0b11100000;
	}
	else if (inString(request, "9bit")==1) {
		*precision = 0b11100010;
	}
	else if (inString(request, "10bit")==1) {
		*precision = 0b11100100;
	}
	else if (inString(request, "11bit")==1) {
		*precision = 0b11100110;
	}
	else if (inString(request, "12bit")==1) {
		*precision = 0b11101110;
	}

	// Read temperature with current precision
	double temp = updateTemperature(*precision);

	char tempStatusStr[32];
	sprintf(tempStatusStr, "Temperature %.4f C", temp);

	// Update LED status
	*led_status = updateLEDStatus(request, *led_status);

	char ledStatusStr[20];
	if (*led_status == 1)
		sprintf(ledStatusStr,"LED is on!");
	else if (*led_status == 0)
		sprintf(ledStatusStr,"LED is off!");

	// Transmit the webpage over UART
	sendString(USART, webpageStart);
	sendString(USART, ledStr);
	sendString(USART, tempStr);

	sendString(USART, "<h2>LED Status</h2>");
	sendString(USART, "<p>");
	sendString(USART, ledStatusStr);
	sendString(USART, "</p>");

	sendString(USART, "<h2>Temperature </h2>");
	sendString(USART, "<p>");
	sendString(USART, tempStatusStr);
	sendString(USART, "</p>");

	sendString(USART, webpageEnd);
}