#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <String.h> // Não é estritamente necessário para LittleFS, mas mantém compatibilidade


#include "bateria.h"
#include "connection.h"
#include "actions.h"
#include "callMeBot.h"
#include "alarm.h"
#include "servidor.h"
#include "horario.h"


#define MAX_STRING_LENGTH 64 // Aumentado para dar mais folga (pode ser 32 se preferir, mas 64 é mais seguro)

#define DEEPSLEEP_TIMEOUT 3*60000 // 30 segundos para DeepSleep
#if DEEPSLEEP_TIMEOUT < 180000
  #error "O valor deve ser maior que 180.000"
#endif
unsigned int contagem;

// Não precisamos mais da struct global se lermos e escrevermos diretamente do arquivo.
// As credenciais serão lidas em variáveis locais ou passadas para as funções conforme necessário.

// --- Protótipos das funções ---
bool _recover_wifi_credentials(); // Retorna true se as credenciais foram lidas com sucesso
bool connect_wifi_saved_credentials(); // Substitui WiFiEEPROM()
bool smartconfig_init(); // Substitui WiFiSmartConfig()
void _save_credentials(const String& ssid, const String& password); // Agora aceita Strings
void OTA_init();
void piscapisca();

// Variáveis para armazenar as credenciais lidas do LittleFS

void setup() {
    Serial.begin(115200);

    // Inicializa o LittleFS
    if (!LittleFS.begin()) {
        Serial.println("Erro ao montar o LittleFS. Formatando...");
        // Se houver um erro na montagem, tente formatar (isso apaga todos os dados!)
        LittleFS.format();
        if (!LittleFS.begin()) {
            Serial.println("Falha crítica: Não foi possível montar o LittleFS mesmo após formatar.");
            // Você pode adicionar um loop infinito ou reinício aqui se for um erro fatal
            while (true) {
                connection_error(); // Exibe um erro visual contínuo
                delay(1000);
            }
        }
    }
    Serial.println("LittleFS montado com sucesso.");

    // Configura os pinos digitais
    pinMode(green_led, OUTPUT);
    pinMode(red_led, OUTPUT);
    pinMode(in1_motor, OUTPUT);
    pinMode(in2_motor, OUTPUT);
    pinMode(stop_pin, INPUT);

    Serial.println("\nIniciando...");

    // Tenta conectar usando as credenciais salvas no LittleFS
    if (connect_wifi_saved_credentials() == false) {
        Serial.println("Erro na conexão por credenciais salvas no LittleFS.");
        Serial.println("Iniciando tentativa por SmartConfig.");
        connection_error(); // Exibe erro de conexão inicial (com duração)

        // Se falhou, tenta SmartConfig
        if (smartconfig_init() == false) {
            Serial.println("Erro na conexão por SmartConfig.");
            connection_error(); // Exibe erro de conexão final (com duração)
            // Se falhar a conexão, o LED azul piscará por 10 segundos
        } else {
            successful_connection(); // Conexão bem-sucedida via SmartConfig (com duração)
        }
    } else {
        successful_connection(); // Conexão bem-sucedida via LittleFS (com duração)
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("WiFi conectado! IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("Não foi possível conectar ao WiFi.");
    }

    // Inicia mDNS para acesso fácil pelo nome
    if (MDNS.begin("meutcc")) {
        Serial.println("mDNS iniciado com sucesso!");
        MDNS.addService("http", "tcp", 80);
    } else {
        Serial.println("Falha ao iniciar mDNS.");
    }

    // Inicializa a atualização OTA
    OTA_init();
    

    // Configura o servidor web
    server.on("/", []() {
        server.send(200, "text/plain", "Rodando...");
    });
    server_init();
    server.on("/battery", _battery_consult); // Rota para testar a bateria
    server.begin(); // Inicia o servidor web
    Serial.println("Servidor HTTP iniciado.");
    contagem = millis();

    call_me_bot_init();
    time_init();

    if(battery_state()==BAIXO)
        send_message("Bateria baixa! Troque quando possível.");

}

void loop() {
    MDNS.update();
    ArduinoOTA.handle();
    server.handleClient();

    int duracao = check_schedule();
    if(duracao != 0)
        irrigation(duracao);

    Serial.print("O próximo alarme está a ");Serial.print(deep_sleep_time());Serial.println(" milisegundos a frente no tempo.");
    /* 
    Entra em deepsleep quando passarem DEEPSLEEP_TIMEOUT milisegundos desde que ele entrou no loop.
    OBS: DEEPSLEEP_TIMEOUT não pode ser um número muito pequeno, pois deve ter tempo suficiente para
    permitir que o usuário configure pelo OTA.
    */

    if (millis() - contagem > DEEPSLEEP_TIMEOUT) {
        int wait = deep_sleep_time();
        if (wait > 5e6){
            ESP.deepSleep(wait);
            Serial.print("Entrando em DeepSleep por "); Serial.print(wait/60e6); Serial.println(" minutos.");
        }
        else {
            contagem += 6e3*60; 
        }
    }

    // if (millis() - contagem > DEEPSLEEP_TIMEOUT) {
    //     Serial.println("Entrando em deepsleep por 15 segundos");
    //     ESP.deepSleep(15e6);
    // }
}

