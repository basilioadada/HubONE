#include <LedControl.h>

// Pinos para os 3 módulos MAX7219
LedControl lc = LedControl(12, 11, 10, 3); // (DIN, CLK, CS, nº de matrizes)

const int botao = 2;
const int buzzer = 3; // pino do buzzer

// Emoções em 8x8
int emotions[4][8] = {
  {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C}, // Feliz
  {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C}, // Triste
  {0x3C,0x42,0xA5,0x81,0xBD,0xA5,0x42,0x3C}, // Bravo
  {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C}  // Cool
};

// Estrela para vitória
byte estrela[8] = {
  0x10,0x38,0x7C,0xFE,0x7C,0x38,0x10,0x00
};

// X para derrota
byte xShape[8] = {
  0x81, // *      *
  0x42, //  *    * 
  0x24, //   *  *  
  0x18, //    **   
  0x18, //    **   
  0x24, //   *  *  
  0x42, //  *    * 
  0x81  // *      *
};

void setup() {
  pinMode(botao, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  for (int i = 0; i < 3; i++) {
    lc.shutdown(i,false);
    lc.setIntensity(i,8);
    lc.clearDisplay(i);
  }
  randomSeed(analogRead(0)); // inicializa aleatório
}

void mostraEmoji(int matriz, int emoji) {
  for (int linha = 0; linha < 8; linha++) {
    lc.setRow(matriz, linha, emotions[emoji][linha]);
  }
}

void mostraEstrela(int matriz) {
  for (int linha = 0; linha < 8; linha++) {
    lc.setRow(matriz, linha, estrela[linha]);
  }
}

void mostraX(int matriz) {
  for (int linha = 0; linha < 8; linha++) {
    lc.setRow(matriz, linha, xShape[linha]);
  }
}

void somVitoria() {
  tone(buzzer, 1000, 200);
  delay(250);
  tone(buzzer, 1200, 200);
  delay(250);
  tone(buzzer, 1500, 300);
  delay(350);
  noTone(buzzer);
}

void somDerrota() {
  tone(buzzer, 400, 300);
  delay(350);
  tone(buzzer, 300, 400);
  delay(450);
  noTone(buzzer);
}

void loop() {
  if (digitalRead(botao) == LOW) {
    delay(200); // debounce
    int sorteio[3];
    for (int i = 0; i < 3; i++) {
      sorteio[i] = random(0,4);
      mostraEmoji(i, sorteio[i]);
    }

    // Verifica vitória ou derrota
    if (sorteio[0] == sorteio[1] && sorteio[1] == sorteio[2]) {
      // animação de vitória: piscar 3 vezes
      for (int pisca = 0; pisca < 3; pisca++) {
        for (int k = 0; k < 3; k++) {
          lc.clearDisplay(k);
        }
        delay(300);
        for (int k = 0; k < 3; k++) {
          mostraEmoji(k, sorteio[k]);
        }
        delay(300);
      }
      // mostra estrela final
      for (int k = 0; k < 3; k++) {
        mostraEstrela(k);
      }
      somVitoria();
    } else {
      // animação de derrota: X piscando 3 vezes
      for (int pisca = 0; pisca < 3; pisca++) {
        for (int k = 0; k < 3; k++) {
          lc.clearDisplay(k);
        }
        delay(300);
        for (int k = 0; k < 3; k++) {
          mostraX(k);
        }
        delay(300);
      }
      // deixa o X fixo
      for (int k = 0; k < 3; k++) {
        mostraX(k);
      }
      somDerrota();
    }
  }
}
