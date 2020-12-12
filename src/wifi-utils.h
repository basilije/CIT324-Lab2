/***********************************************************************************
* Project: Lab2 - Connecting to Wifi
* Class: CIT324 - Networking for IoT
* Author: Kory Herzinger
*
* File: wifi-utils.h
* Description: Defines wifi helper functions.
* Date: 08/19/2020
**********************************************************************************/
#pragma once
#include <WiFi.h>
String wifiAuthModeToString(wifi_auth_mode_t mode);
String wifiStatusToString(wl_status_t status);
String macAddressToString(byte mac[]);