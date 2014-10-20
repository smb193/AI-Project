#include <Windows.h> // Note, this code only compiles on Windows platforms.
#include <GL\glut.h>

const int BLACK = 10;
const int WHITE = 1;

const GLdouble X = 480, Y = 481; //size of window
const GLfloat BACKGROUND[3] = { 0.0f, 0.5f, 0.0f }; //color for background of board
const GLfloat OUTLINE[3] = { 0.0f, 1.0f, 0.0f }; //color of grid outline
int board [8][8];

void gameStart() {
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			board[i][j] = 0;
	board[3][3] = board[4][4] = BLACK;
	board[3][4] = board[4][3] = WHITE;
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
			if (board[i][j] == BLACK) {
				glBegin(GL_POLYGON);
				glColor3f(0, 0, 0);
				glVertex2f(60 * j + 20, 60 * i + 50); // v0
				glVertex2f(60 * j + 40, 60 * i + 50); // v1
				glVertex2f(60 * j + 50, 60 * i + 40); // v2
				glVertex2f(60 * j + 50, 60 * i + 20); // v3
				glVertex2f(60 * j + 40, 60 * i + 10); // v4
				glVertex2f(60 * j + 20, 60 * i + 10); // v5
				glVertex2f(60 * j + 10, 60 * i + 20); // v6
				glVertex2f(60 * j + 10, 60 * i + 40); // v7
				glEnd();
			}
			else if (board[i][j] == WHITE) {
				glBegin(GL_POLYGON);
				glColor3f(1, 1, 1);
				glVertex2f(60 * j + 20, 60 * i + 50); // v0
				glVertex2f(60 * j + 40, 60 * i + 50); // v1
				glVertex2f(60 * j + 50, 60 * i + 40); // v2
				glVertex2f(60 * j + 50, 60 * i + 20); // v3
				glVertex2f(60 * j + 40, 60 * i + 10); // v4
				glVertex2f(60 * j + 20, 60 * i + 10); // v5
				glVertex2f(60 * j + 10, 60 * i + 20); // v6
				glVertex2f(60 * j + 10, 60 * i + 40); // v7
				glEnd();
			}
		}
	}
}

void drawBoard() {
	glBegin(GL_QUADS);
	glColor3fv(BACKGROUND);
	glVertex2f(0, 0); glVertex2f(X, 0); glVertex2f(X, X); glVertex2f(0, X); // Makes background distinguishable
	glEnd();

	glBegin(GL_LINES);
	glColor3fv(OUTLINE);
	for (int i = 0; i <= 8; ++i) {
		glVertex2f(0, i * 60); glVertex2f(X, i * 60); // Draws horizontal lines
	}
	glVertex2f(1, 0); glVertex2f(1, X);
	for (int j = 1; j <= 7; ++j) {
		glVertex2f(j * 60, 0); glVertex2f(j * 60, X); // Draws vertical lines
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
	glutReshapeWindow(X, Y);
}

void idle() {
	return;
}

int main(int argc, char** argv) {
	gameStart();
	glutInit(&argc, argv);                 // Initialize GLUT
	glutCreateWindow("Reversi"); // Create a window with the given title
	glutInitWindowSize(480, 640);   // Set the window's initial width & height
	glutInitWindowPosition(100, 100);
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();           // Enter the infinitely event-processing loop
	return 0;
}