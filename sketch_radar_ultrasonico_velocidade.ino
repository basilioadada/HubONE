#include <Servo.h>

Servo servo;
const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 6;

const float DIST_LIMITE = 150.0; // Limite de 1,5 metros (150 cm)
const int TIMEOUT_US = 10000;    // Timeout curto (~1,7m max) para estabilidade

void setup() {
  Serial.begin(115200); // Manta a comunicação rápida
  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// Disparo rápido do sensor ultrassónico
float dispararSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH, TIMEOUT_US); 
  if (duracao == 0) return 999.0;

  return (duracao * 0.0343) / 2.0;
}

// Média de 2 leituras rápidas para estabilização
float medirDistancia() {
  float d1 = dispararSensor();
  if (d1 > DIST_LIMITE || d1 <= 0) return 999.0;

  delay(6); // Intervalo para dissipar o eco no ambiente

  float d2 = dispararSensor();
  if (d2 > DIST_LIMITE || d2 <= 0) return d1;

  return (d1 + d2) / 2.0;
}

// Rastreio para cálculo da velocidade de aproximação
void rastrearEVelocidade(int angulo, float d1) {
  unsigned long t1 = millis();

  delay(100); // Janela de amostragem adequada à rotação mais lenta

  float d2 = medirDistancia();
  unsigned long t2 = millis();

  if (d2 <= DIST_LIMITE && d2 > 0) {
    float deltaD = d1 - d2;             // cm (positivo indica aproximação)
    float deltaT = (t2 - t1) / 1000.0;  // segundos

    float vel = (deltaT > 0) ? (deltaD / deltaT) : 0.0;
    float velAproximacao = (vel > 0.8) ? vel : 0.0;

    Serial.print(angulo);
    Serial.print(":");
    Serial.print(d2, 1);
    Serial.print(":");
    Serial.println(velAproximacao, 1);
  } else {
    Serial.print(angulo);
    Serial.print(":");
    Serial.print(d1, 1);
    Serial.println(":0.0");
  }
}

void loop() {
  // Varredura de ida (0° a 180°) - Rotação mais lenta
  for (int angulo = 0; angulo <= 180; angulo += 5) {
    servo.write(angulo);
    delay(80); // Aumentado de 35ms para 80ms (diminui a velocidade do radar)
    float dist = medirDistancia();

    if (dist <= DIST_LIMITE && dist > 2.0) {
      rastrearEVelocidade(angulo, dist);
    } else {
      Serial.print(angulo);
      Serial.print(":");
      Serial.print(dist, 1);
      Serial.println(":0.0");
    }
  }

  // Varredura de volta (180° a 0°) - Rotação mais lenta
  for (int angulo = 180; angulo >= 0; angulo -= 5) {
    servo.write(angulo);
    delay(80); // Aumentado de 35ms para 80ms
    float dist = medirDistancia();

    if (dist <= DIST_LIMITE && dist > 2.0) {
      rastrearEVelocidade(angulo, dist);
    } else {
      Serial.print(angulo);
      Serial.print(":");
      Serial.print(dist, 1);
      Serial.println(":0.0");
    }
  }
}