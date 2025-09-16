#ifndef ACTIONS_H
#define ACTIONS_H

#include "arduino.h"

const int LedVerde = 4;
const int LedAzul = 5;
const int motorIn1 = 13;
const int motorIn2 = 12;
const bool sentido = HIGH;

void MotorSentidoAntiHorario();
void MotorSentidoHorario();
void DesligarLeds();
void LigarVerde();
void LigarAzul();

#endif