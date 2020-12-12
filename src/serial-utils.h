/***********************************************************************************
* Project: Lab2 - Connecting to Wifi
* Class: CIT324 - Networking for IoT
* Author: Kory Herzinger
*
* File: serial-utils.h
* Description: Defines serial port helper functions.
* Date: 08/19/2020
**********************************************************************************/
#pragma once
#include <Arduino.h>
void flushSerialInputBuffer();
String serialPrompt(const char* prompt, int maxChars);