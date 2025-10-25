#include "horario.h"
#include <time.h>
#include <ESP8266WiFi.h>

struct tm timeinfo;
const char* ntpServer = "pool.ntp.org";

bool getCurrentTime(int &hora, int &minuto) {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return false;
  }
  hora = timeinfo.tm_hour;
  minuto = timeinfo.tm_min;
  Serial.print("Hora local: "); Serial.print(hora); Serial.print(":"); Serial.println(minuto);
  return true;
}

void setupTime(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

int checkSchedule(){
  int horaAtual, minutoAtual;
  if(getCurrentTime(horaAtual, minutoAtual)){
    return findTime(horaAtual, minutoAtual);
  }
  return 0;
}
