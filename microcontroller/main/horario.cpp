#include "horario.h"
#include "actions.h"
#include <time.h>
#include <ESP8266WiFi.h>

struct tm timeinfo;
const char *ntpServer = "pool.ntp.org";

bool get_current_time(int &hora, int &minuto, int &semana) {
  static uint16_t cont_err = 0;
  static uint16_t cont_suc = 0;

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    cont_err++;
    if (cont_err == 200) {
      cont_err = 0;
      _hour_check_error();
    }
    return false;
  }
  cont_suc++;
  if (cont_suc == 3000) {
    cont_suc = 0;
    _hour_check_success();
  }
  hora = timeinfo.tm_hour;
  minuto = timeinfo.tm_min;
  semana = timeinfo.tm_wday;
  Serial.print("Hora local: ");
  Serial.print(hora);
  Serial.print(":");
  Serial.println(minuto);
  return true;
}

void _hour_check_error() {
  for (int i = 0; i < 2; i++) {
    _red_led_on();
    delay(1000);
    _leds_off();
    delay(1000);
    _red_led_on();
    delay(100);
    _leds_off();
    delay(1000);
  }
}

void _hour_check_success() {
  for (int i = 0; i < 2; i++) {
    _green_led_on();
    delay(1000);
    _leds_off();
    delay(1000);
    _green_led_on();
    delay(100);
    _leds_off();
    delay(1000);
  }
}

void time_init() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

int check_schedule() {
  int horaAtual, minutoAtual, diaSemana;
  if (get_current_time(horaAtual, minutoAtual, diaSemana)) {
    return _find_time(horaAtual, minutoAtual, diaSemana);
  }
  return 0;
}

/*
  @brief Calcula o tempo em microsegundos para o microcontrolador ficar em deepsleep
  @return Tempo em minutos para o DeepSleep
*/

int deep_sleep_time() {
  int horaAtual, minutoAtual, diaSemana;
  if (!get_current_time(horaAtual, minutoAtual, diaSemana))
    return 1;
  int proximoAlarme = _find_nearest_alarm(horaAtual, minutoAtual, diaSemana);
  if(proximoAlarme < 2 * 60) // Se estiver dentro das próximas 2 horas, espera até esse tempo
    return (proximoAlarme - 2) * 60 * 1e6; // Acorda 2 minutos antes do programado
  else
    return 2*60*1e6;
}
