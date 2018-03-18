#include "Interface.hpp"

#include "glut.h"
#include "Graficos.hpp"
#include <iostream>
using namespace std;
#include "Pattern.hpp"
#include "Configurations.h"

int WINDOW_W = STARTING_WINDOW_WIDTH;
int WINDOW_H = STARTING_WINDOW_HEIGHT;

void updateWindowSize() {
	WINDOW_W = glutGet(GLUT_WINDOW_WIDTH);
	WINDOW_H = glutGet(GLUT_WINDOW_HEIGHT);
}

void display() {

	glClearColor(1, 1, 1, 1);
	glClear( GL_COLOR_BUFFER_BIT );

	updateWindowSize();

	glColor3f(0, 0, 0);

	resetGraphics();
	translateGraphics(templateXPercent-1., 1.-templateYPercent);
	scaleGraphics(templateXPercent, templateYPercent);

		drawPattern();

		if (DRAW_PATTERN_GRID)
			drawPatternGrid();

		glColor3f(1, 0, 0);

		drawBorder();

	resetGraphics();


	translateGraphics(0, -0.22);
	scaleGraphics(0.75, 0.75);

	glColor3f(0, 0, 0);

		drawTexture();

		if (DRAW_TEXTURE_GRID)
			drawTextureGrid();

	glColor3f(1, 0, 0);

		drawBorder();

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {

}

void mouseMove(int x, int y) {

}

void keyboard(unsigned char key, int x, int y) {

	if (key == 13) {
		cout << "Criando textura. Aguarde...\n\n";
		resetTexture();
		makeTexture();
		glutPostRedisplay();
	}
}

Pairf convertCoord(int x, int y) {
	Pairf p;
	p.x = -1 + 2*((float) x / WINDOW_W);
	p.y = 1 - 2*((float) y / WINDOW_H);
	return p;
}

int getWindowWidth() {
	return WINDOW_W;
}

int getWindowHeight() {
	return WINDOW_H;
}