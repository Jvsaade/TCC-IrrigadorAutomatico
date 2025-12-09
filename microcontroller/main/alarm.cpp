#include "alarm.h"

bool _load_json_from_file(const char* path, DynamicJsonDocument& doc) {
  File file = LittleFS.open(path, "r");
  if (!file) return false;
  DeserializationError error = deserializeJson(doc, file);
  file.close();
  return !error;
}

bool _save_json_to_file(const char* path, DynamicJsonDocument& doc) {
  File file = LittleFS.open(path, "w");
  if (!file) return false;
  serializeJson(doc, file);
  file.close();
  return true;
}

bool _update_alarm_property(const String& nome, const String& key, const String& value) {
  DynamicJsonDocument doc(1024);
  if (!_load_json_from_file(ALARM_FILE, doc)) return false;
  if (!doc.is<JsonArray>()) return false;
  JsonArray arr = doc.as<JsonArray>();
  for (JsonObject a : arr) {
    if (a["nome"].as<String>() == nome) {
      a[key] = value;
      return _save_json_to_file(ALARM_FILE, doc);
    }
  }
  return false;
}

bool _remove_alarm(const String& nome) {
  DynamicJsonDocument doc(1024);
  if (!_load_json_from_file(ALARM_FILE, doc)) return false;
  if (!doc.is<JsonArray>()) return false;
  JsonArray arr = doc.as<JsonArray>();
  for (int i = 0; i < arr.size(); i++) {
    if (arr[i]["nome"].as<String>() == nome) {
      arr.remove(i);
      return _save_json_to_file(ALARM_FILE, doc);
    }
  }
  return false;
}

bool _alarm_exists(const String& nome) {
  DynamicJsonDocument doc(1024);
  if (!_load_json_from_file(ALARM_FILE, doc)) return false;
  if (!doc.is<JsonArray>()) return false;
  for (JsonObject a : doc.as<JsonArray>()) {
    if (a["nome"].as<String>() == nome) return true;
  }
  return false;
}

int _find_time(int hora, int minuto, int diaSemana) {
  DynamicJsonDocument doc(1024);
  if (!_load_json_from_file(ALARM_FILE, doc)) return 0;
  if (!doc.is<JsonArray>()) return 0;
  
  for (JsonObject a : doc.as<JsonArray>()) {
    if (a["hora"].as<int>() == hora && 
        a["minuto"].as<int>() == minuto && 
        a["ativo"].as<String>() == "true") {
      
      String diasSemana = a["dias"].as<String>();
      if (diasSemana.length() == 7 && diasSemana[diaSemana] == '1') {
        Serial.println("Horário de irrigação!");
        return a["duracao"].as<int>();
      }
    }
  }
  return 0;
}

int _find_nearest_alarm(int hora, int minuto, int diaSemanaAtual) { // Retorna quantos minutos faltam para o próximo alarme
  DynamicJsonDocument doc(1024);
  if (!_load_json_from_file(ALARM_FILE, doc)) return 1;
  if (!doc.is<JsonArray>()) return 1;
  JsonArray alarmes = doc.as<JsonArray>();
  if (alarmes.size() == 0) return 1;

  int minutosAtuais = hora * 60 + minuto;
  int menorDiferenca = 7 * 24 * 60;  // 1 semana em minutos
  bool encontrouAlarme = false;

  for (JsonObject alarme : alarmes) {
    int horaAlarme = alarme["hora"].as<int>();
    int minutoAlarme = alarme["minuto"].as<int>();
    String semana = alarme["dias"].as<String>();
    int minutosAlarme = horaAlarme * 60 + minutoAlarme;

    // Verifica para cada dia ativo do alarme
    for (int diaOffset = 0; diaOffset < 7; diaOffset++) {
      int diaAlarme = (diaSemanaAtual + diaOffset) % 7;

      if (semana[diaAlarme] == '1') {
        int diferenca = (diaOffset * 24 * 60) + (minutosAlarme - minutosAtuais);

        // Ignora alarmes que já passaram no dia atual
        if (diaOffset == 0 && diferenca < 0) {
          continue;
        }

        if (diferenca < menorDiferenca && diferenca >= 0) {
          menorDiferenca = diferenca;
          encontrouAlarme = true;
        }
      }
    }
  }

  return encontrouAlarme ? menorDiferenca : 1;
}
