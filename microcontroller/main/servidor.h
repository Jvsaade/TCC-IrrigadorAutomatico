#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "connection.h"
#include "alarm.h"

void _set_alarm();

void _edit_alarm();

void _delete_alarm();

void _activate_alarm();

void _consult_alarm();

void _delete_alarm();

void server_init();

#endif