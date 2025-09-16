#ifndef BATERIA_H
#define BATERIA_H

#include "Arduino.h"

const int PINO_ADC_BATERIA = A0;
const float R1 = 1000000.0;
const float R2 = 470000.0;
const float FATOR_DIVISOR = (R1 + R2) / R2;
const float VOLTAGEM_ALTA = 2.80;
const float VOLTAGEM_BAIXA = 2.30;

enum BatteryState{
    ALTO,
    MEDIO,
    BAIXO,
};

BatteryState EstadoBateria();

#endif