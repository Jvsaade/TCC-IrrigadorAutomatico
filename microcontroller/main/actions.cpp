#include "actions.h"

void MotorSentidoAntiHorario(int velocidade) {
    digitalWrite(motorIn1, LOW);
    analogWrite(motorIn2, velocidade);
    delay(100);
    digitalWrite(motorIn2, LOW);

}

void MotorSentidoHorario(int velocidade) {
    analogWrite(motorIn1, velocidade);
    digitalWrite(motorIn2, LOW);
    delay(100);
    digitalWrite(motorIn1, LOW);
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

void Irrigation(int duracao){

}