#ifndef HORARIO_H
#define HORARIO_H

#include <ESP8266WiFi.h>
#include <time.h>
#include "alarm.h"

extern const char* ntpServer;
const long gmtOffset_sec = -3*3600;
const int daylightOffset_sec = 0;

extern struct tm timeinfo;

void time_init();
bool get_current_time(int &hora, int &minuto, int &semana);
void _hour_check_error();
void _hour_check_success();
int check_schedule();
int deep_sleep_time();

#endif