/**
 * globals.h
 * 
 * Created on: 15.08.2018
 *     Author: Benedikt-Alexander Mokro?? (benedikt@bhorn.de)
 */

#ifndef GLOBALS_H
#define GLOBALS_H
#include <WiFiManager.h>

/**
 * WiFi Config
 */
#define AP_PASSWORD "Antwort42"

/**
 * Misc 
 */
#define UTC_TIMEZONE 2
#define DEFAULT_NAME_SOCKET1 "Vitrine"
#define DEFAULT_NAME_SOCKET2 "Links"
#define DEFAULT_NAME_SOCKET3 "Rechts"

/**
 * Don't touch anything below this line!
 * I mean it...
 *  - bam
 */
#define CODE_VERSION "v1.1"

struct configPortalHolder {
  WiFiManagerParameter* nameSocket1;        
  WiFiManagerParameter* nameSocket2;
  WiFiManagerParameter* nameSocket3;
} configParameter;

#endif

