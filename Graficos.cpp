#include "Graficos.hpp"

#include "glut.h"

void resetGraphics() {
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	gluOrtho2D(-1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
}

void scaleGraphics(float factorX, float factorY) {
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glScalef(factorX, factorY, 1);
}

void translateGraphics(float distX, float distY) {
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glTranslatef(distX, distY, 1);
}