#pragma once
#ifndef _MAZE_
#define _MAZE_

#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "LinkedList.h"

#ifndef elif
#define elif(expr) else if (expr)
#endif

#ifndef _GEOMETRY_
struct Point {
	Point(int x = 0, int y = 0) : x(x), y(y) {}
	int x, y;
};
#endif // ifndef _GEOMETRY_

struct Tile {
	enum Type {
		Wall,
		Space,
		Border,
		Beginning,
		End,
		NewLine,
		Unknown
	};
	Tile(Type type, char symbol, bool explored = false) : type(type), symbol(symbol), explored(explored) {}

	Type type;
	char symbol;
	bool explored;
};

class Maze {
private:
	_Array_with_count<Tile> _tile_arr;
public:
	Maze(int dim_x = 0, int dim_y = 0, char wall_char = '+', char space_char = ' ', char new_line_char = '\n', std::string file_name = "Maze.txt") {

		int dim[3] = { dim_x * dim_y, dim_x, dim_y };

		std::string courier_str;
		std::ifstream maze_file(file_name);

		if (maze_file.is_open()) maze_file >> courier_str;
		else std::cout << "Failed to open file: " << file_name << "\n";

		_LINKED_ARRAY<Tile> tiles;

		using TT = Tile::Type;

		for (char c : courier_str) {
			Tile::Type t;
			if   (c == new_line_char) t = TT::NewLine;
			elif (c == space_char)    t = TT::Space;
			elif (c == wall_char)     t = TT::Wall;
			else                      t = TT::Unknown;

			tiles.push_back(Tile(t, c));
		}
		_tile_arr = tiles.get_array();

		// Beginning/end logic

		for (int i = 0; i < _tile_arr.count; i++) {
			Tile curr_tile = _tile_arr[i];
			if (curr_tile.type == TT::NewLine) {
				_tile_arr[i - 1].type = TT::Border;
				_tile_arr[i + 1].type = TT::Border;
			}
			
		}
	}
};

class Player {
private:
	Maze _maze;
	Point _pos;
public:
	Player(Point pos, Maze maze) : _pos(pos), _maze(maze) {}

};

class Display {
private:
	_LINKED_ARRAY<Player> _players;
public:
	Display(){}
};


#endif // ifndef _MAZE_