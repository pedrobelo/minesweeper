#pragma once

#include <GLFW/glfw3.h>
#include <cmath>
#include "def.h"
#include "gui.h"

class Board
{
public:
	Board(int, int);
	void run();

	int _height, _width;
	std::vector<std::vector<Cell>> _cells;

private:
	Gui _gui;

	bool endOfGame();
};


/*************************************************************************
**************************************************************************
**************************************************************************
Board
**************************************************************************
**************************************************************************
*************************************************************************/

Board::Board(int height, int width){
	_height = height; _width = width;
	_cells.resize(width, std::vector<Cell>(height));

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
					if(_cells[y][x].getVisibility() == UNEXOLORED)
						_cells[y][x].flag();
					else if(_cells[y][x].getVisibility() == FLAGGED)
						_cells[y][x].unflag();
				}
				if(button == LEFT) {
					if(_cells[y][x].getVisibility() == UNEXOLORED) {
						if(_cells[y][x].explore() == BOMB)
							return;
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

