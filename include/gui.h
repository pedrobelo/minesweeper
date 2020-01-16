#pragma once

#include <GLFW/glfw3.h>
#include <cmath>
#include "def.h"
#include "cell.h"

class Gui
{
public:
	Gui();
	Gui(int, int, bool);
	~Gui();
	void drawBoard(const std::vector<std::vector<Cell>>& c);

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	bool getLastMousePress(int& x, int& y, MouseButton &button);

public:
	GLFWwindow* _window;

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
	int _square_pixels;
	int _height, _width;
	int _last_pressed_x, _last_pressed_y;
	MouseButton _mouse_button;
	bool _pressed;
};


static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

static void mouseButtonCallback_static(GLFWwindow* window, int button, int action, int mods) {
	Gui* gui = (Gui*)glfwGetWindowUserPointer(window);
	gui->mouseButtonCallback(window, button, action, mods);
}

void Gui::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	double xpos, ypos;
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		_last_pressed_x = (int)xpos/SQUARE_SIZE;
		_last_pressed_y = _height-(int)ypos/SQUARE_SIZE-1;
		_mouse_button = LEFT;
		_pressed = true;
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		_last_pressed_x = (int)xpos/SQUARE_SIZE;
		_last_pressed_y = _height-(int)ypos/SQUARE_SIZE-1;
		_mouse_button = RIGHT;
		_pressed = true;
	}
}

bool Gui::getLastMousePress(int& x, int& y, MouseButton &button) {
	if(_pressed) {
		x = _last_pressed_x; y = _last_pressed_y; button = _mouse_button;
		_pressed = false;
		return true;
	}
	return false;
}

Gui::Gui() {
	_window = nullptr;
}

Gui::Gui(int height, int width, bool interaction) : _height(height), _width(width), _pressed(false) {
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

	if(interaction)
		glfwSetMouseButtonCallback(_window, mouseButtonCallback_static);
}

float Gui::getXAxis(float col, float position) {
	return 2.0*(col+position)/_width-1;
}

float Gui::getYAxis(float row, float position) {
	return 2.0*(row+position)/_height-1;
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

	for(uint i=0; i < c.size(); i++) {
		for(uint j=0; j < c[i].size(); j++) {
			if(c[i][j].getVisibility() == UNEXPLORED) {
				drawUnpressedSquare(i, j);
			}
			else if(c[i][j].getVisibility() == FLAGGED) {
				drawUnpressedSquare(i, j);
				drawFlag(i,j);
			}
		}
	}

	for(uint i=0; i < c.size(); i++) {
		for(uint j=0; j < c[i].size(); j++) {
			if(c[i][j].getVisibility() == BOMB) {
				drawPressedSquare(i, j);
				drawBomb(i,j);
			}
			else if(c[i][j].getVisibility() == FREE) {
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