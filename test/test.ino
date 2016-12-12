/*
BUFSM
~~~~~~~~
Send coordinates over wi-fi using ESP-8266

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid      = "USER";
const char* password  = "PASSW";
const char* host      = "bufsm.dalmago.xyz";
String url = "/api/things";
WiFiClient client;

char* lng[]={"-29.71946","-29.71943","-29.71936", "-29.71933", "-29.71927",
"-29.71909", "-29.71918", "-29.6887613", "29.71905", "-29.71897"};
char* lat[]={"-53.71774","-53.71722","-53.71681", "-53.7166", "-53.71616",
"-53.71534", "-53.71472", "-53.71424", " -53.71376", "-53.71316"};



void setup() {
  Serial.begin(115200);
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void loop() {
  for (int i = 0; i < 6; i++){

    String data = "{\"thing\":\"thing_name\", \"key\":\"TOKEN\", \"lat\":\"" +
                    String(lat[i]) + "\", \"lon\":\"" + String(lng[i]) + "\"}";

    if (client.connect(host, 80)){
      client.print(String("POST ") + url +
                   " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: " + data.length() + "\r\n" +
                   "\r\n" +
                   data + "\n");
      Serial.println("Dados enviados");
    }else{
      Serial.println("Erro na conexao");
    }
    client.stop();
    delay(5000);
    Serial.println("......");
    }

}
