#include <FS.h>
#include <ESP8266WiFi.h>
#include <fauxmoESP.h>

#define GPIO_0 0
#define GPIO_1 1
#define GPIO_2 2
#define rfTX 2

#include "eim826.h"
const char* ssid     = "...";
const char* password = "...";


fauxmoESP fauxmo;

int links = 0;
int rechts = 0;
int vitrine = 0;

int next = 0;

void setup() {

    Serial.begin(115200);

    delay(10);

    Serial.println();
    Serial.println();
    Serial.println(("Toggeling everything off..."));
    pinMode(rfTX, OUTPUT);
    //alles aus
    setSocket(0, 0);
    yield();
    setSocket(1, 0);
    yield();
    setSocket(2, 0);
    yield();

    // We start by connecting to a WiFi network
    
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.println(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    fauxmo.addDevice("vitrine");
    fauxmo.addDevice("links");
    fauxmo.addDevice("rechts");
    fauxmo.enable(true);

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state) {
        //vitrine = 0;
        //links = 1;
        //rechts = 2;
        Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
        setSocket(device_id, state ? 1 : 0);
        switch(device_id){
          case 0:
            return vitrine = state;
          case 1:
            return links = state;
          case 2:
            return rechts = state;
        }
    });
    fauxmo.onGetState([](unsigned char device_id, const char * device_name) {
        switch(device_id){
          case 0:
            return vitrine == 1;
          case 1:
            return links == 1;
          case 2:
            return rechts == 1;
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
