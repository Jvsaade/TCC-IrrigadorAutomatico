#ifndef HORARIO_H
#define HORARIO_H

#include <ESP8266WiFi.h>
#include <time.h>
#include "alarm.h"

extern const char* ntpServer;
const long gmtOffset_sec = -3*3600;
const int daylightOffset_sec = 0;

extern struct tm timeinfo;

void setupTime();
bool getCurrentTime(int &hora, int &minuto, int &semana);
void erroHorario();
void sucessoHorario();
int checkSchedule();
int tempoDeepSleep();

#endif