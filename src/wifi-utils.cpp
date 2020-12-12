/***********************************************************************************
* Project: Lab2 - Connecting to Wifi
* Class: CIT324 - Networking for IoT
* Author: Kory Herzinger
*
* File: wifi-utils.cpp
* Description: Implements wifi helper functions.
* Date: 08/19/2020
**********************************************************************************/
#include "wifi-utils.h"

/***********************************************************************************
* Purpose: Returns a string representation of a wifi auth mode.
* Arguments: wifi_auth_mode_t mode - A wifi auth mode enum value.
* Returns: A string representing the wifi auth mode.
**********************************************************************************/
String wifiAuthModeToString(wifi_auth_mode_t mode) {
	switch(mode) {
		case WIFI_AUTH_WEP: return "WEP";
		case WIFI_AUTH_WPA_PSK: return "WPA";
		case WIFI_AUTH_WPA2_PSK: return "WPA2";
		case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
		case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2 Enterprise";
		case WIFI_AUTH_OPEN: return "None";
		default: return "Unknown";
	}
}
/***********************************************************************************
* Purpose: Returns a string representation of the wifi status.
* Arguments: wl_status_t status - A wifi status enum value.
* Returns: A string representing the wifi status.
**********************************************************************************/
String wifiStatusToString(wl_status_t status) {
	switch (status) {
	case WL_CONNECT_FAILED: return "Connection failed";
	case WL_CONNECTED: return "Connected";
	case WL_CONNECTION_LOST: return "Connection lost";
	case WL_DISCONNECTED: case WL_IDLE_STATUS: return "Disconnected";
	default: return "Unknown";
	}
}
/***********************************************************************************
* Purpose: Converts a MAC address in a byte array to a string.
* Arguments: byte mac[] - A byte array containing the 6 octets of the MAC addr.
* Returns: A string representing the MAC address.
**********************************************************************************/
String macAddressToString(byte mac[])
{
	return
	String(mac[0], HEX) + ":" +
	String(mac[1], HEX) + ":" +
	String(mac[2], HEX) + ":" +
	String(mac[3], HEX) + ":" +
	String(mac[4], HEX) + ":" +
	String(mac[5], HEX);
}
