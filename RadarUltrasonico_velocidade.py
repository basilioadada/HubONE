import sys
import math
import time
import serial
import pygame

COM_PORT = 'COM6'  # Altere para a sua porta COM
BAUD_RATE = 115200  # Aumentado para corresponder ao Arduino

try:
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=0.01)
    time.sleep(2)
except Exception as e:
    print(f"Erro ao abrir a porta {COM_PORT}: {e}")
    sys.exit()

pygame.init()
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Radar Ultrassónico - Medidor de Velocidade")
clock = pygame.time.Clock()

CENTER = (WIDTH // 2, HEIGHT - 50)
RAD_MAX = 420

BLACK = (0, 0, 0)
GREEN = (0, 255, 0)
DARK_GREEN = (0, 90, 0)
RED = (255, 50, 50)
CYAN = (0, 200, 200)
WHITE = (255, 255, 255)

font_small = pygame.font.SysFont("Arial", 13)
font_med = pygame.font.SysFont("Arial", 15, bold=True)
font_large = pygame.font.SysFont("Arial", 18, bold=True)

angulo_atual = 0.0
distancia_atual = 0.0
velocidade_exibida = 0.0
tempo_ultima_velocidade = 0.0

MAX_DIST = 150.0
MIN_DIST = 8.0
ESCALA_PX = RAD_MAX / MAX_DIST

radar_map = [None] * 181

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # EVITA TRAVAMENTO: Se o buffer acumular mais de 150 bytes, descarta o atraso
    if ser.in_waiting > 150:
        ser.reset_input_buffer()

    # Leitura limitada a 5 linhas por quadro para manter os 60 FPS fluídos
    linhas_lidas = 0
    while ser.in_waiting > 0 and linhas_lidas < 5:
        try:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            parts = line.split(':')
            if len(parts) >= 3:
                a = float(parts[0])
                d = float(parts[1])
                v = float(parts[2])

                angulo_atual = a
                distancia_atual = d

                if v > 0:
                    velocidade_exibida = v
                    tempo_ultima_velocidade = time.time()

                idx = int(max(0, min(180, a)))
                if MIN_DIST <= d <= MAX_DIST:
                    radar_map[idx] = {'dist': d, 'vel': v, 'time': time.time()}
                else:
                    radar_map[idx] = None
        except (ValueError, IndexError):
            pass
        linhas_lidas += 1

    agora = time.time()
    if agora - tempo_ultima_velocidade > 2.5:
        velocidade_exibida = 0.0

    screen.fill(BLACK)

    # Desenho da Grade Semicircular
    for d_marca in range(30, 151, 30):
        r_px = int(d_marca * ESCALA_PX)
        rect = pygame.Rect(CENTER[0] - r_px, CENTER[1] - r_px, r_px * 2, r_px * 2)
        pygame.draw.arc(screen, DARK_GREEN, rect, 0, math.pi, 1)

    for a in range(0, 181, 30):
        rad = math.radians(a)
        x = CENTER[0] + RAD_MAX * math.cos(rad)
        y = CENTER[1] - RAD_MAX * math.sin(rad)
        pygame.draw.line(screen, DARK_GREEN, CENTER, (x, y), 1)

    # Linha de Varredura
    rad_sweep = math.radians(angulo_atual)
    sweep_x = CENTER[0] + RAD_MAX * math.cos(rad_sweep)
    sweep_y = CENTER[1] - RAD_MAX * math.sin(rad_sweep)
    pygame.draw.line(screen, GREEN, CENTER, (sweep_x, sweep_y), 2)

    # Alvos Detectados
    for i in range(181):
        obj = radar_map[i]
        if obj is not None:
            if agora - obj['time'] > 3.0:
                radar_map[i] = None
                continue

            rad = math.radians(i)
            dist_px = obj['dist'] * ESCALA_PX
            px = CENTER[0] + dist_px * math.cos(rad)
            py = CENTER[1] - dist_px * math.sin(rad)

            if obj['vel'] > 0:
                pygame.draw.circle(screen, RED, (int(px), int(py)), 7)
                txt = font_small.render(f"{int(obj['dist'])}cm | {obj['vel']:.1f} cm/s", True, WHITE)
                screen.blit(txt, (px + 10, py - 10))
            else:
                pygame.draw.circle(screen, GREEN, (int(px), int(py)), 4)

    # HUD / Painel
    screen.blit(font_med.render("Radar Ultrassónico (Python)", True, GREEN), (15, 15))
    screen.blit(font_small.render(f"Ângulo: {int(angulo_atual)}°", True, GREEN), (WIDTH - 160, 15))
    screen.blit(font_small.render(f"Distância: {distancia_atual:.1f} cm", True, GREEN), (WIDTH - 160, 35))

    if velocidade_exibida > 0:
        screen.blit(font_large.render(f"VELOCIDADE: {velocidade_exibida:.1f} cm/s", True, RED), (WIDTH - 260, 60))
    else:
        screen.blit(font_small.render("Velocidade: 0.0 cm/s", True, CYAN), (WIDTH - 160, 60))

    footer_txt = font_med.render("Prof BASILIO ADADA", True, GREEN)
    footer_rect = footer_txt.get_rect(bottomright=(WIDTH - 15, HEIGHT - 15))
    screen.blit(footer_txt, footer_rect)

    pygame.display.flip()
    clock.tick(60)

ser.close()
pygame.quit()
sys.exit()