#include "minesweeper.h"
#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>

#include "board.h"


int main()
{
	
	Board _board(10, 10);
	_board.run();
	return 0;
}