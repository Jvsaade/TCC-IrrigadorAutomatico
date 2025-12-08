#ifndef ALARM_H
#define ALARM_H

#define ALARM_FILE "/alarm_file.txt"

#include <ArduinoJson.h>
#include <LittleFS.h>

bool _load_json_from_file(const char* path, DynamicJsonDocument& doc);

bool _save_json_to_file(const char* path, DynamicJsonDocument& doc);

bool _update_alarm_property(const String& nome, const String& key, const String& value);

bool _remove_alarm(const String& nome);

bool _alarm_exists(const String& nome);

int _find_time(int hora, int minuto, int diaSemana);

int _find_nearest_alarm(int hora, int minuto, int diaSemanaAtual);

#endif