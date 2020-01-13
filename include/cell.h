#pragma once

#include <GLFW/glfw3.h>
#include <cmath>
#include "def.h"
#include "gui.h"

/*
class cell is defined in this file

cell corresponds to a square in the minesweeper game. Each cell is defined by
two values: Visibility and content.

	visibility --> shows the visibility of the cell. The visibility is the information the player
				has access to. For instance, a cell can be UNEXOLORED,
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

private:
	Visibility _visibility;
	int _content;
	
};


//cell is initialized with a given content in [-1,8] and Visibility
Cell::Cell() : _visibility(UNEXOLORED), _content(0) {}
Cell::Cell(int content) : _visibility(UNEXOLORED), _content(content) {}

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
//visibility other than UNEXOLORED, then nothing happens and the current
//vivibility is returned
Visibility Cell::explore() {
	if(_visibility != UNEXOLORED)
		return _visibility;

	if(_content == BOMB) {
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
	if(_visibility != UNEXOLORED)
		return false;

	_visibility = FLAGGED;
	return true;
}

//removes the flag
bool Cell::unflag() {
	if(_visibility != FLAGGED)
		return false;

	_visibility = UNEXOLORED;
	return true;
}
