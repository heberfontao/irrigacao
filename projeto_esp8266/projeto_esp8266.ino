
#include <ESP8266WiFi.h>
#include "config.h"

AdafruitIO_Feed *sensor = io.feed("sensor");
AdafruitIO_Feed *valvula = io.feed("valvula");

void controlValve(AdafruitIO_Data *data) {
  // Placeholder para a lógica de controle da válvula
  String command = data->value();
  // Enviar o comando de volta para o Arduino através da Serial
  Serial.println(command);
}

void connectToWiFi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  // Inicializa a comunicação Serial com o Arduino
  Serial.begin(9600);

  // Conectar à rede Wi-Fi
  connectToWiFi();
  
  // Conectar ao Adafruit IO
  io.connect();

  // Esperar até estar conectado
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Conectado ao Adafruit IO!");
}

void loop() {
  // Verifica se há dados disponíveis na Serial para ler
  if (Serial.available() > 0) {
    // Ler a linha de dados da Serial
    String data = Serial.readStringUntil('\n'); // Ler até o fim de linha

    if (data.startsWith("U")) {
      // Dados de umidade do solo
      int umidade = data.substring(1).toInt();
      sensor->save(umidade);
    } else if (data.startsWith("V")) {
      // Dados de ativação da válvula
      int contagemValvula = data.substring(1).toInt();
      valvula->save(contagemValvula);
    }
  }
}
