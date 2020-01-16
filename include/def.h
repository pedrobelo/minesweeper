#pragma once

#define SQUARE_SIZE 60
#define SHADE 0.1
#define FLAG_X 0.45
#define FLAG_Y 0.3
#define FLAG 0.1

enum Visibility {FREE=0, BOMB=-1, UNEXPLORED=-2, FLAGGED=-3};
enum MouseButton {RIGHT=0, LEFT=1};
class Board;
