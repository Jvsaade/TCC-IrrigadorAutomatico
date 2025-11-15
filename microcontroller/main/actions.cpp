#include "actions.h"

void MotorSentidoAntiHorario(int velocidade) {
    digitalWrite(motorIn1, LOW);
    analogWrite(motorIn2, velocidade);
    int tempoInicial = millis();
    while(millis()-tempoInicial < 3e3 or digitalRead(stopPin) == LOW){
    }
    digitalWrite(motorIn2, LOW);
}

void MotorSentidoHorario(int velocidade) {
    analogWrite(motorIn1, velocidade);
    digitalWrite(motorIn2, LOW);
    int tempoInicial = millis();
    while(millis()-tempoInicial < 3e3 or digitalRead(stopPin) == LOW){
    }
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
    MotorSentidoAntiHorario(500);
}

void FecharValvula(){
    MotorSentidoHorario(500);
}

void Irrigation(int duracao){
    Serial.print("Irrigação iniciada por "); Serial.print(duracao); Serial.println(" minutos");
    AbrirValvula();
    int tempoInicial = millis();
    while(millis() - tempoInicial < (duracao*1e3)*60){
        LigarAzul();
        delay(100);
        LigarVerde();
        delay(1000);
    }
    //delay((duracao*1e3)*60);
    FecharValvula();
    Serial.print("Irrigação finalizada.");

}