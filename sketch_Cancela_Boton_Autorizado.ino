#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 8
MFRC522 mfrc522(SS_PIN, RST_PIN);

Servo cancela;
int ledR = 3, ledG = 5, ledB = 6;
int buzzer = 7; // pino do buzzer

// Lista de UIDs autorizados
byte autorizados[][4] = {
  {0x23, 0xC1, 0xA2, 0xA5}, // Cartão 1
  {0x12, 0x34, 0x56, 0x78}  // Cartão 2 (exemplo)
};
int totalAutorizados = 2;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  cancela.attach(9);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(buzzer, OUTPUT);
  fecharCancela();
  Serial.println("Sistema pronto. Aproxime o cartão...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;
  
  Serial.print("UID detectado: ");
  for (byte i=0; i<mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (cartaoAutorizado()) {
    Serial.println("Cartão autorizado. Abrindo cancela...");
    somAutorizado();
    abrirCancela();
    delay(5000);
    Serial.println("Fechando cancela...");
    fecharCancela();
  } else {
    Serial.println("Cartão NÃO autorizado.");
    somBloqueio();
  }
}

bool cartaoAutorizado() {
  for (int c=0; c<totalAutorizados; c++) {
    bool ok = true;
    for (byte i=0; i<4; i++) {
      if (mfrc522.uid.uidByte[i] != autorizados[c][i]) ok = false;
    }
    if (ok) return true;
  }
  return false;
}

void abrirCancela() {
  setLED(0,0,255); // azul
  cancela.write(90);
  delay(500);
  setLED(0,255,0); // verde
  Serial.println("Cancela aberta.");
}

void fecharCancela() {
  cancela.write(0);
  setLED(255,0,0); // vermelho
  Serial.println("Cancela fechada.");
}

void setLED(int r, int g, int b) {
  analogWrite(ledR, r);
  analogWrite(ledG, g);
  analogWrite(ledB, b);
}

void somAutorizado() {
  tone(buzzer, 1000, 200); // beep agudo
  delay(250);
  tone(buzzer, 1200, 200); // segundo beep
}

void somBloqueio() {
  tone(buzzer, 400, 500); // beep grave e longo
}
