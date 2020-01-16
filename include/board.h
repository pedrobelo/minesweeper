#pragma once

#include <cstdlib>
#include <GLFW/glfw3.h>
#include <cmath>
#include "def.h"
#include "gui.h"

class Board
{
public:
	Board(int, int, int);
	void run();

	int _height, _width, _bomb_cnt;
	std::vector<std::vector<Cell>> _cells;

private:
	Gui _gui;
	void openFreeSpace(int row, int col);
	bool endOfGame();

};


/*************************************************************************
**************************************************************************
**************************************************************************
Board
**************************************************************************
**************************************************************************
*************************************************************************/

Board::Board(int height, int width, int bomb_cnt) : _height(height),
													_width(width), 
													_bomb_cnt(bomb_cnt) {
	_cells.resize(width, std::vector<Cell>(height));
	Cell::initBoard(_cells, _bomb_cnt);
}

void Board::openFreeSpace(int row, int col) {
	if(_cells[row][col].getContent() == 0)
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				if((i!=0||j!=0) && row+i >= 0 && row+i < _height && col+j >= 0 && col+j < _width)
					if(_cells[row+i][col+j].getVisibility() == UNEXPLORED) {
						_cells[row+i][col+j].explore();
						openFreeSpace(row+i, col+j);
					}
}

void Board::run() {
	char ans;
	std::cout << "Do you want to play the game yourself? (y/n)" << std::endl;
	std::cin >> ans;

	if(ans == 'y') {
		int x, y;
		MouseButton button;
		_gui = Gui(10, 10, true);
		glfwSetWindowUserPointer(_gui._window, &_gui);

		while(!endOfGame() && !glfwWindowShouldClose(_gui._window)) {
			_gui.drawBoard(_cells);
			if(_gui.getLastMousePress(x, y, button)) {
				if(button == RIGHT) {
					if(_cells[y][x].getVisibility() == UNEXPLORED)
						_cells[y][x].flag();
					else if(_cells[y][x].getVisibility() == FLAGGED)
						_cells[y][x].unflag();
				}
				if(button == LEFT) {
					if(_cells[y][x].getVisibility() == UNEXPLORED) {
						if(_cells[y][x].explore() == BOMB);
							//return;
						openFreeSpace(y, x);
					}
				}
			}

		}
	}
	else {
		_gui = Gui(10, 10, false);
		//play here
	}
}

bool Board::endOfGame() {
	return false;
}

