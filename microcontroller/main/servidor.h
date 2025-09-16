#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "connection.h"
#include "alarm.h"

void setAlarm();

void editAlarm();

void deleteAlarm();

void activateAlarm();

void deleteAll();

void inicializarServidor();

#endif