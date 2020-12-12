/***********************************************************************************
* Project: Lab2 - Whiskey Bug Revisited: Pub/Sub using MQTT over WiFi
* Class: CIT324 - Networking for IoT
* Author: Kory Herzinger
*
* File: string-utils.cpp
* Description: Implements helper functions for String conversions.
* Date: 08/21/2020
**********************************************************************************/
#include "string-utils.h"
/***********************************************************************************
* Purpose: Converts a float into a string.
* Arguments: value - The float value to convert.
* Returns: A string object containing the float value.
**********************************************************************************/
String floatToString(float value) {
// encode the float value as a string
char strValue[16];
snprintf(strValue, 16, "%.3f", value);
return String(strValue);
}