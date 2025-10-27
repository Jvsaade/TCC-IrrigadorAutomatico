#include "actions.h"

void MotorSentidoAntiHorario(int velocidade, int tempo) {
    digitalWrite(motorIn1, LOW);
    analogWrite(motorIn2, velocidade);
    delay(100);
    digitalWrite(motorIn2, LOW);

}

void MotorSentidoHorario(int velocidade, int tempo) {
    analogWrite(motorIn1, velocidade);
    digitalWrite(motorIn2, LOW);
    delay(tempo);
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

void AbrirValvula(){
    MotorSentidoAntiHorario(500, 100);
}

void FecharValvula(){
    MotorSentidoHorario(500, 100);
}

void Irrigation(int duracao){
    Serial.print("Irrigação iniciada por "); Serial.print(duracao); Serial.println(" minutos");
    AbrirValvula();
    delay((duracao*1e3)*60);
    FecharValvula();
    Serial.print("Irrigação finalizada.");

}