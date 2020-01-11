#include "minesweeper.h"
#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>

#include"board.h"

/*
int openWindow() {
	while (!glfwWindowShouldClose(_window)) {
		float x=100; float y=100; int width=2; int height=2;
		glfwGetFramebufferSize(_window, &width, &height);

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex2f(-0.6f, -0.4f);
		glVertex2f(0.6f, -0.4f);
		glVertex2f(0.f, 0.6f);
		glEnd();
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}


void Board::fillBoard(){
	for (int i = 0; i < _height; i++)
		for (int j = 0; j < _width; j++)
			_board[i][j] = 0;

	for (int i = 0; i < _bomb_cnt; i++) {
		int y = rand()%_height;
		int x = rand()%_width;

		if(_board[y][x] == -1) {
			i--;
			continue;
		}

		_board[y][x] = -1;
		for (int k = -1; k < 2; k++)
			for (int p = -1; p < 2; p++)
				if(p!=0 && k!=0 && y+k>=0 && x+p>=0 && y+k<_height && x+p<_width)
					if(_board[y+k][x+p] != -1)
						_board[y+k][x+p]++;
	}
}
*/

int main()
{
	
	Board _b(10,10);
	while(1)
		_b.drawBoard();
	return 0;
}