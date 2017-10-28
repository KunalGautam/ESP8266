/*
  ESP8266-01
  Communicate with ESP01 Module.

  This code aims at communicating Arduino with ESP01 Module without any problem.
  
  Author: Kunal Gautam
  v0.01 
*/

// Creating Alias 
#define dbg Serial 

// Defining Variables, you can change these values
#define espbaud 9600
#define DEBUG true
#define TXPIN 10 //TX Pin of ESP01 Module
#define RXPIN 11 //RX Pin of ESP01 Module
#define CWMODE 3 // Set mode of ESP. 1(Client), 2(Server/Router), 3(Both 1+2)
#define BUFFER_SIZE 128 // Less buffer is good.

#define SSID "RANDOMSSID"  // WiFi SSID of the Router
#define SSIDPW "RANDOMPASSWORD" //WiFi Password of the Router

#define APID "ESP" // When ESP is set as Access Point, Set its SSID
#define APPW "123456789"  // When ESP is set as Access Point, Set its SSID's Password
#define APCH 1 // When ESP is set as Access Point, Set its channel number
#define APECN 3 // When ESP is set as Access Point, Set its ECN where (0 -> Open, 2 ->WPA_PSK, 3 -> WPA2_PSK, 4 -> WPA_WPA2_PSL)

int isok_ready;
char buffer[BUFFER_SIZE];

// Including some library
#include <SoftwareSerial.h>

// Initiate Library
SoftwareSerial esp(TXPIN, RXPIN);





void setup() {
  dbg.begin(9600);
  while (!dbg) {;}

  delay(1000); // Delay assuming ESP is getting powered on

if (ATresp() == 1) {
  dbg.println("ESP Module Communicated\nConnecting to WiFi Network");
  setESPmode();
} else {
  dbg.println("Unable to communicate with ESP Module[ERR001]");
}



}// End of setup()

void loop() {
  // put your main code here, to run repeatedly:

}

int ATresp() { // Get AT Response
  esp.begin(espbaud);
  while (!esp) { ; }
  dbg.println("Sending AT Command");
  esp.println("AT");
  wait_for_esp_response(1000, "OK");
  dbg.println(buffer);  
  return isok_ready;
}

void setESPmode() {
  esp.begin(espbaud);
  while (!esp) { ; }
  dbg.println("Setting Mode of the Device");
  esp.print("AT+CWMODE_CUR=");
  esp.println(CWMODE);
  wait_for_esp_response(1000, "OK");
  dbg.println(buffer);
  
  if (CWMODE == 2 || CWMODE == 3) { // Following should run only if CWMODE IS 2 or 3
        
      // Set Accesspoint name and password
      dbg.println("Setting Access Point");
      esp.print("AT+CWSAP_CUR=\"");
      esp.print(APID);
      esp.print("\",\"");
      esp.print(APPW);
      esp.print("\",");
      esp.print(APCH);
      esp.print(",");
      esp.println(APECN);
      wait_for_esp_response(1000, "OK");
      dbg.println(buffer);

  }

  if (CWMODE == 1 || CWMODE == 3) { // Following should run only if CWMODE IS 1 or 3
        
      // Set Accesspoint name and password
      dbg.println("Connecting to WiFi Network");
      esp.print("AT+CWJAP_CUR=\"");
      esp.print(SSID);
      esp.print("\",\"");
      esp.print(SSIDPW);
      esp.println("\"");
      wait_for_esp_response(5000, "WIFI GOT IP");
      dbg.println(buffer);

  }

  


  
}







































// Code for reading response

byte wait_for_esp_response(int timeout, char* term) {
  unsigned long t = millis();
  bool found = false;
  int i = 0;

  int len = strlen(term);
  isok_ready = 0;
  // wait for at most timeout milliseconds
  buffer[0]=0;
  while (millis() < t + timeout) {
    if (esp.available()) {
      buffer[i++] = esp.read();
      if (i >= len) {
        if (strncmp(buffer + i - len, term, len) == 0) {
          isok_ready = 1;
          found = true;
          break;
        }
      }
    }
  }
  buffer[i] = 0;
  return found;
}

