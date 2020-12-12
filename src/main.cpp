/***********************************************************************************
* Project: Lab2
* Class: CIT324 - Networking for IoT
* Author: Vasilije Mehandzic
*
* File: main.cpp
* Description: Main file after Exercises #1, #2, #3
* Date: 12/12/2020
**********************************************************************************/

#include <Arduino.h>
#include "serial-utils.h"
#include "wifi-utils.h"
#include "string-utils.h"
#include <PubSubClient.h>
#include "whiskey-bug.h"
// for disable brownout detector https://github.com/espressif/arduino-esp32/issues/863
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// operation modes enum
enum operation_type { 
    OPERATION_TYPE_NORMAL,
    OPERATION_TYPE_UDP_BROADCAST,
    OPERATION_TYPE_MQTT_MODE,
};

const unsigned int UDP_PORT = 8888;
const unsigned int UDP_PACKET_SIZE = 64;
const char* P_UDP_MESSAGE = "^^^^^^<| DoN'T MoVE DoN'T SToP {<waka waka waka waka>} |>^^^^^^";
const char* MQTT_BROKER_SERVER = "maqiatto.com";
const uint16_t MQTT_BROKER_PORT = 1883;
const char* MQTT_BROKER_CLIENT_ID = "WhiskeyBug";
const char* MQTT_TEMPERATURE_TOPIC = "vasske@gmail.com/Temperature";
const char* MQTT_PRESSURE_TOPIC = "vasske@gmail.com/Pressure";
const char* MQTT_ALCOHOL_TOPIC = "vasske@gmail.com/AlcoholContent";
const char MQTT_BROKER_USERNAME[] = "vasske@gmail.com";
const char MQTT_BROKER_PASSWORD[] = "emkjutitiPASS";

WiFiUDP Udp;
IPAddress remote_ip;
int serial_read, incoming_byte, num_ssid, key_index = 0, current_mode_of_operation = OPERATION_TYPE_NORMAL;  // network key Index number

String key_pressed;
byte mac[6];
wl_status_t status = WL_IDLE_STATUS;  // the Wifi radio's status
time_t seconds = time(NULL);

char ch_temp[16], ch_pres[16], ch_alco[16];
WhiskeyBug whiskey_bug;
WiFiClient espClient;
PubSubClient client(espClient);

/***********************************************************************************
* Purpose: Print the main menu content.
* No arguments, no returns
**********************************************************************************/
void printMainMenu() {  
  Serial.print("A – Display MAC address\n");
  Serial.print("L - List available wifi networks\n");
  Serial.print("C – Connect to a wifi network\n");
  Serial.print("D – Disconnect from the network\n");
  Serial.print("I – Display connection info\n");
  Serial.print("M – Display the menu options\n");
  Serial.print("V - change the current mode to ");

  if (current_mode_of_operation == OPERATION_TYPE_NORMAL)
    Serial.print("UDP_BROADCAST\n");
  else
    Serial.print("NORMAL\n");

  Serial.print("Q - change the current mode to MQTT mode\n"); 
}


/***********************************************************************************
* Purpose: Print on the serial port the mac address in use.
* No arguments, no returns 
**********************************************************************************/
void printMacAddresses() {  
    WiFi.macAddress(mac);  // get your MAC address
    Serial.println(macAddressToString(mac));  // and print  your MAC address
}

/***********************************************************************************
* Purpose: Scan and detailed serial port print of the Network APs found
* No arguments, no returns
**********************************************************************************/
void networksList() {
  int num_of_ssid = WiFi.scanNetworks();   
  if (num_of_ssid > -1) {
    for (int this_net = 0; this_net < num_of_ssid; this_net++) {     
      Serial.print(this_net + 1);  // print the network number      
      Serial.print(". " + WiFi.SSID(this_net) + " [" );  // print the ssid
      Serial.print(wifiAuthModeToString(WiFi.encryptionType(this_net)).c_str());  // print the authentication mode
      Serial.print("]  (");
      Serial.print(WiFi.RSSI(this_net));  // print the ssid, encryption type and rssi for each network found
      Serial.print(" dBm)\n");
    }
  }
  else
    Serial.print("Couldn't get, or there is not a wifi connection!\n");
}

/***********************************************************************************
* Purpose: Connect to the chosen network from the list 
* No arguments, no returns
**********************************************************************************/
void connect() {  
  String ssid = WiFi.SSID(std::atoi(serialPrompt("\nChoose Network: ", 3).c_str()) - 1);
  String network_password = serialPrompt("Password: ", 42);  // that's it
  const char* cch_ssid = ssid.c_str();
  const char* cch_net_pss = network_password.c_str();
  Serial.print("Connecting to "); Serial.print(cch_ssid); Serial.print("...\n\n");
  WiFi.begin(cch_ssid, cch_net_pss);
  delay(2000);
  Serial.println(wifiStatusToString(WiFi.status()).c_str()); 
}

/***********************************************************************************
* Purpose: Disconnect WiFi and print the current status
* No arguments, no returns
**********************************************************************************/
void disconnect() {
  Serial.print("Disonnecting...");
  WiFi.disconnect();
  delay(2000);
  status = WiFi.status();
  Serial.println(wifiStatusToString(status).c_str());   
}

/***********************************************************************************
* Purpose: Print the connection info
* No arguments, no returns
**********************************************************************************/
void connectionInfo() {
  Serial.print("Status:\t\t");  Serial.println(wifiStatusToString(WiFi.status()).c_str());
  Serial.print("Network:\t");  Serial.println(WiFi.SSID());
  Serial.print("IP Address:\t");  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask:\t");  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway:\t");  Serial.println(WiFi.gatewayIP());
} 

/***********************************************************************************
* Purpose: Change the operation mode to normal
* No arguments, no returns 
**********************************************************************************/
void changeModeToNormal() {
  current_mode_of_operation = OPERATION_TYPE_NORMAL;
  Serial.println("NORMAL MODE");
}

/***********************************************************************************
* Purpose: Change the operation mode to udp broadcast
* No arguments, no returns
**********************************************************************************/
void changeModeToUDP() {
  current_mode_of_operation = OPERATION_TYPE_UDP_BROADCAST;
  Serial.println("UDP_BROADCAST MODE\nESC - change the current mode to NORMAL");
}

/***********************************************************************************
* Purpose: Switch between two main operation modes
* No arguments, no returns 
**********************************************************************************/
void changeMode() {
  if (current_mode_of_operation == OPERATION_TYPE_NORMAL)
    changeModeToUDP();
  else 
    changeModeToNormal();
}

/***********************************************************************************
* Purpose: Change the operation mode to mqtt
* No arguments, no returns 
**********************************************************************************/
void changeModeToMQTT() {
  current_mode_of_operation = OPERATION_TYPE_MQTT_MODE;
  Serial.print("\nMQTT MODE\nX - change the mode to NORMAL\n");
}

/***********************************************************************************
* Purpose: Check if the key "x" is pressed
* No arguments, no returns, affects some globals
**********************************************************************************/
void checkForXPressed() {
  if (Serial.available() > 0)
    serial_read = Serial.read();
  if ((serial_read == 88)||(serial_read == 120))  //  if X or x is pressed
    changeModeToNormal();
}

/***********************************************************************************
* Purpose: Check if the key "Esc" is pressed
* No arguments, no returns, affects some globals
**********************************************************************************/
void checkForESCPressed() {
  if (Serial.available() > 0)
      serial_read = Serial.read();
  if (serial_read == 27)
    changeModeToNormal();
}

/***********************************************************************************
* Purpose: Send earlies specified UDP packet with possible loop break with ESC key
* No arguments, no returns
**********************************************************************************/
void sendUDP()
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("You need to connect first! Switching back to the normal mode.\n");
    changeModeToNormal();
  }
  else {
    remote_ip = WiFi.gatewayIP();
    remote_ip[3] = 255;
    checkForESCPressed();
    
    // exit from loop every 10 seconds
    while (time(NULL) - seconds < 10 && current_mode_of_operation == OPERATION_TYPE_UDP_BROADCAST)
      checkForESCPressed();

    Udp.begin(UDP_PORT);
    Udp.beginPacket(remote_ip, UDP_PORT);

    for (int i = 0; i < UDP_PACKET_SIZE; i++)
      Udp.write(P_UDP_MESSAGE[i]);

    Udp.endPacket();
    Udp.stop();    
    seconds = time(NULL);
  }
}


/***********************************************************************************
* Purpose: Send the payload to topic
* Arguments: topic - ; payload
* No returns
**********************************************************************************/
void myMQTT(const char* topic, const char* payload) {
  client.setServer(MQTT_BROKER_SERVER, MQTT_BROKER_PORT);

  if (client.connect(MQTT_BROKER_CLIENT_ID, MQTT_BROKER_USERNAME, MQTT_BROKER_PASSWORD)) {
      if (client.publish(topic, payload)) {}
      else {
        Serial.print("ERROR: publishing failed with state ");
        Serial.print(client.state());
        delay(2000);
      }
  } else {
    Serial.print("ERROR: connect failed with state ");
    Serial.print(client.state());
    delay(2000);
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector  
  Serial.begin(115200,  SERIAL_8N1);
  printMainMenu();
}

void loop() {
  delay(1000);
  Serial.println("–––––––––––––––––––––––––––––––––––––––––\n");

  switch (current_mode_of_operation)
  {

    case OPERATION_TYPE_MQTT_MODE:
      if (WiFi.status() == WL_CONNECTED) {       
        checkForXPressed();  
        // exit from loopmore than every second after previous
        while (time(NULL) - seconds < 1 && current_mode_of_operation == OPERATION_TYPE_MQTT_MODE) {
          checkForXPressed();
        }
        // Read the sensors from the whiskey bug, convert it to char[] and send it
        sprintf(ch_temp, "%f", whiskey_bug.getTemp());
        sprintf(ch_pres, "%f", whiskey_bug.getPressure());
        sprintf(ch_alco, "%f", whiskey_bug.getAlcoholContent());
        myMQTT(MQTT_TEMPERATURE_TOPIC, ch_temp);
        myMQTT(MQTT_PRESSURE_TOPIC, ch_pres);
        myMQTT(MQTT_ALCOHOL_TOPIC, ch_alco);
        seconds = time(NULL);
      }
      else
      {
        Serial.println("\nYou need to connect first! Will not try mqtt operations.\nSwitching back to NORMAL MODE\n");
        changeModeToNormal();        
      }
      
      break;

    case OPERATION_TYPE_UDP_BROADCAST:
      sendUDP();
      break;

    case OPERATION_TYPE_NORMAL:

      key_pressed = serialPrompt("Choice: ", 1);

      switch (key_pressed[0])
      {
        // if the key m is pressed print the main menu again.
        case 'M':
        case 'm':
          printMainMenu();
          break;

        // if the key a is pressed print the mac address.
        case 'A':
        case 'a':
          printMacAddresses();
          break;

        // if the key l is pressed scan and list the networks.
        case 'L':
        case 'l':
          networksList();
          break;       

        // if the key c is pressed list the networks and connect.
        case 'C':
        case 'c':
          networksList();
          connect();
          break;                

        // if the key d is pressed disconnect from network.
        case 'D':
        case 'd':
          disconnect();
          break;

        // if the key i is pressed print the connection info.
        case 'I':
        case 'i':
          connectionInfo();
          break; 

        // if the key v is pressed change the operational mode.
        case 'V':
        case 'v':
          changeMode();
          break; 

        // if the key q is pressed change the operational mode to mqtt.
        case 'Q':
        case 'q':
          changeModeToMQTT();
          break;          
      }
      break;
  }
}