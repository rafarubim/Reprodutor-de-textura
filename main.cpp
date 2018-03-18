#include "glut.h"
#include "Interface.hpp"
#include "Pattern.hpp"
#include "Configurations.h"

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(getWindowWidth(), getWindowHeight());
	glutInitWindowPosition(750, 200);

	glutCreateWindow(NOME_TEXTURA);

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);

	gluOrtho2D(-1, 1, -1, 1);

	initialize();
	
	makeTexture();

	glutMainLoop();

	return 0;
}