#include "actions.h"

void MotorSentidoAntiHorario(int velocidade, int tempo) {
    digitalWrite(motorIn1, LOW);
    analogWrite(motorIn2, velocidade);
    delay(tempo);
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
    MotorSentidoAntiHorario(200, 200);
}

void FecharValvula(){
    MotorSentidoHorario(200, 200);
}

void Irrigation(int duracao){
    Serial.print("Irrigação iniciada por "); Serial.print(duracao); Serial.println(" minutos");
    AbrirValvula();
    unsigned long count = millis();
    while(millis()-count < (duracao*1e3)*60){
        LigarAzul();
        delay(100);
        DesligarLeds();
        LigarVerde();
        delay(1000);
        DesligarLeds();
    }
    Serial.print("Irrigação finalizada.");
    FecharValvula();

}