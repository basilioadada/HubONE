#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 6;

Servo cancela;

long duracao;
int distancia;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  cancela.attach(servoPin);
  cancela.write(0); // posição inicial (cancela fechada)
  Serial.begin(9600);
}

void loop() {
  // Envia pulso para o sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calcula tempo de resposta
  duracao = pulseIn(echoPin, HIGH);
  distancia = duracao * 0.034 / 2; // distância em cm

  Serial.print("Distancia: ");
  Serial.println(distancia);

  // Se detectar veículo a menos de 20 cm
  if (distancia > 0 && distancia < 20) {
    cancela.write(90); // abre cancela
    delay(5000);       // espera 5 segundos
    cancela.write(0);  // fecha cancela
  }

  delay(200);
}
