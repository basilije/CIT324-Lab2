/***********************************************************************************
* Project: Lab2 - Whiskey Bug Revisited: Pub/Sub using MQTT over WiFi
* Class: CIT324 - Networking for IoT
* Author: Kory Herzinger
*
* File: whiskey-bug.h
* Description: Defines helper functions to simulate sensor input from the
* "Whiskey Bug" IoT device.
* Date: 08/21/2020
**********************************************************************************/
#pragma once
class WhiskeyBug {
public:
WhiskeyBug();
~WhiskeyBug();
// Simulates sampling a temperature sensor attached to the Whiskey Bug.
float getTemp();
// Simulates sampling a pressure sensor attached to the Whiskey Bug.
float getPressure();
// Simulates sampling an alcohol sensor attached to the Whiskey Bug.
float getAlcoholContent();
};