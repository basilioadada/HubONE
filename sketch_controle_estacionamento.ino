#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// --- Objetos ---
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C pode variar (0x3F em alguns módulos)
Servo cancela;

// --- Pinos ---
const int trigPin = 8;
const int echoPin = 9;
const int irEntrada = 2;
const int irSaida = 3;
const int servoPin = 6;

// --- Variáveis ---
long duracao;
int distancia;
int vagas = 3; // total de vagas

// --- Setup ---
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irEntrada, INPUT);
  pinMode(irSaida, INPUT);

  cancela.attach(servoPin);
  cancela.write(0); // posição inicial (fechada)

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Estacionamento");
  lcd.setCursor(0,1);
  lcd.print("Vagas: ");
  lcd.print(vagas);
}

// --- Função para medir distância ---
int medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracao = pulseIn(echoPin, HIGH);
  distancia = duracao * 0.034 / 2; // cm
  return distancia;
}

// --- Loop principal ---
void loop() {
  int dist = medirDistancia();

  // Veículo detectado na entrada
  if (dist < 15 && vagas > 0) {
    cancela.write(90); // abre cancela
    delay(3000);       // tempo para passar
    cancela.write(0);  // fecha cancela
  }

  // Contabiliza entrada
  if (digitalRead(irEntrada) == LOW && vagas > 0) {
    vagas--;
    atualizarDisplay();
    delay(1000); // debounce
  }

  // Contabiliza saída
  if (digitalRead(irSaida) == LOW && vagas < 3) {
    vagas++;
    atualizarDisplay();
    // abre cancela para saída
    cancela.write(90);
    delay(3000);
    cancela.write(0);
    delay(1000); // debounce
  }
}

// --- Atualiza display ---
void atualizarDisplay() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Estacionamento");
  lcd.setCursor(0,1);
  lcd.print("Vagas: ");
  lcd.print(vagas);
}
