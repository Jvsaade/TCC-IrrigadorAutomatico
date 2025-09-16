#ifndef ALARM_H
#define ALARM_H

#define ALARM_FILE "/alarm_file.txt"

#include <ArduinoJson.h>
#include <LittleFS.h>

bool loadJsonFromFile(const char* path, DynamicJsonDocument& doc);

bool saveJsonToFile(const char* path, DynamicJsonDocument& doc);

bool updateAlarmProperty(const String& nome, const String& key, const String& value);

bool removeAlarm(const String& nome);

bool alarmExists(const String& nome);

#endif