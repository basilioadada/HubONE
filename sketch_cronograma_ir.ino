#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensorInicio = 7;
int sensorFim = 8;

unsigned long tempoInicio = 0;
unsigned long tempoFim = 0;
unsigned long tempoTotal = 0;

bool cronometroAtivo = false;
bool medicaoFinalizada = false;

void setup() {
  pinMode(sensorInicio, INPUT);
  pinMode(sensorFim, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Sistema Tempo");
  delay(2000);
  lcd.clear();
}

void loop() {
  int estadoInicio = digitalRead(sensorInicio);
  int estadoFim = digitalRead(sensorFim);

  // INICIO (SAIDA)
  if (estadoInicio == LOW && !cronometroAtivo) {
    tempoInicio = millis();
    cronometroAtivo = true;
    medicaoFinalizada = false;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("INICIO...");
    delay(500);
  }

  // FIM (CHEGADA)
  if (estadoFim == LOW && cronometroAtivo && !medicaoFinalizada) {
    tempoFim = millis();
    tempoTotal = tempoFim - tempoInicio;

    cronometroAtivo = false;
    medicaoFinalizada = true;

    float segundos = tempoTotal / 1000.0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tempo gasto:");

    lcd.setCursor(0, 1);
    lcd.print(segundos);
    lcd.print(" s");

    delay(3000);
  }

  // STATUS
  if (!cronometroAtivo && !medicaoFinalizada) {
    lcd.setCursor(0, 0);
    lcd.print("Aguardando...");
    lcd.setCursor(0, 1);
    lcd.print("Passe no inicio");
  }
}
