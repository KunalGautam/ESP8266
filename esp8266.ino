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
#define BUFFER_SIZE 128 // Less buffer is good.
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
  dbg.println("ESP Module Communicated");

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

