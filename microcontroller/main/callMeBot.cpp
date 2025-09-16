#include "callMeBot.h"

String phoneNumber = "+553189299861"; // <-- Coloque seu número aqui
String apiKey = "3231892"; // <-- Coloque sua API Key aqui
WiFiClientSecure client;

void enviarMensagem(String message){
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(client, url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.println("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void CallMeBotConfig(){
  client.setInsecure();
}