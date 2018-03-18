#pragma once

#include "Pairs.h"

#define templateXPercent 0.2
#define templateYPercent 0.2 // Percentuais da tela que o template que será exibido ocupará (No comprimento e na largura).

void display();

void mouse(int, int, int, int);

void mouseMove(int, int);

void keyboard(unsigned char, int, int);

Pairf convertCoord(int, int);

int getWindowWidth();

int getWindowHeight();