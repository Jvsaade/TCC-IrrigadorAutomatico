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


void ConnectionError();

void OTA_Initialization();

void SuccessfulConnection();

void consulta_bateria();

bool IniciarSmartConfig();

void SalvarCredenciais(const String& ssid, const String& password);

bool RecuperarCredenciaisWiFi();

bool ConectarWiFiComCredenciaisSalvas();

#endif