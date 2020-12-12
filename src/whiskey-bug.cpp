/***********************************************************************************
* Project: Lab2 - Whiskey Bug Revisited: Pub/Sub using MQTT over WiFi
* Class: CIT324 - Networking for IoT
* Author: Kory Herzinger
*
* File: whiskey-bug.cpp
* Description: Implements helper functions to simulate sensor input from the
* "Whiskey Bug" IoT device.
* Date: 08/21/2020
**********************************************************************************/
#include "whiskey-bug.h"
#include <Arduino.h>
/***********************************************************************************
* Purpose: Default Constructor.
* Arguments: None
* Returns: None
**********************************************************************************/
WhiskeyBug::WhiskeyBug() {
}
/***********************************************************************************
* Purpose: Destructor.
* Arguments: None
* Returns: None
**********************************************************************************/
WhiskeyBug::~WhiskeyBug() {
}
/***********************************************************************************
* Purpose: Simulates sampling a temperature sensor attached to the Whiskey Bug.
* Arguments: None
* Returns: A float representing the temperature of the barrel in degrees
* Celsius.
**********************************************************************************/
float WhiskeyBug::getTemp() {
// 15C to 20C is the ideal whiskey barrel temp :)
int rn = random(1500, 2000);
return (float)(rn / 10.0f);
}
/***********************************************************************************
* Purpose: Simulates sampling a pressure sensor attached to the Whiskey Bug.
* Arguments: None
* Returns: A float representing the pressure of the barrel in kilo-pascals.
**********************************************************************************/
float WhiskeyBug::getPressure() {
// 94200 Pa = 94.2 kPa @ 2000ft altitude
// 101325 Pa = 101.3 kPa @ sea level
int rn = random(94200, 101325);
return (float)(rn / 1000.0f);
}
/***********************************************************************************
* Purpose: Simulates sampling an alcohol sensor attached to the Whiskey Bug.
* Arguments: None
* Returns: A float representing the alcohol content of the whiskey in the
* barrel.
**********************************************************************************/
float WhiskeyBug::getAlcoholContent() {
// linear regression of alcohol content
static float abv = 62.5;
abv = abv - 0.01;
return abv;
}