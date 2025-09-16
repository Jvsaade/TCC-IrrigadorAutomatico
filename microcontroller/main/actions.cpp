#include "actions.h"

void MotorSentidoAntiHorario() {
    digitalWrite(motorIn1, !sentido);
    digitalWrite(motorIn2, sentido);
}

void MotorSentidoHorario() {
    digitalWrite(motorIn1, sentido);
    digitalWrite(motorIn2, !sentido);
}

void DesligarLeds() {
    digitalWrite(LedVerde, LOW);
    digitalWrite(LedAzul, LOW);
}

void LigarVerde() {
    digitalWrite(LedAzul, LOW);
    digitalWrite(LedVerde, HIGH);
}

void LigarAzul() {
    digitalWrite(LedVerde, LOW);
    digitalWrite(LedAzul, HIGH);
}