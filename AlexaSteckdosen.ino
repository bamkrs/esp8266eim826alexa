#include <FS.h>
#include <ESP8266WiFi.h>
#include <fauxmoESP.h>
#include "globals.h"

#define GPIO_0 0
#define GPIO_1 1
#define GPIO_2 2
#define rfTX 2

#include "eim826.h"

fauxmoESP fauxmo;

int socket1State = 0;
int socket2State = 0;
int socket3State = 0;

int next = 0;

String getChipIdHex()
{
  return WiFi.macAddress().substring(9,11)+  // xx:xx:xx:XX:xx:xx
         WiFi.macAddress().substring(12,14)+ // xx:xx:xx:xx:XX:xx
         WiFi.macAddress().substring(15,17); // xx:xx:xx:xx:xx:XX
}

void setup() {

  Serial.begin(74880);
  
  if (SPIFFS.begin()) {
    SPIFFS.end();
    Serial.println(F("Mounted FS"));
    Serial.println(CODE_VERSION);
    Serial.println(WiFi.macAddress());
    wifiConnect();
  }
  else
  {
    // Cant mount memory. PANIC...
    pinMode(1, OUTPUT);
    bool ledOn = true;
    while(1)
    {
      digitalWrite(1,ledOn);
      ledOn = !ledOn;
      delay(250);
    }
  }
  
  Serial.println(F("Toggeling everything off..."));
  pinMode(rfTX, OUTPUT);
  //alles aus
  setSocket(0, 0);
  yield();
  setSocket(1, 0);
  yield();
  setSocket(2, 0);
  yield();

  fauxmo.addDevice(configParameter.nameSocket1->getValue());
  fauxmo.addDevice(configParameter.nameSocket2->getValue());
  fauxmo.addDevice(configParameter.nameSocket3->getValue());
  fauxmo.enable(true);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state) {
    //vitrine = 0;
    //links = 1;
    //rechts = 2;
    Serial.printf(("[MAIN] Device #%d (%s) state: %s\n"), device_id, device_name, state ? "ON" : "OFF");
    setSocket(device_id, state ? 1 : 0);
    switch(device_id){
      case 0:
        return socket1State = state;
      case 1:
        return socket2State = state;
      case 2:
        return socket3State = state;
    }
  });
  fauxmo.onGetState([](unsigned char device_id, const char * device_name) {
    switch(device_id){
      case 0:
        return socket1State == 1;
      case 1:
        return socket2State == 1;
      case 2:
        return socket3State == 1;
    }
    return false;
  });
}

void loop() {
    //setSocket(4, next);
    //Serial.println(("Socket ALL"));
    //yield();
    fauxmo.handle();
    //if(next == 0)
    //  next = 1;
    //else 
    //  next = 0;
    //delay(3);
}
