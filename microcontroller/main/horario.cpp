#include "horario.h"
#include "actions.h"
#include <time.h>
#include <ESP8266WiFi.h>

struct tm timeinfo;
const char *ntpServer = "pool.ntp.org";

bool getCurrentTime(int &hora, int &minuto) {
  static uint16_t cont_err = 0;
  static uint16_t cont_suc = 0;

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    cont_err++;
    if (cont_err == 200) {
      cont_err = 0;
      erroHorario();
    }
    return false;
  }
  cont_suc++;
  if (cont_suc == 3000) {
    cont_suc = 0;
    sucessoHorario();
  }
  hora = timeinfo.tm_hour;
  minuto = timeinfo.tm_min;
  Serial.print("Hora local: ");
  Serial.print(hora);
  Serial.print(":");
  Serial.println(minuto);
  return true;
}

void erroHorario() {
  for (int i = 0; i < 2; i++) {
    LigarAzul();
    delay(1000);
    DesligarLeds();
    delay(1000);
    LigarAzul();
    delay(100);
    DesligarLeds();
    delay(1000);
  }
}

void sucessoHorario() {
  for (int i = 0; i < 2; i++) {
    LigarVerde();
    delay(1000);
    DesligarLeds();
    delay(1000);
    LigarVerde();
    delay(100);
    DesligarLeds();
    delay(1000);
  }
}

void setupTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

int checkSchedule() {
  int horaAtual, minutoAtual;
  if (getCurrentTime(horaAtual, minutoAtual)) {
    return findTime(horaAtual, minutoAtual);
  }
  return 0;
}

/*
  @brief Calcula o tempo em minutos para o microcontrolador ficar em deepsleep
  @return Tempo em minutos para o DeepSleep
*/

int tempoDeepSleep() {
  int horaAtual, minutoAtual;
  if (!getCurrentTime(horaAtual, minutoAtual))
    return 1;
  return nearestSchedule(horaAtual, minutoAtual) * 1e6;
}
