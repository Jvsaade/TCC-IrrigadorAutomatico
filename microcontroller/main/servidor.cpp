#include "servidor.h"

void _set_alarm(){
  if (!server.hasArg("plain")) return server.send(400, "text/plain", "JSON ausente");
  DynamicJsonDocument doc(1024);
  if (deserializeJson(doc, server.arg("plain"))) return server.send(400, "text/plain", "Erro JSON");

  String nome = doc["nomeAlarme"].as<String>();
  if (_alarm_exists(nome)) return server.send(400, "text/plain", "Já existe");

  DynamicJsonDocument data(1024);
  _load_json_from_file(ALARM_FILE, data);
  JsonArray arr = data.is<JsonArray>() ? data.as<JsonArray>() : data.to<JsonArray>();
  JsonObject a = arr.createNestedObject();
  a["nome"] = nome;
  a["hora"] = doc["horaAlarme"].as<String>();
  a["minuto"] = doc["minutoAlarme"].as<String>();
  a["duracao"] = doc["duracaoAlarme"].as<String>();
  a["ativo"] = doc["ativo"].as<String>();
  a["dias"] = doc["diasSemana"].as<String>();
  _save_json_to_file(ALARM_FILE, data);
  Serial.print("Alarme adicionado. Nome: "); Serial.println(nome);
  Serial.print("Hora: "); Serial.println(a["hora"].as<String>());
  Serial.print("Minuto: "); Serial.println(a["minuto"].as<String>());
  Serial.print("Duração: "); Serial.println(a["duracao"].as<String>());
  server.send(200, "text/plain", "Criado");
}

void _edit_alarm(){
  if (!server.hasArg("plain") || !server.hasArg("nomeAntigo"))
    return server.send(400, "text/plain", "Faltando dados");
  String antigo = server.arg("nomeAntigo");
  DynamicJsonDocument doc(1024);
  if (deserializeJson(doc, server.arg("plain")))
    return server.send(400, "text/plain", "Erro JSON");

  DynamicJsonDocument data(1024);
  if (!_load_json_from_file(ALARM_FILE, data)) return server.send(500, "text/plain", "Erro ao abrir");
  JsonArray arr = data.as<JsonArray>();
  for (JsonObject a : arr) {
      if (a["nome"].as<String>() == antigo) {
          if (doc.containsKey("nomeAlarme")) a["nome"] = doc["nomeAlarme"].as<String>();
          if (doc.containsKey("horaAlarme")) a["hora"] = doc["horaAlarme"].as<String>();
          if (doc.containsKey("minutoAlarme")) a["minuto"] = doc["minutoAlarme"].as<String>();
          if (doc.containsKey("duracaoAlarme")) a["duracao"] = doc["duracaoAlarme"].as<String>();
          if (doc.containsKey("ativo")) a["ativo"] = doc["ativo"].as<String>();
          if (doc.containsKey("diasSemana")) a["dias"] = doc["diasSemana"].as<String>();
          _save_json_to_file(ALARM_FILE, data);
          return server.send(200, "text/plain", "Editado");
      }
  }
  server.send(404, "text/plain", "Não encontrado");

}

void _delete_alarm(){
  if (!server.hasArg("name")) return server.send(400, "text/plain", "Faltando nome");
  if (_remove_alarm(server.arg("name"))) server.send(200, "text/plain", "Removido");
  else server.send(404, "text/plain", "Não encontrado");

}

void _activate_alarm(){
  if (!server.hasArg("name")) return server.send(400, "text/plain", "Faltando nome");
  if (_update_alarm_Property(server.arg("name"), "ativo", "true")) server.send(200, "text/plain", "Ativado");
  else server.send(404, "text/plain", "Não encontrado");

}

void _deactivate_alarm(){
  if (!server.hasArg("name")) return server.send(400, "text/plain", "Faltando nome");
  if (_update_alarm_Property(server.arg("name"), "ativo", "false")) server.send(200, "text/plain", "Desativado");
  else server.send(404, "text/plain", "Não encontrado");

}

void _delete_all(){
  File file = LittleFS.open(ALARM_FILE, "w");
  file.print("[]");
  file.close();
  server.send(200, "text/plain", "Todos excluídos");

}

void _consult_alarm(){
  if (!server.hasArg("name")) return server.send(400, "text/plain", "Faltando nome");
  Serial.println("Consultando alarme...");
  if (_alarm_exists(server.arg("name"))) server.send(200, "text/plain", "True");
  else server.send(200, "text/plain", "False");
}

void server_init(){
  server.on("/setAlarm", HTTP_POST, _set_alarm); // Adicionado HTTP_POST
  server.on("/editAlarm", HTTP_POST, _edit_alarm); // Adicionado HTTP_POST
  server.on("/deleteAlarm", HTTP_POST, _delete_alarm); // Adicionado HTTP_POST
  server.on("/deleteAll", HTTP_POST, _delete_all); // Adicionado HTTP_POST
  server.on("/deactivateAlarm", HTTP_POST, _deactivate_alarm); // Adicionado HTTP_POST
  server.on("/activateAlarm", HTTP_POST, _activate_alarm); // Adicionado HTTP_POST
  server.on("/consultAlarm", _consult_alarm);
}
