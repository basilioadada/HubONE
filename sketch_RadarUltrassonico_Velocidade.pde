import processing.serial.*;

Serial myPort;
float anguloAtual = 0;
float distanciaAtual = 0;
float velocidadeExibida = 0;
int tempoUltimaVelocidade = 0;

float MAX_DIST = 150.0; // Limite do radar (150 cm)
float MIN_DIST = 8.0;   // Filtro para ruídos colados ao sensor (< 8 cm)

class Alvo {
  float ang, dist, vel;
  int tempoCriacao;

  Alvo(float a, float d, float v) {
    ang = a;
    dist = d;
    vel = v;
    tempoCriacao = millis();
  }
}

Alvo[] radarMap = new Alvo[181];

void setup() {
  size(800, 600);
  // Atualizado para 115200 baud (mesma velocidade configurada no Arduino)
  myPort = new Serial(this, "COM6", 115200); // Ajuste a porta COM do seu computador
  smooth();
}

void draw() {
  // --- SISTEMA ANTI-TRAVAMENTO DA PORTA SERIAL ---
  // Se o buffer acumular mais de 200 bytes, limpa o excesso para eliminar atrasos
  if (myPort.available() > 200) {
    myPort.clear();
  }

  // Processa até 10 linhas pendentes por ciclo para manter a renderização a 60 FPS
  int linhasLidas = 0;
  while (myPort.available() > 0 && linhasLidas < 10) {
    String inString = myPort.readStringUntil('\n');
    if (inString != null) {
      processarLinha(trim(inString));
    }
    linhasLidas++;
  }

  background(0);

  // Retém o valor de velocidade por 2,5 segundos
  if (millis() - tempoUltimaVelocidade > 2500) {
    velocidadeExibida = 0.0;
  }

  // Geometria do Radar (Origem rebaixada para expandir o arco de 180° em 800x600)
  float centroX = width / 2.0;
  float centroY = height - 50;
  float radMax = 420.0;
  float escalaPx = radMax / MAX_DIST;

  // --- GRAFISMO DO RADAR ---
  pushMatrix();
  translate(centroX, centroY);

  // Arcos Concêntricos graduados de 30 em 30 cm até 150 cm
  stroke(0, 90, 0);
  noFill();
  strokeWeight(1);
  for (int d = 30; d <= 150; d += 30) {
    float rPx = d * escalaPx;
    arc(0, 0, rPx * 2, rPx * 2, PI, TWO_PI);
  }

  // Linhas Radiais (0° a 180°)
  for (int a = 0; a <= 180; a += 30) {
    float rad = radians(a);
    line(0, 0, radMax * cos(rad), -radMax * sin(rad));
  }

  // Linha de Varredura Principal
  stroke(0, 255, 0, 220);
  strokeWeight(2);
  float radSweep = radians(anguloAtual);
  line(0, 0, radMax * cos(radSweep), -radMax * sin(radSweep));
  strokeWeight(1);

  // Renderização dos Alvos
  int agora = millis();
  for (int i = 0; i <= 180; i++) {
    Alvo obj = radarMap[i];
    if (obj != null) {
      if (agora - obj.tempoCriacao > 3000) {
        radarMap[i] = null;
        continue;
      }

      float rad = radians(obj.ang);
      float distPx = obj.dist * escalaPx;
      float x = distPx * cos(rad);
      float y = -distPx * sin(rad);

      if (obj.vel > 0) {
        fill(255, 50, 50);
        noStroke();
        ellipse(x, y, 12, 12);

        fill(255);
        textSize(12);
        textAlign(LEFT, CENTER);
        text(int(obj.dist) + "cm | " + nf(obj.vel, 0, 1) + " cm/s", x + 10, y - 5);
      } else {
        fill(0, 255, 0);
        noStroke();
        ellipse(x, y, 6, 6);
      }
    }
  }
  popMatrix();

  // --- PAINEL DE DADOS E RODAPÉ (Coordenadas Fixas) ---
  fill(0, 255, 0);
  textSize(15);
  textAlign(LEFT, TOP);
  text("Radar Ultrassónico (Java)", 15, 15);

  textAlign(RIGHT, TOP);
  textSize(13);
  text("Ângulo: " + int(anguloAtual) + "°", width - 15, 15);
  text("Distância: " + nf(distanciaAtual, 0, 1) + " cm", width - 15, 35);

  if (velocidadeExibida > 0) {
    fill(255, 50, 50);
    textSize(18);
    text("VELOCIDADE: " + nf(velocidadeExibida, 0, 1) + " cm/s", width - 15, 60);
  } else {
    fill(0, 200, 200);
    textSize(13);
    text("Velocidade: 0.0 cm/s", width - 15, 60);
  }

  // RODAPÉ - Identificação do Professor
  fill(0, 255, 0);
  textSize(15);
  textAlign(RIGHT, BOTTOM);
  text("Prof BASILIO ADADA", width - 15, height - 15);
}

void processarLinha(String inString) {
  String[] parts = split(inString, ':');
  if (parts.length >= 3) {
    try {
      float a = float(parts[0]);
      float d = float(parts[1]);
      float v = float(parts[2]);

      anguloAtual = a;
      distanciaAtual = d;

      if (v > 0) {
        velocidadeExibida = v;
        tempoUltimaVelocidade = millis();
      }

      int idx = int(constrain(a, 0, 180));

      if (d >= MIN_DIST && d <= MAX_DIST) {
        radarMap[idx] = new Alvo(a, d, v);
      } else {
        radarMap[idx] = null;
      }
    } catch (Exception e) {
      // Descarta linhas corrompidas durante o processamento
    }
  }
}
