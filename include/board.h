#pragma once

#include <GLFW/glfw3.h>
#include <cmath>

#define SQUARE_SIZE 60
#define SHADE 0.1
#define FLAG_X 0.45
#define FLAG_Y 0.3
#define FLAG 0.1

enum Visibility {FREE=0, BOMB=-1, UNEXOLORED=-2, FLAGGED=-3};
enum MouseButton {RIGHT=0, LEFT=1};
class Board;

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


class Gui
{
public:
	Gui();
	Gui(int, int);
	~Gui();
	void drawBoard(const std::vector<std::vector<Cell>>& c);

private:
	void drawUnpressedSquare(int, int);
	void drawPressedSquare(int, int);
	void drawQestionMarkSquare();
	void drawFlag(int, int);
	void drawBomb(int, int);
	void drawNumber(int, int, int);

	float getXAxis(float col, float position);
	float getYAxis(float row, float position);
	int getRow(int ypos);
	int getCol(int xpos);


private:
	GLFWwindow* _window;
	int _square_pixels;
	int _height, _width;
	int last_pressed_x, last_pressed_y;
	MouseButton mouse_button;
};


class Board
{
public:
	Board(int, int);
	Board(int, int, bool);
	void drawBoard();

	int _height, _width;
	std::vector<std::vector<Cell>> _cells;

private:
	Gui _gui;
};



/*************************************************************************
**************************************************************************
**************************************************************************
Cell
**************************************************************************
**************************************************************************
*************************************************************************/

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


/*************************************************************************
**************************************************************************
**************************************************************************
GUI
**************************************************************************
**************************************************************************
*************************************************************************/

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	double xpos, ypos;
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << xpos << " " << ypos << std::endl;
	}      
}

Gui::Gui() {
	_window = nullptr;
}

Gui::Gui(int height, int width) : _height(height), _width(width) {
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	_window = glfwCreateWindow(width*SQUARE_SIZE, height*SQUARE_SIZE, "Minesweeper", NULL, NULL);

	if (!_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(_window);
	glfwSetMouseButtonCallback(_window, mouseButtonCallback);
}

float Gui::getXAxis(float col, float position) {
	return 2.0*(col+position)/_width-1;
}

float Gui::getYAxis(float row, float position) {
	return 2.0*(row+position)/_height-1;
}

int Gui::getRow(int ypos) {

}

int Gui::getCol(int xpos) {
	
}

void Gui::drawUnpressedSquare(int row, int col) {

	float out_y_low = 2.0*(row+0)/_height-1;
	float out_y_high = 2.0*(row+1)/_height-1;
	float out_x_low = 2.0*(col+0)/_width-1;
	float out_x_high = 2.0*(col+1)/_width-1;

	float in_y_low = 2.0*(row+0+SHADE)/_height-1;
	float in_y_high = 2.0*(row+1-SHADE)/_height-1;
	float in_x_low = 2.0*(col+0+SHADE)/_width-1;
	float in_x_high = 2.0*(col+1-SHADE)/_width-1;

	glBegin(GL_QUADS);
		glColor3f(0.65f, 0.65f, 0.65f);
		glVertex2f(in_x_low, in_y_high);
		glVertex2f(in_x_low, in_y_low);
		glVertex2f(in_x_high, in_y_low);
		glVertex2f(in_x_high, in_y_high);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.55f, 0.55f, 0.55f);
		glVertex2f(in_x_high, in_y_high);
		glVertex2f(in_x_high, in_y_low);
		glVertex2f(out_x_high, out_y_low);
		glVertex2f(out_x_high, out_y_high);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.55f, 0.55f, 0.55f);
		glVertex2f(in_x_high, in_y_low);
		glVertex2f(in_x_low, in_y_low);
		glVertex2f(out_x_low, out_y_low);
		glVertex2f(out_x_high, out_y_low);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.75f, 0.75f, 0.75f);
		glVertex2f(in_x_low, in_y_low);
		glVertex2f(in_x_low, in_y_high);
		glVertex2f(out_x_low, out_y_high);
		glVertex2f(out_x_low, out_y_low);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.75f, 0.75f, 0.75f);
		glVertex2f(in_x_low, in_y_high);
		glVertex2f(in_x_high, in_y_high);
		glVertex2f(out_x_high, out_y_high);
		glVertex2f(out_x_low, out_y_high);
	glEnd();
}

void Gui::drawPressedSquare(int row, int col) {
	float x_low = getXAxis(col, 0);
	float x_high = getXAxis(col, 1);
	float y_low = getYAxis(row, 0);
	float y_high = getYAxis(row, 1);

	glBegin(GL_QUADS);
		glColor3f(0.65f, 0.65f, 0.65f);
		glVertex2f(x_low, y_high);
		glVertex2f(x_low, y_low);
		glVertex2f(x_high, y_low);
		glVertex2f(x_high, y_high);
	glEnd();


	glBegin(GL_LINES);
		glColor3f(0.6f, 0.6f, 0.6f);
		glVertex2f(x_low, y_low);
		glVertex2f(x_low, y_high);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.6f, 0.6f, 0.6f);
		glVertex2f(x_low, y_high);
		glVertex2f(x_high, y_high);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.6f, 0.6f, 0.6f);
		glVertex2f(x_high, y_high);
		glVertex2f(x_high, y_low);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(0.6f, 0.6f, 0.6f);
		glVertex2f(x_high, y_low);
		glVertex2f(x_low, y_low);
	glEnd();
}

void Gui::drawFlag(int row, int col) {
	getYAxis(row, FLAG_Y);
	float y_low = getYAxis(row, FLAG_Y);
	float y_high = getYAxis(row, 1-FLAG_Y);
	float x_low = getXAxis(col, 0.5);
	float x_high = getXAxis(col, 0.5+FLAG);

	glBegin(GL_QUADS);
		glColor3f(1, 0, 0);
		glVertex2f(x_low, y_high);
		glVertex2f(x_low, y_low);
		glVertex2f(x_high, y_low);
		glVertex2f(x_high, y_high);
	glEnd();

	float p1_x, p1_y, p2_x, p2_y, p3_x, p3_y;
	p1_x = x_low; p1_y = y_high;
	p3_x = x_low; p3_y = getYAxis(row, 1-FLAG_Y - (1-2*FLAG_Y)/2);
	p2_x = getXAxis(col, 0.5-3*FLAG); p2_y = (p1_y+p3_y)/2;

	glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex2f(p1_x, p1_y);
		glVertex2f(p2_x, p2_y);
		glVertex2f(p3_x, p3_y);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0, 0, 0);

		glVertex2f(getXAxis(col, 0.5+2*FLAG), y_low);
		glVertex2f(getXAxis(col, 0.5-2*FLAG), y_low);
		glVertex2f(getXAxis(col, 0.5-2*FLAG), y_low-0.01);
		glVertex2f(getXAxis(col, 0.5+2*FLAG), y_low-0.01);
	glEnd();
}

void Gui::drawBomb(int row, int col) {

	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.2f);

		glVertex2f(getXAxis(col, 0.35), getYAxis(row, 0.35));
		glVertex2f(getXAxis(col, 0.35), getYAxis(row, 0.65));
		glVertex2f(getXAxis(col, 0.65), getYAxis(row, 0.65));
		glVertex2f(getXAxis(col, 0.65), getYAxis(row, 0.35));
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.2f);

		glVertex2f(getXAxis(col, 0.45), getYAxis(row, 0.25));
		glVertex2f(getXAxis(col, 0.45), getYAxis(row, 0.75));
		glVertex2f(getXAxis(col, 0.55), getYAxis(row, 0.75));
		glVertex2f(getXAxis(col, 0.55), getYAxis(row, 0.25));
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.2f);

		glVertex2f(getXAxis(col, 0.25), getYAxis(row, 0.45));
		glVertex2f(getXAxis(col, 0.75), getYAxis(row, 0.45));
		glVertex2f(getXAxis(col, 0.75), getYAxis(row, 0.55));
		glVertex2f(getXAxis(col, 0.25), getYAxis(row, 0.55));
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.9f, 0.9f, 0.9f);

		glVertex2f(getXAxis(col, 0.4), getYAxis(row, 0.52));
		glVertex2f(getXAxis(col, 0.4), getYAxis(row, 0.6));
		glVertex2f(getXAxis(col, 0.48), getYAxis(row, 0.6));
		glVertex2f(getXAxis(col, 0.48), getYAxis(row, 0.52));
	glEnd();


}

void Gui::drawNumber(int row, int col, int number) {

	float x1=0.275, x2=0.325, x3=0.375, x4=0.625, x5=0.675, x6=0.725;
	float y1=0.1, y2=0.15, y3=0.2, y4=0.45, y5=0.5, y6=0.55, y7=0.8, y8=0.85, y9=0.9;

	glColor3f(0.1f, 0.1f, 0.1f);
	if(number==1)
		glColor3f(0, 0, 1);
	else if(number==2)
		glColor3f(0, 1, 0);
	else if(number==3)
		glColor3f(1, 0, 0);
	else if(number==4)
		glColor3f(0, 0, 0.5f);
	else if(number==5)
		glColor3f(0.5f, 0, 0);
	else if(number==6)
		glColor3f(0.5f, 0.5f, 0);
	else if(number==7)
		glColor3f(0, 1, 1);
	else if(number==8)
		glColor3f(0, 0.5f, 0.5f);

	//top horizontal
	if(number==2 || number==3 || number==5 || number==6 || number==7 || number==8 || number==9) {
		glBegin(GL_POLYGON);
			glVertex2f(getXAxis(col, x2), getYAxis(row, y8));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y9));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y9));
			glVertex2f(getXAxis(col, x5), getYAxis(row, y8));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y7));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y7));
		glEnd();
	}

	//middle horizontal
	if(number==2 || number==3 || number==4 || number==5 || number==6 || number==8 || number==9) {
		glBegin(GL_POLYGON);
			glVertex2f(getXAxis(col, x2), getYAxis(row, y5));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y6));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y6));
			glVertex2f(getXAxis(col, x5), getYAxis(row, y5));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y4));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y4));
		glEnd();
	}

	//bottom horizontal
	if(number==2 || number==3 || number==5 || number==6 || number==8 || number==9) {
		glBegin(GL_POLYGON);
			glVertex2f(getXAxis(col, x2), getYAxis(row, y2));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y3));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y3));
			glVertex2f(getXAxis(col, x5), getYAxis(row, y2));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y1));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y1));
		glEnd();
	}

	//top vertical left
	if(number==4 || number==5 || number==6 || number==8 || number==9) {
		glBegin(GL_POLYGON);
			glVertex2f(getXAxis(col, x2), getYAxis(row, y8));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y7));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y6));
			glVertex2f(getXAxis(col, x2), getYAxis(row, y5));
			glVertex2f(getXAxis(col, x1), getYAxis(row, y6));
			glVertex2f(getXAxis(col, x1), getYAxis(row, y7));
		glEnd();
	}

	//top vertical right
	if(number==1 || number==2 || number==3 || number==4  || number==7 || number==8 || number==9) {
		glBegin(GL_POLYGON);
			glVertex2f(getXAxis(col, x5), getYAxis(row, y8));
			glVertex2f(getXAxis(col, x6), getYAxis(row, y7));
			glVertex2f(getXAxis(col, x6), getYAxis(row, y6));
			glVertex2f(getXAxis(col, x5), getYAxis(row, y5));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y6));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y7));
		glEnd();
	}

	//bottom vertical left
	if(number==2 || number==6 || number==8) {
		glBegin(GL_POLYGON);
			glVertex2f(getXAxis(col, x2), getYAxis(row, y5));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y4));
			glVertex2f(getXAxis(col, x3), getYAxis(row, y3));
			glVertex2f(getXAxis(col, x2), getYAxis(row, y2));
			glVertex2f(getXAxis(col, x1), getYAxis(row, y3));
			glVertex2f(getXAxis(col, x1), getYAxis(row, y4));
		glEnd();
	}

	//bottom vertical right
	if(number==1 || number==3 || number==4 || number==5 || number==6 || number==7 || number==8 || number==9) {
		glBegin(GL_POLYGON);
			glVertex2f(getXAxis(col, x5), getYAxis(row, y5));
			glVertex2f(getXAxis(col, x6), getYAxis(row, y4));
			glVertex2f(getXAxis(col, x6), getYAxis(row, y3));
			glVertex2f(getXAxis(col, x5), getYAxis(row, y2));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y3));
			glVertex2f(getXAxis(col, x4), getYAxis(row, y4));
		glEnd();
	}

}

void Gui::drawBoard(const std::vector<std::vector<Cell>>& c) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.5, 0.5, 0.5);

	for(int i=0; i < c.size(); i++) {
		for(int j=0; j < c[i].size(); j++) {
			if(c[i][j].getVisibility() == UNEXOLORED) {
				drawUnpressedSquare(i, j);
			}
			else if(c[i][j].getVisibility() == FLAGGED) {
				drawUnpressedSquare(i, j);
				drawFlag(i,j);
			}
		}
	}

	for(int i=0; i < c.size(); i++) {
		for(int j=0; j < c[i].size(); j++) {
			if(c[i][j].getVisibility() == BOMB) {
				drawPressedSquare(i, j);
				drawBomb(i,j);
			}
			else if(c[i][j].getVisibility() == FLAGGED) {
				drawPressedSquare(i, j);
				drawNumber(i,j,c[i][j].getContent());
			}
		}
	}

	glFlush();
	glfwSwapBuffers(_window);
	glfwPollEvents();

}

Gui::~Gui() {
	if(_window) {
		/*glfwSetErrorCallbaglfwDestroyWindow(_window);
		ck(error_callback);
		glfwTerminate();*/
	}
}


/*************************************************************************
**************************************************************************
**************************************************************************
Board
**************************************************************************
**************************************************************************
*************************************************************************/

Board::Board(int height, int width) :_gui(height, width) {
	_height = height; _width = width;
	_cells.resize(width, std::vector<Cell>(height));

	_gui.drawBoard(_cells);
}

Board::Board(int height, int width, bool gui) : _gui(gui ? Gui(height, width):Gui()) {
	_height = height; _width = width;
	_cells.resize(width, std::vector<Cell>(height));
}

void Board::drawBoard() {
	_gui.drawBoard(_cells);
}

