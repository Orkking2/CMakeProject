#pragma once
#ifndef _MAZE_
#define _MAZE_

#include //

class Maze {
public:
	maze(int* dim) : dim_(dim) {

	}

private:
	int* dim_;
	bool* bool_list_;
};

class player {
public:
	player(int pos = 0, Maze maze) : pos_(pos), maze_(maze) {}

private:
	Maze maze_;
	int pos_;
};



#endif // ifndef _MAZE_