#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço pode ser 0x27 ou 0x3F

const int botao = 2;
const int buzzer = 8;
const int ledVerde = 9;
const int ledVermelho = 10;

unsigned long tempoPressionado;
unsigned long ultimaAcao;
String morse = "";
String mensagem = "";

// Tabela Morse simplificada (A–Z)
String letras[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
                   ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
                   "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

char alfabeto[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M',
                   'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

void setup() {
  pinMode(botao, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  lcd.init();
  lcd.backlight();

  // Primeira linha inicial
  lcd.setCursor(0,0);
  lcd.print("Morse HUbONE");

  // Segunda linha fixa
  lcd.setCursor(0,1);
  lcd.print("Pf Basilio Adada");
}

void loop() {
  if (digitalRead(botao) == LOW) {
    tempoPressionado = millis();
    while (digitalRead(botao) == LOW);
    tempoPressionado = millis() - tempoPressionado;

    if (tempoPressionado < 300) {
      morse += ".";
      tone(buzzer, 1000, 200); // som curto
      digitalWrite(ledVerde, HIGH);
      delay(200);
      digitalWrite(ledVerde, LOW);
    } else {
      morse += "-";
      tone(buzzer, 1000, 600); // som longo
      digitalWrite(ledVermelho, HIGH);
      delay(600);
      digitalWrite(ledVermelho, LOW);
    }
    ultimaAcao = millis();
  }

  // Pausa entre letras
  if (morse.length() > 0 && millis() - ultimaAcao > 1000) {
    for (int i=0; i<26; i++) {
      if (morse == letras[i]) {
        mensagem += alfabeto[i];
        break;
      }
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(mensagem);

    // Mantém a segunda linha fixa
    lcd.setCursor(0,1);
    lcd.print("Prof Basilio Adada");

    morse = "";
  }

  // Pausa longa entre palavras
  if (mensagem.length() > 0 && millis() - ultimaAcao > 3000) {
    mensagem += " ";
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(mensagem);

    // Mantém a segunda linha fixa
    lcd.setCursor(0,1);
    lcd.print("Prof Basilio Adada");

    ultimaAcao = millis();
  }
}
