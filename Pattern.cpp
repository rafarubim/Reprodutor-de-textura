#include "Pattern.hpp"

#include <iostream>
#include <thread>
using namespace std;
#include "glut.h"
#include <ctime>
#include "Configurations.h"
#include "Lista.cpp"
#include "Pairs.h"

typedef enum {TEMPLATE, IMAGEM};

bool** _padrao;
int _patternWidth;
int _patternHeight;

int _dev_width;
int _dev_height;

int _patternSearchWidth;
int _patternSearchHeight;

char _texture[TEXTURE_WIDTH][TEXTURE_HEIGHT];


static void calcDistPartial(int **distanceMatrix, int matrixWidth, Pair selectedDEv, int iMin, int jMin, int iMax, int jMax, int* minDistanceRet);
static void partMatrix(int height, int width, int piecesNum, int pieceIndex, int* iMinRet, int* jMinRet, int* iMaxRet, int* jMaxRet);
static int obterMinimo(int v[], int n);

void resetTexture() {

	for (int i = 0; i < TEXTURE_HEIGHT; i++)
		for (int j = 0; j < TEXTURE_WIDTH; j++)
			_texture[i][j] = -1;
}

void initialize() {

	srand(time(NULL));

	FILE* fTexture;
	fTexture = fopen(NOME_TEXTURA, "rt");
	if (fTexture == NULL) {printf("Erro na abertura do arquivo \"Texture.bin\".\n\n");	exit(1);}

	if (fscanf(fTexture, " %d %d", &_patternWidth, &_patternHeight) != 2) {printf("O arquivo \"Textura.bin\" esta em formato desconhecido (1).\n\n");	exit(1);}

	if (TEMPLATE_WIDTH < 1 || TEMPLATE_HEIGHT < 1 || TEMPLATE_WIDTH > TEXTURE_WIDTH || TEMPLATE_HEIGHT > TEXTURE_HEIGHT || TEMPLATE_WIDTH > _patternWidth || TEMPLATE_HEIGHT > _patternHeight)
		{cout << "O tamanho de template escolhido e inadequado para o padrao usado ou para a textura desejada!\n\n"; exit(1);}

	_padrao = new bool*[_patternHeight];
	if (_padrao == NULL) {cout << "Erro(1) de memoria.\n\n";	exit(1);}

	for (int i = 0; i < _patternHeight; i++) {
		_padrao[i] = new bool[_patternWidth];
		if (_padrao[i] == NULL) {cout << "Erro (2."<<i<<") de memoria.\n\n";	exit(1);}
	}

	for (int i = 0; i < _patternHeight; i++)
		for(int j = 0; j < _patternWidth; j++)
			if (fscanf(fTexture, " %d", &_padrao[i][j]) != 1) {cout << "O arquivo \"Textura.bin\" esta em formato desconhecido (2).\n\n";	exit(1);}

	fclose(fTexture);

	resetTexture();

	cout << "Criando textura. Aguarde...\n\n";
}

int getPatternWidth() {
	return _patternWidth;
}

int getPatternHeight() {
	return _patternHeight;
}

void drawPatternGrid() {
	glBegin(GL_LINES);
		for (int i = 0; i <= (_patternWidth); i++) {
			glVertex2f(-1 + i*2./_patternWidth, -1);
			glVertex2f(-1 + i*2./_patternWidth, 1);
		}
		for (int i = 0; i <= (_patternHeight); i++) {
			glVertex2f(-1, -1 + i*2./_patternHeight);
			glVertex2f(1, -1 + i*2./_patternHeight);
		}
	glEnd();
}

void drawPattern() {
	glBegin(GL_QUADS);
		for(int i = 0; i < _patternHeight; i++)
			for(int j = 0; j < _patternWidth; j++)
				if (_padrao[i][j]) {
					glVertex2f(-1 + j*2./_patternWidth, 1 - i*2./_patternHeight);
					glVertex2f(-1 + (j+1)*2./_patternWidth, 1 - i*2./_patternHeight);
					glVertex2f(-1 + (j+1)*2./_patternWidth, 1 - (i+1)*2./_patternHeight);
					glVertex2f(-1 + j*2./_patternWidth, 1 - (i+1)*2./_patternHeight);
				}
				
	glEnd();
}

void drawTextureGrid() {
	glBegin(GL_LINES);
		for (int i = 0; i <= (TEXTURE_WIDTH); i++) {
			glVertex2f(-1 + i*2./TEXTURE_WIDTH, -1);
			glVertex2f(-1 + i*2./TEXTURE_WIDTH, 1);
		}
		for (int i = 0; i <= (TEXTURE_HEIGHT); i++) {
			glVertex2f(-1, -1 + i*2./TEXTURE_HEIGHT);
			glVertex2f(1, -1 + i*2./TEXTURE_HEIGHT);
		}
	glEnd();
}

void drawTexture() {
	glBegin(GL_QUADS);
		for(int i = 0; i < TEXTURE_HEIGHT; i++)
			for(int j = 0; j < TEXTURE_WIDTH; j++)
				if (_texture[i][j] == 1) {
					glVertex2f(-1 + j*2./TEXTURE_WIDTH, 1 - i*2./TEXTURE_HEIGHT);
					glVertex2f(-1 + (j+1)*2./TEXTURE_WIDTH, 1 - i*2./TEXTURE_HEIGHT);
					glVertex2f(-1 + (j+1)*2./TEXTURE_WIDTH, 1 - (i+1)*2./TEXTURE_HEIGHT);
					glVertex2f(-1 + j*2./TEXTURE_WIDTH, 1 - (i+1)*2./TEXTURE_HEIGHT);
				}
				
	glEnd();
}

void drawBorder() {
	glBegin(GL_LINE_LOOP);
		glVertex2f(-1, -1);
		glVertex2f(-1, 1);
		glVertex2f(1, 1);
		glVertex2f(1, -1);
	glEnd();
}

int patternDistance(int dEvY, int dEvX, int patternY, int patternX) {

	int soma = 0;

	for (int i = 0; i < TEMPLATE_HEIGHT; i ++)
		for (int j = 0; j < TEMPLATE_WIDTH; j++)
			if (_texture[dEvY+i][dEvX+j] != -1 && _texture[dEvY+i][dEvX+j] != _padrao[patternY+i][patternX+j])
				soma++;
	
	return soma;
}

void makeDataEvent(int dEvY, int dEvX, int patternY, int patternX) {

	for (int i = 0; i < TEMPLATE_HEIGHT; i ++)
		for (int j = 0; j < TEMPLATE_WIDTH; j++)
			_texture[dEvY+i][dEvX+j] = _padrao[patternY+i][patternX+j];
}

void makeTexture() {

	_dev_width = (TEXTURE_WIDTH - TEMPLATE_WIDTH + 1);
	_dev_height = (TEXTURE_HEIGHT - TEMPLATE_HEIGHT + 1);

	_patternSearchWidth = _patternWidth - TEMPLATE_WIDTH + 1;
	_patternSearchHeight = _patternHeight - TEMPLATE_HEIGHT + 1;

	if (NUM_THREADS < 1) { cout << "Erro: numero de threads nao pode ser inferior a 1!\n\n";	exit(1); }
	thread* myThreads = new thread[NUM_THREADS-1];

	int lstSize = _dev_width * _dev_height;
	//Lista<Pair> remainingDEvs

	Pair* vecRemainingDEvs = new Pair[lstSize];
	if (vecRemainingDEvs == NULL) {cout << "Erro em criacao de textura: memoria insuficiente\n\n";	exit(1);};

	int hundredPercent = lstSize;
	int percent = 1;
	//cout << "lstSize: " << lstSize << endl;

	for (int i = 0; i < _dev_height; i++)
		for(int j = 0; j < _dev_width; j++) {
			Pair p = {j, i};
			//remainingDEvs.insere(0, p);
			vecRemainingDEvs[i*_dev_width +j] = p;
		}

	while (lstSize > 0) {

		int selectedIndex = rand()%lstSize;

		//cout << "SelectedIndex: " << selectedIndex << ", lstSize: " << lstSize << endl;

		//Pair selectedDev = remainingDEvs[selectedIndex];
		Pair selectedDev = vecRemainingDEvs[selectedIndex];
		//remainingDEvs.retira(selectedIndex);
		vecRemainingDEvs[selectedIndex] = vecRemainingDEvs[lstSize-1];
		lstSize--;

		int* minDistances = new int[NUM_THREADS];

		int** distances = new int*[_patternSearchHeight];

		for (int i = 0; i < _patternSearchHeight; i++)
			distances[i] = new int[_patternSearchWidth];

		int iMin, jMin, iMax, jMax;
		for (int i = 1; i < NUM_THREADS; i++) {
			partMatrix(_patternSearchHeight, _patternSearchWidth, NUM_THREADS, i, &iMin, &jMin, &iMax, &jMax);
			myThreads[i - 1] = thread(calcDistPartial, distances, _patternSearchWidth, selectedDev, iMin, jMin, iMax, jMax, &minDistances[i]);
		}
		partMatrix(_patternSearchHeight, _patternSearchWidth, NUM_THREADS, 0, &iMin, &jMin, &iMax, &jMax);
		calcDistPartial(distances, _patternSearchWidth, selectedDev, iMin, jMin, iMax, jMax, &minDistances[0]);

		for (int i = 1; i < NUM_THREADS; i++)
			myThreads[i - 1].join();

		int minDistance = obterMinimo(minDistances, NUM_THREADS);

		delete[] minDistances;

		//Lista<Pair> possiblePatterns;
		Pair* vecPossiblePatterns = new Pair[_patternSearchWidth*_patternSearchHeight];

		int possiblePatternsNum = 0;

		for (int i = 0; i < _patternSearchHeight; i++)
			for (int j = 0; j < _patternSearchWidth; j++)
				if (distances[i][j] == minDistance) {
					Pair patternU;
					patternU.x = j;
					patternU.y = i;
					//possiblePatterns.insere(0, patternU);
					vecPossiblePatterns[possiblePatternsNum] = patternU;
					possiblePatternsNum++;
				}

		int chosenIndex = rand()%possiblePatternsNum;

		//Pair chosenPattern = possiblePatterns[chosenIndex];
		Pair chosenPattern = vecPossiblePatterns[chosenIndex];

		delete[] vecPossiblePatterns;

		for (int i = 0; i < _patternSearchHeight; i++)
			delete[] distances[i];

		delete[] distances;

		makeDataEvent(selectedDev.y, selectedDev.x, chosenPattern.y, chosenPattern.x);

		if (WRITE_PERCENTAGE) 
			for (int i = 1; i <= 10; i++) 
				if (10-lstSize*10/hundredPercent == i && percent == i) {
					cout << 10*i << "% completo\n";
					percent ++;
					if (i == 10)
						cout << endl;
				}
		
	}

	delete [] vecRemainingDEvs;
}

void calcDistPartial(int **distanceMatrix, int matrixWidth, Pair selectedDEv, int iMin, int jMin, int iMax, int jMax, int* minDistanceRet) {

	int minDistanceTemp = TEMPLATE_WIDTH*TEMPLATE_HEIGHT;

	for (int i = iMin; i <= iMax; i++) {
		int j;
		if (i == iMin)
			j = jMin;
		else
			j = 0;
		for (; (i != iMax && j < matrixWidth) || (i == iMax && j < jMax); j++) {

			int tempDistance;

			tempDistance = patternDistance(selectedDEv.y, selectedDEv.x, i, j);

			if (tempDistance < minDistanceTemp)
				minDistanceTemp = tempDistance;

			distanceMatrix[i][j] = tempDistance;
		}
	}

	*minDistanceRet = minDistanceTemp;
}

void partMatrix(int height, int width, int piecesNum, int pieceIndex, int* iMinRet, int* jMinRet, int* iMaxRet, int* jMaxRet) {
	int totalElems = height * width;
	int division = totalElems / piecesNum;
	int vecMin = pieceIndex * division;
	int vecMax;
	if (pieceIndex == piecesNum - 1)
		vecMax = totalElems;
	else
		vecMax = (pieceIndex + 1) * division;
	*iMinRet = vecMin / width;
	*jMinRet = vecMin % width;
	*iMaxRet = vecMax / width;
	*jMaxRet = vecMax % width;
}

int obterMinimo(int v[], int n) {
	if (n == 0) { cout << "Erro ao procurar minimo de vetor vazio\n\n";	exit(1); }
	int min = v[0];
	for (int i = 1; i < n; i++)
		if (min > v[i])
			min = v[i];
	return min;
}