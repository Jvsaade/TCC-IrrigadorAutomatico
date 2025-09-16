#include "alarm.h"

bool loadJsonFromFile(const char* path, DynamicJsonDocument& doc) {
    File file = LittleFS.open(path, "r");
    if (!file) return false;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    return !error;
}

bool saveJsonToFile(const char* path, DynamicJsonDocument& doc) {
    File file = LittleFS.open(path, "w");
    if (!file) return false;
    serializeJson(doc, file);
    file.close();
    return true;
}

bool updateAlarmProperty(const String& nome, const String& key, const String& value) {
    DynamicJsonDocument doc(1024);
    if (!loadJsonFromFile(ALARM_FILE, doc)) return false;
    if (!doc.is<JsonArray>()) return false;
    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject a : arr) {
        if (a["nome"].as<String>() == nome) {
            a[key] = value;
            return saveJsonToFile(ALARM_FILE, doc);
        }
    }
    return false;
}

bool removeAlarm(const String& nome) {
    DynamicJsonDocument doc(1024);
    if (!loadJsonFromFile(ALARM_FILE, doc)) return false;
    if (!doc.is<JsonArray>()) return false;
    JsonArray arr = doc.as<JsonArray>();
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i]["nome"].as<String>() == nome) {
            arr.remove(i);
            return saveJsonToFile(ALARM_FILE, doc);
        }
    }
    return false;
}

bool alarmExists(const String& nome) {
    DynamicJsonDocument doc(1024);
    if (!loadJsonFromFile(ALARM_FILE, doc)) return false;
    if (!doc.is<JsonArray>()) return false;
    for (JsonObject a : doc.as<JsonArray>()) {
        if (a["nome"].as<String>() == nome) return true;
    }
    return false;
}
