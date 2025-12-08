#ifndef ACTIONS_H
#define ACTIONS_H

#include "arduino.h"

const int green_led = 4;
const int red_led = 5;
const int in1_motor = 14;
const int in2_motor = 12;
const int stop_pin = 13;

void _counterclockwise_motor(int velocidade);
void _clockwise_motor(int velocidade);
void _leds_off();
void _green_led_on();
void _red_led_on();
void irrigation(int duracao);

#endif