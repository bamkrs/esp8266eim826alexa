/**
 * Management.ino
 * 
 * Created on: 15.08.2018
 *     Author: Benedikt-Alexander Mokroß (benedikt@bhorn.de)
 */

#include <FS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "globals.h"

const char* socket1Annotation = "Name of socket 1";
const char* socket2Annotation = "Name of socket 2";
const char* socket3Annotation = "Name of socket 3";
const char* socket1Parameter = "PARAMETER_NAME_SOCKET1";
const char* socket2Parameter = "PARAMETER_NAME_SOCKET2";
const char* socket3Parameter = "PARAMETER_NAME_SOCKET3";

bool wifiManagerSetup()
{  
  if(!SPIFFS.begin()){
    return false;
  }
  String nameSocket1Str = getConfigFS("ns1");
  if(nameSocket1Str.length() > 0)
    configParameter.nameSocket1 = new WiFiManagerParameter(socket1Parameter, socket1Annotation, nameSocket1Str.c_str(), 64);
  else
    configParameter.nameSocket1 = new WiFiManagerParameter(socket1Parameter, socket1Annotation, DEFAULT_NAME_SOCKET1, 64);
  
  String nameSocket2Str = getConfigFS("ns2");
  if(nameSocket2Str.length() > 0)
    configParameter.nameSocket2 = new WiFiManagerParameter(socket2Parameter, socket2Annotation, nameSocket2Str.c_str(), 64);
  else
    configParameter.nameSocket2 = new WiFiManagerParameter(socket2Parameter, socket2Annotation, DEFAULT_NAME_SOCKET2, 64);
  
  String nameSocket3Str = getConfigFS("ns3");
  if(nameSocket3Str.length() > 0)
    configParameter.nameSocket3 = new WiFiManagerParameter(socket3Parameter, socket3Annotation, nameSocket3Str.c_str(), 32);
  else
    configParameter.nameSocket3 = new WiFiManagerParameter(socket3Parameter, socket3Annotation, DEFAULT_NAME_SOCKET3, 32);
  SPIFFS.end();
  return true;
}

bool wifiManagerPersist()
{
  if(!SPIFFS.begin()){
    return false;
  }
  bool s1 = setConfigFS("ns1", configParameter.nameSocket1->getValue());
  bool s2 = setConfigFS("ns2", configParameter.nameSocket2->getValue());
  bool s3 = setConfigFS("ns3", configParameter.nameSocket3->getValue());
  SPIFFS.end();
  return s1 && s2 && s3;
}

void wifiManagerTellSettings()
{
  Serial.print(F("Socket 1 Name:\t\t"));
  Serial.println(configParameter.nameSocket1->getValue());

  Serial.print(F("Socket 2 Name:\t\t"));
  Serial.println(configParameter.nameSocket2->getValue());

  Serial.print(F("Socket 3 Name:\t\t"));
  Serial.println(configParameter.nameSocket3->getValue());
}

bool wifiManager()
{
  WiFiManager wifiManager;
  wifiManagerSetup();
  
  WiFiManagerParameter custom_text_s1(("<p>Name of socket 1"));
  wifiManager.addParameter(&custom_text_s1);
  wifiManager.addParameter(configParameter.nameSocket1);
  
  WiFiManagerParameter custom_text_s2(("</p><p>Name of socket 2"));
  wifiManager.addParameter(&custom_text_s2);
  wifiManager.addParameter(configParameter.nameSocket2);

  WiFiManagerParameter custom_text_s3(("</p><p>Name of socket 3"));
  wifiManager.addParameter(&custom_text_s3);
  wifiManager.addParameter(configParameter.nameSocket3);

  WiFiManagerParameter custom_text_end(("</p>"));
  wifiManager.addParameter(&custom_text_end);
  
  String apid = String("Sockets_")+getChipIdHex();
  wifiManager.autoConnect(apid.c_str(),AP_PASSWORD);

  wifiManagerPersist();

  wifiManagerTellSettings();
  return true;
}

void wifiConnect()
{  
  Serial.print("Connecting to AP");
  if(wifiManager())
    Serial.println("WiFi connected");  
}


