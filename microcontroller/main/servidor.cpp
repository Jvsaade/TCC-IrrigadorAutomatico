#include "servidor.h"

void setAlarm(){
  if (!server.hasArg("plain")) return server.send(400, "text/plain", "JSON ausente");
  DynamicJsonDocument doc(1024);
  if (deserializeJson(doc, server.arg("plain"))) return server.send(400, "text/plain", "Erro JSON");

  String nome = doc["nomeAlarme"].as<String>();
  if (alarmExists(nome)) return server.send(400, "text/plain", "Já existe");

  DynamicJsonDocument data(1024);
  loadJsonFromFile(ALARM_FILE, data);
  JsonArray arr = data.is<JsonArray>() ? data.as<JsonArray>() : data.to<JsonArray>();
  JsonObject a = arr.createNestedObject();
  a["nome"] = nome;
  a["hora"] = doc["horaAlarme"].as<String>();
  a["minuto"] = doc["minutoAlarme"].as<String>();
  a["duracao"] = doc["duracaoAlarme"].as<String>();
  a["ativo"] = doc["ativo"].as<String>();
  a["dias"] = doc["diasSemana"].as<String>();
  saveJsonToFile(ALARM_FILE, data);
  server.send(200, "text/plain", "Criado");

}

void editAlarm(){
  if (!server.hasArg("plain") || !server.hasArg("nomeAntigo"))
    return server.send(400, "text/plain", "Faltando dados");
  String antigo = server.arg("nomeAntigo");
  DynamicJsonDocument doc(1024);
  if (deserializeJson(doc, server.arg("plain")))
    return server.send(400, "text/plain", "Erro JSON");

  DynamicJsonDocument data(1024);
  if (!loadJsonFromFile(ALARM_FILE, data)) return server.send(500, "text/plain", "Erro ao abrir");
  JsonArray arr = data.as<JsonArray>();
  for (JsonObject a : arr) {
      if (a["nome"].as<String>() == antigo) {
          if (doc.containsKey("nomeAlarme")) a["nome"] = doc["nomeAlarme"].as<String>();
          if (doc.containsKey("horaAlarme")) a["hora"] = doc["horaAlarme"].as<String>();
          if (doc.containsKey("minutoAlarme")) a["minuto"] = doc["minutoAlarme"].as<String>();
          if (doc.containsKey("duracaoAlarme")) a["duracao"] = doc["duracaoAlarme"].as<String>();
          if (doc.containsKey("ativo")) a["ativo"] = doc["ativo"].as<String>();
          if (doc.containsKey("diasSemana")) a["dias"] = doc["diasSemana"].as<String>();
          saveJsonToFile(ALARM_FILE, data);
          return server.send(200, "text/plain", "Editado");
      }
  }
  server.send(404, "text/plain", "Não encontrado");

}

void deleteAlarm(){
  if (!server.hasArg("name")) return server.send(400, "text/plain", "Faltando nome");
  if (removeAlarm(server.arg("name"))) server.send(200, "text/plain", "Removido");
  else server.send(404, "text/plain", "Não encontrado");

}

void activateAlarm(){
  if (!server.hasArg("name")) return server.send(400, "text/plain", "Faltando nome");
  if (updateAlarmProperty(server.arg("name"), "ativo", "true")) server.send(200, "text/plain", "Ativado");
  else server.send(404, "text/plain", "Não encontrado");

}

void deactivateAlarm(){
  if (!server.hasArg("name")) return server.send(400, "text/plain", "Faltando nome");
  if (updateAlarmProperty(server.arg("name"), "ativo", "false")) server.send(200, "text/plain", "Desativado");
  else server.send(404, "text/plain", "Não encontrado");

}

void deleteAll(){
  File file = LittleFS.open(ALARM_FILE, "w");
  file.print("[]");
  file.close();
  server.send(200, "text/plain", "Todos excluídos");

}

void inicializarServidor(){
  server.on("/setAlarm", HTTP_POST, setAlarm); // Adicionado HTTP_POST
  server.on("/editAlarm", HTTP_POST, editAlarm); // Adicionado HTTP_POST
  server.on("/deleteAlarm", HTTP_POST, deleteAlarm); // Adicionado HTTP_POST
  server.on("/deleteAll", HTTP_POST, deleteAll); // Adicionado HTTP_POST
  server.on("/deactivateAlarm", HTTP_POST, deactivateAlarm); // Adicionado HTTP_POST
  server.on("/activateAlarm", HTTP_POST, activateAlarm); // Adicionado HTTP_POST

}