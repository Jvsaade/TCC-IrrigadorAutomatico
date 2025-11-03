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

int findTime(int hora, int minuto){
    DynamicJsonDocument doc(1024);
    if (!loadJsonFromFile(ALARM_FILE, doc)) return false;
    if (!doc.is<JsonArray>()) return false;
    for (JsonObject a : doc.as<JsonArray>()) {
        if (a["hora"].as<int>() == hora and a["minuto"].as<int>() == minuto and a["ativo"].as<String>() == "true") {
            Serial.println("Horário encontrado!");
            Serial.print("Duração do alarme: "); Serial.println(a["duracao"].as<int>());
            return a["duracao"].as<int>();
        }
    }
    Serial.println("Horário não encontrado...");
    return 0;
}

int nearestSchedule(int hora, int minuto) {
    DynamicJsonDocument doc(1024);
    // Se não conseguir carregar o arquivo, retorna 1
    if (!loadJsonFromFile(ALARM_FILE, doc)) return 1;
    // Se não for um array JSON, retorna 1
    if (!doc.is<JsonArray>()) return 1;
    JsonArray alarmes = doc.as<JsonArray>();
    // Se o array estiver vazio, retorna 1
    if (alarmes.size() == 0) return 1;
    int menorDiferenca = 24 * 60; // Valor máximo em minutos (1 dia)
    int alarmeMaisProximo = 1; // Valor padrão se não encontrar
    for (JsonObject alarme : alarmes) {
        int horaAlarme = alarme["hora"].as<int>();
        int minutoAlarme = alarme["minuto"].as<int>();
        // Calcula diferença em minutos
        int minutosAtuais = hora * 60 + minuto;
        int minutosAlarme = horaAlarme * 60 + minutoAlarme;
        int diferenca = minutosAlarme - minutosAtuais;
        // Se a diferença for negativa, significa que o alarme é para o dia seguinte
        if (diferenca < 0) {
            diferenca += 24 * 60; // Adiciona um dia em minutos
        }
        // Encontra o alarme mais próximo
        if (diferenca < menorDiferenca) {
            menorDiferenca = diferenca;
            alarmeMaisProximo = menorDiferenca; // Retorna a diferença em minutos
        }
    }
    return alarmeMaisProximo;
}
