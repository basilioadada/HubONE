#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo da cancela
Servo cancela;

// Pinos dos sensores
const int sensorEntrada = 2; // IR de entrada
const int sensorSaida   = 3; // IR de saída

// Variáveis
int vagas = 3; // total de vagas
bool estadoEntradaAnterior = HIGH;
bool estadoSaidaAnterior   = HIGH;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Estacionamento");
  lcd.setCursor(0,1);
  lcd.print("Vagas: ");
  lcd.print(vagas);

  cancela.attach(9);
  cancela.write(0); // cancela fechada

  pinMode(sensorEntrada, INPUT);
  pinMode(sensorSaida, INPUT);
}

void loop() {
  int estadoEntrada = digitalRead(sensorEntrada);
  int estadoSaida   = digitalRead(sensorSaida);

  // --- ENTRADA ---
  if (estadoEntrada == LOW && estadoEntradaAnterior == HIGH && vagas > 0) {
    abrirCancela();
    vagas--; // ocupa uma vaga
    atualizarDisplay();
    fecharCancela();
  }
  estadoEntradaAnterior = estadoEntrada;

  // --- SAÍDA ---
  if (estadoSaida == LOW && estadoSaidaAnterior == HIGH && vagas < 3) {
    abrirCancela();
    vagas++; // libera uma vaga
    atualizarDisplay();
    fecharCancela();
  }
  estadoSaidaAnterior = estadoSaida;
}

void abrirCancela() {
  cancela.write(90); // abre cancela
  delay(2000);       // tempo aberto
}

void fecharCancela() {
  cancela.write(0);  // fecha cancela
}

void atualizarDisplay() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Estacionamento");
  lcd.setCursor(0,1);
  lcd.print("Vagas: ");
  lcd.print(vagas);
}
