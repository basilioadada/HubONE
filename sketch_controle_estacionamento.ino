#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Configuração do LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo motor da cancela
Servo cancela;

// Pinos dos sensores
const int sensorEntrada = 2;   // HW-201 na entrada
const int sensorContagem = 3;  // IR após a cancela (conta entrada/saída)

// Variáveis de controle
int vagas = 3;
bool estadoEntradaAnterior = HIGH;
bool estadoContagemAnterior = HIGH;

void setup() {
  // Inicializa LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Estacionamento");
  lcd.setCursor(0,1);
  lcd.print("Vagas: ");
  lcd.print(vagas);

  // Servo
  cancela.attach(9);
  cancela.write(0); // cancela fechada

  // Sensores
  pinMode(sensorEntrada, INPUT);
  pinMode(sensorContagem, INPUT);
}

void loop() {
  // Leitura dos sensores
  int estadoEntrada = digitalRead(sensorEntrada);
  int estadoContagem = digitalRead(sensorContagem);

  // --- DETECTA CHEGADA ---
  if (estadoEntrada == LOW && estadoEntradaAnterior == HIGH && vagas > 0) {
    abrirCancela();
  }
  estadoEntradaAnterior = estadoEntrada;

  // --- CONTABILIZA ENTRADA ---
  if (estadoContagem == LOW && estadoContagemAnterior == HIGH) {
    if (vagas > 0) {
      vagas--;
      atualizarDisplay();
      fecharCancela();
    }
  }
  estadoContagemAnterior = estadoContagem;

  // --- CONTABILIZA SAÍDA ---
  // Supondo que o mesmo sensor detecta saída (carro passando no sentido inverso)
  // Aqui simplificamos: se cancela abrir sem vagas, é saída
  if (estadoEntrada == LOW && estadoEntradaAnterior == HIGH && vagas < 3) {
    abrirCancela();
    delay(2000); // tempo para carro passar
    vagas++;
    atualizarDisplay();
    fecharCancela();
  }
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
