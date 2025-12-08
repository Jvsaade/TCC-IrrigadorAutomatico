#include "bateria.h"


BatteryState battery_state(){
    int leitura_bateria = analogRead(PINO_ADC_BATERIA);
    float battery_voltage = (((float) leitura_bateria)/1023.0)*FATOR_DIVISOR;
    if(battery_voltage > VOLTAGEM_ALTA)
    {
        return ALTO;
    }
    else{
        if(battery_voltage > VOLTAGEM_BAIXA)
            return MEDIO;
        else
            return BAIXO;
    }
}

float battery_voltage(){
    int leitura_bateria = analogRead(PINO_ADC_BATERIA);
    float battery_voltage = (((float) leitura_bateria)/1023.0)*FATOR_DIVISOR;
    return battery_voltage;
}