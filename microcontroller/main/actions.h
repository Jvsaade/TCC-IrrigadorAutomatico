#ifndef ACTIONS_H
#define ACTIONS_H

#include "arduino.h"

const int LedVerde = 4;
const int LedAzul = 5;
const int motorIn1 = 14;
const int motorIn2 = 12;

void _counterclockwise_motor(int velocidade, int tempo);
void _clockwise_motor(int velocidade, int tempo);
void _leds_off();
void _green_led_on();
void _red_led_on();
void irrigation(int duracao);

#endif