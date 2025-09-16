#include "connection.h"

ESP8266WebServer server(80);
const unsigned int WiFiSmartConfig_Timeout = 30000;
const unsigned int WiFiConnection_Timeout = 15000;
String globalSSID = "";
String globalPW = "";

void ConnectionError() {
    // Animação de erro por alguns segundos
    unsigned long duracao_animacao = 10000; // 10 segundos
    unsigned long tempo_inicial = millis();
    while (millis() - tempo_inicial < duracao_animacao) {
        LigarAzul();
        delay(100);
        LigarVerde();
        delay(100);
    }
    DesligarLeds(); // Desliga os LEDs após a animação de erro
}

void OTA_Initialization() {
    ArduinoOTA.setHostname("meutcc");
    ArduinoOTA.setPassword("12345");
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        // NOTA: Não faça nada que demore muito aqui, ou você pode ter um Watchdog reset
        Serial.println("Iniciando atualização " + type);
        // Desligue LEDs ou motores se estiverem ativos para sinalizar atualização
        DesligarLeds();
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nFim da atualização!");
        // Acenda um LED ou sinalize que a atualização terminou
        LigarVerde(); // Sinal de sucesso
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progresso: %u%%\r\n", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Erro[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Autenticação falhou");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Erro ao iniciar");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Erro de conexão");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Erro ao receber");
        else if (error == OTA_END_ERROR) Serial.println("Erro ao finalizar");
        // Sinalize um erro com os LEDs
        ConnectionError(); // Ou uma função específica para erro OTA
    });
    ArduinoOTA.begin();
    Serial.println("OTA inicializado. Aguardando atualizações...");
    // --- FIM DA CONFIGURAÇÃO OTA ---
}

void consulta_bateria(){
    String state;
    switch(EstadoBateria()){
        case ALTO:
        state = "Bateria alta";
        break;
        case MEDIO:
        state = "Bateria média";
        break;
        case BAIXO:
        state = "Bateria baixa";
        break;
    }
    server.send(200, "text/plain", state);
}

void SuccessfulConnection() {
    // Animação de sucesso por alguns segundos
    unsigned long duracao_animacao = 5000; // 5 segundos
    unsigned long tempo_inicial = millis();
    while (millis() - tempo_inicial < duracao_animacao) {
        DesligarLeds();
        delay(500); // 0.5 segundo
        LigarVerde();
        delay(500); // 0.5 segundo
    }
    LigarVerde(); // Deixa o LED verde ligado após a animação para indicar conexão
}

bool IniciarSmartConfig() {
    WiFi.mode(WIFI_STA); // Define o modo de estação
    Serial.println("Iniciando SmartConfig...");
    WiFi.beginSmartConfig(); // Inicia o modo SmartConfig

    unsigned long tempo_inicial = millis();
    while (millis() - tempo_inicial < WiFiSmartConfig_Timeout) {
        if (WiFi.status() == WL_CONNECTED) {
            // Se conectou, salva as novas credenciais
            String currentSSID = WiFi.SSID();
            String currentPW = WiFi.psk();
            SalvarCredenciais(currentSSID, currentPW); // Salva no LittleFS

            Serial.println("Conectado pelo SmartConfig e credenciais salvas!");
            Serial.print("SSID conectado: ");
            Serial.println(currentSSID);
            Serial.print("PW conectado: ");
            Serial.println(currentPW);
            return true; // Conectado e salvo
        }
        delay(1000); // Espera 1 segundo para o SmartConfig ou conexão
        Serial.print("."); // Feedback visual
    }
    Serial.println("\nSmartConfig Timeout.");
    return false; // SmartConfig não conectou a tempo
}

void SalvarCredenciais(const String& ssid, const String& password) {
    File configFile = LittleFS.open(CONFIG_FILE, "w"); // "w" para sobrescrever o arquivo
    if (!configFile) {
        Serial.println("Erro ao criar/abrir arquivo para salvar credenciais.");
        return;
    }

    configFile.println(ssid);
    configFile.println(password);
    configFile.close();
    Serial.println("Credenciais salvas no LittleFS.");
}

bool RecuperarCredenciaisWiFi() {
    File configFile = LittleFS.open(CONFIG_FILE, "r");
    if (!configFile) {
        Serial.println("Arquivo de configuração não encontrado ou erro ao abrir.");
        globalSSID = "";
        globalPW = "";
        return false;
    }

    globalSSID = configFile.readStringUntil('\n');
    globalPW = configFile.readStringUntil('\n'); // Lê a segunda linha para a senha
    configFile.close();

    // Remove espaços em branco ou caracteres de nova linha extras que podem ser lidos
    globalSSID.trim();
    globalPW.trim();

    Serial.print("SSID lido do LittleFS: ");
    Serial.println(globalSSID);
    Serial.print("PW lido do LittleFS: ");
    Serial.println(globalPW);

    if (globalSSID.length() > 0 && globalPW.length() > 0) {
        return true;
    } else {
        Serial.println("Credenciais vazias ou inválidas no arquivo.");
        return false;
    }
}

bool ConectarWiFiComCredenciaisSalvas() {
    if (!RecuperarCredenciaisWiFi()) { // Tenta carregar as credenciais
        Serial.println("Não foi possível carregar credenciais válidas do LittleFS.");
        return false;
    }

    Serial.print("Tentando conectar com SSID: ");
    Serial.println(globalSSID);

    // Conecta ao Wi-Fi
    WiFi.begin(globalSSID.c_str(), globalPW.c_str());

    unsigned long tempo_inicial = millis();
    while (millis() - tempo_inicial < WiFiConnection_Timeout) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nConectado com credenciais salvas!");
            return true; // Conectado com sucesso
        }
        delay(500); // Espera 0.5 segundo antes de verificar novamente
        Serial.print("."); // Feedback visual
    }
    Serial.println("\nTempo esgotado para conexão com credenciais salvas.");
    return false; // Tempo esgotado, não conectou
}
