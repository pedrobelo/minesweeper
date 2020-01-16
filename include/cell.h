#pragma once

#include <cstdlib>
#include <ctime>

#include <GLFW/glfw3.h>
#include <cmath>
#include "def.h"
#include "gui.h"

/*
class cell is defined in this file

cell corresponds to a square in the minesweeper game. Each cell is defined by
two values: Visibility and content.

	visibility --> shows the visibility of the cell. The visibility is the information the player
				has access to. For instance, a cell can be UNEXPLORED,
				FREE, BOMB (in which case, the game has been lost) and finnaly it can
				be FLAGGED.

	content --> this represents the intrinsic value of each cell. The player does not
				have access to this value until the cell has been explored. The
				content is a number that represents the number of bombs around it. 
				In case the cell is a bomb, then the value is -1.
*/
class Cell
{
public:
	Cell();
	Cell(int);

	Visibility getVisibility() const;
	int getContent() const;

	Visibility explore();
	bool flag();
	bool unflag();

	static void initBoard(std::vector<std::vector<Cell>>& cells, int bomb_cnt);

private:
	Visibility _visibility;
	int _content;	
};


//cell is initialized with a given content in [-1,8] and Visibility
Cell::Cell() : _visibility(UNEXPLORED), _content(0) {}
Cell::Cell(int content) : _visibility(UNEXPLORED), _content(content) {}

//get info of a cell
int Cell::getContent() const {
	//only returns the content if the cell has been covered
	if(_visibility != FREE)
		return _visibility;

	return _content;
}

//returns the visibility of the cell as a enum.
Visibility Cell::getVisibility() const {
	return _visibility;
}

//explore a cell, making it available to the player. If the cell has a
//visibility other than UNEXPLORED, then nothing happens and the current
//vivibility is returned
Visibility Cell::explore() {
	if(_visibility != UNEXPLORED)
		return _visibility;

	if(_content == (int)BOMB) {
		_visibility = BOMB;
		return _visibility;
	}
	else {
		_visibility = FREE;
		return _visibility;
	}
}

//flags a cell to represent a bomb. Only useful for the player
bool Cell::flag() {
	if(_visibility != UNEXPLORED)
		return false;

	_visibility = FLAGGED;
	return true;
}

//removes the flag
bool Cell::unflag() {
	if(_visibility != FLAGGED)
		return false;

	_visibility = UNEXPLORED;
	return true;
}

void Cell::initBoard(std::vector<std::vector<Cell>>& cells, int bomb_cnt) {
	int height = (int)cells.size();
	int width = (int)cells[0].size();

	std::srand(std::time(0));

	for (int cnt = 0; cnt < bomb_cnt; cnt++) {
		int row = std::rand()%height;
		int col = std::rand()%width;

		if(cells[row][col]._content == (int)BOMB) {
			cnt--;
			continue;
		}

		cells[row][col]._content = (int)BOMB;
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				if(row+i >= 0 && row+i < height && col+j >= 0 && col+j < width)
					if(cells[row+i][col+j]._content != (int)BOMB)
						cells[row+i][col+j]._content++;

	}
}

