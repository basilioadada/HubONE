/************************************************************/
/* Painel de Senhas com LCD 16x2 I2C                       */
/************************************************************/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Endereço do LCD (0x27 ou 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definições dos pinos
#define botao_DOWN 6
#define botao_UP 7
#define buzzer 8

int senha = -1;

void setup() {
  // Inicializa LCD I2C
  lcd.init();
  lcd.backlight();

  // Botões com pull-up interno
  pinMode(botao_UP, INPUT_PULLUP);
  pinMode(botao_DOWN, INPUT_PULLUP);

  // Buzzer
  pinMode(buzzer, OUTPUT);

  // Inicia com senha 0
  chamar(+1);
}

void loop() {
  if (senha > -1 && digitalRead(botao_UP) == 0) {
    chamar(+1);
    delay(200); // debounce
  }

  if (senha > 0 && digitalRead(botao_DOWN) == 0) {
    chamar(-1);
    delay(200); // debounce
  }
}

// Função que atualiza a senha
void chamar(int botao)
{
  tone(buzzer, 262, 250);
  delay(250);
  tone(buzzer, 2093, 400);
  delay(400);

  senha = senha + botao;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Painel Senhas");

  lcd.setCursor(0, 1);
  lcd.print("Senha: ");
  lcd.print(senha);
}