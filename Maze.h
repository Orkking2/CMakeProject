#pragma once
#ifndef _MAZE_
#define _MAZE_

#include //

struct tile {
	tile(bool on) : on(on) {}
	bool on;
};

class maze {
public:
	maze() {

	}
private:
	tile* tile_list_;
};

class player {
public:
	player(int pos = 0) : pos(pos) {}

private:
	int pos;
};



#endif // ifndef _MAZE_