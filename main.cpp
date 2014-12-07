#include <Windows.h> // Note, this code only compiles on Windows platforms.
#include <GL\glut.h>
#include "gameLogic.h"
#include <iostream>

const int BLACK = 10; // representation for black player
const int WHITE = 1; // representatino for white player

const GLdouble X = 480, Y = 481; // Size of game window
const GLfloat BACKGROUND[3] = { 0.0f, 0.5f, 0.0f }; // Color for background of board
const GLfloat OUTLINE[3] = { 0.0f, 1.0f, 0.0f }; // Color of grid outline
Game board; // Array state representation of game board
int turn; // Represent whose turn it is

void gameStart() { //initializes game board with center four pieces
	board = Game();
	board.board[3][3] = board.board[4][4] = BLACK; // NE and SW are black
	board.board[3][4] = board.board[4][3] = WHITE; // NW and SE are white
	board.getMoves();
}

void init() {
	glViewport(0, 0, X, Y);
	glClearColor(0.2f, 0.2f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, X, 0, Y);
	glMatrixMode(GL_MODELVIEW);
}

void drawPieces() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (board.board[i][j] == BLACK) { // Draw a black piece
				glBegin(GL_POLYGON);
				glColor3f(0, 0, 0);
				glVertex2f(60 * i + 20, 60 * j + 50); // v0
				glVertex2f(60 * i + 40, 60 * j + 50); // v1
				glVertex2f(60 * i + 50, 60 * j + 40); // v2
				glVertex2f(60 * i + 50, 60 * j + 20); // v3
				glVertex2f(60 * i + 40, 60 * j + 10); // v4
				glVertex2f(60 * i + 20, 60 * j + 10); // v5
				glVertex2f(60 * i + 10, 60 * j + 20); // v6
				glVertex2f(60 * i + 10, 60 * j + 40); // v7
				glEnd();
			}
			else if (board.board[i][j] == WHITE) { // Draw a white piece
				glBegin(GL_POLYGON);
				glColor3f(1, 1, 1);
				glVertex2f(60 * i + 20, 60 * j + 50); // v0
				glVertex2f(60 * i + 40, 60 * j + 50); // v1
				glVertex2f(60 * i + 50, 60 * j + 40); // v2
				glVertex2f(60 * i + 50, 60 * j + 20); // v3
				glVertex2f(60 * i + 40, 60 * j + 10); // v4
				glVertex2f(60 * i + 20, 60 * j + 10); // v5
				glVertex2f(60 * i + 10, 60 * j + 20); // v6
				glVertex2f(60 * i + 10, 60 * j + 40); // v7
				glEnd();
			} // Do nothing if board[i][j] == 0
		}
	}
}

void drawBoard() {
	glBegin(GL_QUADS);
	glColor3fv(BACKGROUND);
	glVertex2f(0, 0); glVertex2f(X, 0); glVertex2f(X, X); glVertex2f(0, X); // Makes background of board distinguishable; important when adding score/turn UI
	glEnd();

	glBegin(GL_LINES);
	glColor3fv(OUTLINE);
	for (int i = 0; i <= 8; ++i) {
		glVertex2f(0, i * 60); glVertex2f(X, i * 60); // Draws horizontal grid lines
	}
	glVertex2f(1, 0); glVertex2f(1, X);
	for (int j = 1; j <= 7; ++j) {
		glVertex2f(j * 60, 0); glVertex2f(j * 60, X); // Draws vertical grid lines
	}
	glVertex2f(X-0.5, 0); glVertex2f(X-0.5, X);
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	drawBoard();
	drawPieces();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, X, Y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, X, 0.0, Y);
	glMatrixMode(GL_MODELVIEW);
	glutReshapeWindow(X, Y); // Maintains constant window size
}

void idle() {
	return;
}

void mouse(int button, int state, int x, int y) {
	int indX = x / 60; // Convert pixels to board index;
	int indY = (Y - y) / 60; // Measures from top-left, so must convert since array draws from bottom right
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // Detects array position based on where mouse is left-clicked
		std::cout << "Clicked on: (" << indX << ", " << indY << ")\n";
		if (board.board[indX][indY] == 0) {
			board.makeMove(indX, indY);
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { // Resets board if right-clicked
		gameStart();
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	gameStart();
	glutInit(&argc, argv);                 // Initialize GLUT
	glutCreateWindow("Reversi"); // Create a window with the given title
	glutInitWindowSize(480, 640);   // Set the window's initial width & height
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}