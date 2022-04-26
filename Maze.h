#pragma once
#ifndef _MAZE_
#define _MAZE_

#include //

class Maze {
public:
	maze() {

	}

private:
	bool* maze_;
};

class player {
public:
	player(int pos = 0, Maze maze) : pos_(pos), maze_(maze) {}

private:
	Maze maze_;
	int pos_;
};



#endif // ifndef _MAZE_