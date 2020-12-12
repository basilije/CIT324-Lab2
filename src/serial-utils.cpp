/***********************************************************************************
* Project: Lab2 - Connecting to Wifi
* Class: CIT324 - Networking for IoT
* Author: Kory Herzinger
*
* File: serial-utils.cpp
* Description: Implements serial port helper functions.
* Date: 08/19/2020
**********************************************************************************/
#include "serial-utils.h"

/***********************************************************************************
* Purpose: Flushes any characters waiting in the serial port input buffer. This
* is used especially for handling CRLF from pressing the enter key
* when reading a string from the serial port.
* Arguments: None
* Returns: None
**********************************************************************************/
void flushSerialInputBuffer() {
	// get the number of characters waiting in the serial input buffer
	int charsInBuffer = Serial.available();
	// flush the input buffer by reading characters that are waiting
	while (charsInBuffer > 0) {
		Serial.read();
		charsInBuffer--;
		}
}
/***********************************************************************************
* Purpose: Prompts for input and reads in characters from the serial port into
* a string until the enter key is sent.
* Arguments: const char* prompt - The prompt message to display on the serial
* port.
* int maxChars - The max number of characters to read into the
* string
* Returns: The string that was sent across the serial port (otensibly, entered
* by a user at the terminal).
**********************************************************************************/
String serialPrompt(const char* prompt, int maxChars) {
	char str[maxChars + 1];
	int idx = 0;
	// display the prompt message
	Serial.print(prompt);
	// read in characters to the string up to maxChars
	while (idx < maxChars) {
		if (Serial.available() > 0) {
			// read in the byte and append the NULL terminator
			Serial.readBytes(&str[idx], 1);
			str[idx+1] = '\0';
			// check if the enter key was pressed
			if (str[idx] == '\r' || str[idx] == '\n') {
				// the enter key was pressed, so we're done... remove the last character
				// (either a carriage-return or newline) from the string and exit the loop
				str[idx] = '\0';
				break;
				}
			else {
				// output the character to the serial port
				Serial.print(String(str[idx]));

				// increment the index in the array
				idx = ((idx < maxChars) ? (idx + 1) : idx);
			}
		}
		else {
		// wait for a byte to be sent across the serial port
		delay(10);
		}
	}
	// flush the serial input buffer since the enter key is often
	// represented by both a carriage-return ('\r') and a newline
	// ('\n').
	flushSerialInputBuffer();
	// print a newline
	Serial.println("");
	// return the string that was sent across the serial port
	return String(str);
}
