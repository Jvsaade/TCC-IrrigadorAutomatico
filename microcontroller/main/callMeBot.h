#ifndef CALL_ME_BOT_H
#define CALL_ME_BOT_H

#include <UrlEncode.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

extern WiFiClientSecure client;

extern String phoneNumber; // <-- Coloque seu número aqui
extern String apiKey; // <-- Coloque sua API Key aqui

void send_message(String message);
void call_me_bot_init();

#endif