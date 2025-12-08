#include "actions.h"

void _counterclockwise_motor(int velocidade) {
    digitalWrite(in1_motor, LOW);
    analogWrite(in2_motor, velocidade);
    int tempoInicial = millis();
    while(millis()-tempoInicial < 3e3 or digitalRead(stop_pin) == LOW){
    }
    digitalWrite(in2_motor, LOW);
}

void _clockwise_motor(int speed) {
    analogWrite(in1_motor, speed);
    digitalWrite(in2_motor, LOW);
    int initial_time = millis();
    while(millis()-initial_time < 3e3 or digitalRead(stop_pin) == LOW){
    }
    digitalWrite(in1_motor, LOW);
}

void _leds_off() {
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, LOW);
}

void _green_led_on() {
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, HIGH);
}

void _red_led_on() {
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, HIGH);
}

void _open_valve(){
    _counterclockwise_motor(500);
}

void _close_valve(){
    _clockwise_motor(500);
}

void irrigation(int duration){
    Serial.print("Irrigação iniciada por "); Serial.print(duration); Serial.println(" minutos");
    _open_valve();
    int initial_time = millis();
    while(millis() - initial_time < (duration*1e3)*60){
        _red_led_on();
        delay(100);
        _green_led_on();
        delay(1000);
    }
    //delay((duracao*1e3)*60);
    _close_valve();
    Serial.print("Irrigação finalizada.");

}