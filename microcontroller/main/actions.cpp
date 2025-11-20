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
    MotorSentidoHorario(500, 100);
}

void FecharValvula(){
    MotorSentidoAntiHorario(500, 100);
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
    }    FecharValvula();
    Serial.print("Irrigação finalizada.");
}