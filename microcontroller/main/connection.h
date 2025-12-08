#ifndef CONNECTION_H
#define CONNECTION_H

#define CONFIG_FILE "/wifi_config.txt" // Nome do arquivo para armazenar as credenciais
// Senha OTA: 12345

#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "Arduino.h"
#include "actions.h"
#include "bateria.h"


extern ESP8266WebServer server;
extern const unsigned int WiFiSmartConfig_Timeout;
extern const unsigned int WiFiConnection_Timeout;
extern String globalSSID;
extern String globalPW;


void connection_error();

void OTA_init();

void successful_connection();

void _battery_consult();

bool smartconfig_init();

void _save_credentials(const String& ssid, const String& password);

bool _recover_wifi_credentials();

bool connect_wifi_saved_credentials();

#endif