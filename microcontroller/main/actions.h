#ifndef ACTIONS_H
#define ACTIONS_H

#include "arduino.h"

const int LedVerde = 4;
const int LedAzul = 5;
const int motorIn1 = 14;
const int motorIn2 = 12;
const int stopPin = 13;

void MotorSentidoAntiHorario(int velocidade);
void MotorSentidoHorario(int velocidade);
void DesligarLeds();
void LigarVerde();
void LigarAzul();
void Irrigation(int duracao);

#endif