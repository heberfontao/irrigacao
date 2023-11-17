
#include <SoftwareSerial.h>

// Configuração dos pinos para SoftwareSerial
SoftwareSerial espSerial(10, 11); // RX, TX

// Variáveis do programa
const int Sensor = A0;
const int Valvula = 12;
const int limiarSeco = 60;
const int tempoRega = 10; // Tempo de rega em segundos
int umidadeSolo = 0;
int valveActivationCount = 0;

void setup() {
  pinMode(Valvula, OUTPUT);
  digitalWrite(Valvula, HIGH); // Desliga a válvula inicialmente

  // Inicializa os LEDs
  pinMode(5, OUTPUT); // Vermelho
  pinMode(6, OUTPUT); // Amarelo
  pinMode(7, OUTPUT); // Verde

  // Inicializa a comunicação Serial com o computador
  Serial.begin(9600);
  // Inicializa a comunicação Serial com o ESP8266
  espSerial.begin(9600);
}

void loop() {
  // Média das leituras de umidade
  int somaUmidade = 0;
  for (int i = 0; i < 5; i++) {
    somaUmidade += analogRead(Sensor);
    delay(10000); // Espera 10 segundos para a próxima leitura
  }
  umidadeSolo = somaUmidade / 5; // Calcula a média
  umidadeSolo = map(umidadeSolo, 1023, 0, 0, 100); // Converte para porcentagem

  // Envia a umidade do solo para o ESP8266
  espSerial.println("U" + String(umidadeSolo));

  // Lógica de controle da válvula
  if (umidadeSolo < limiarSeco) {
    digitalWrite(5, HIGH); // Vermelho
    digitalWrite(7, LOW);  // Verde
    digitalWrite(Valvula, LOW); // Liga a válvula
    delay(tempoRega * 1000); // Mantém a válvula ligada pelo tempo definido
    digitalWrite(Valvula, HIGH); // Desliga a válvula
    digitalWrite(5, LOW);   // Vermelho
    digitalWrite(6, HIGH);  // Amarelo
    delay(5000); // Espera um pouco antes de fazer a próxima leitura
    digitalWrite(6, LOW);   // Amarelo
    valveActivationCount++;
    // Envia a contagem de ativação da válvula para o ESP8266
    espSerial.println("V" + String(valveActivationCount));
  } else {
    digitalWrite(6, LOW);   // Amarelo
    digitalWrite(7, HIGH);  // Verde
    delay(60000); // Espera até a próxima leitura
  }
}
