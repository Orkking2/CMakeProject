#pragma once
#ifndef _NSTD_MAZE_
#define _NSTD_MAZE_

#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "LinkedList.h"
#include "Defines.h"

_NSTD_BEGIN

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
	Tile(Type type, char symbol, bool explored = false) : type_(type), symbol_(symbol), explored_(explored) {}

	Type type_;
	char symbol_;
	bool explored_;
};

class Maze {
private:
	int (vector<Tile>::* len)() const = &vector<Tile>::get_len;
	_NSTD vector<Tile>(_NSTD _LINKED_ARRAY<Tile>::* array)() const = &_NSTD _LINKED_ARRAY<Tile>::get_array;

	_NSTD vector<Tile> _tile_arr;
public:
	Maze(int dim_x = 0, int dim_y = 0, 
		char wall_char = '+', char space_char = ' ', char new_line_char = '\n', 
		std::string file_name = "Maze.txt") 
	{

		int dim[3] = { dim_x * dim_y, dim_x, dim_y };

		std::string courier_str;
		std::ifstream maze_file(file_name);

		if (maze_file.is_open())
			maze_file >> courier_str;
		else
			std::cout << "Failed to open file: " << file_name << "\n";

		_NSTD _LINKED_ARRAY<Tile> tiles;

		for (char c : courier_str) {
			Tile::Type t;
			if (c == new_line_char) t = Tile::Type::NewLine;
			elif(c == space_char)    t = Tile::Type::Space;
			elif(c == wall_char)     t = Tile::Type::Wall;
			else                      t = Tile::Type::Unknown;

			tiles.push_back(Tile(t, c));
		}
		_tile_arr = tiles.get_array();

		// Beginning/end logic

		for (int i = 0; i < (_tile_arr.*len)(); i++) {
			Tile curr_tile = _tile_arr[i];
			if (curr_tile.type_ == Tile::Type::NewLine) {
				_tile_arr[i - 1].type_ = Tile::Type::Border;
				_tile_arr[i + 1].type_ = Tile::Type::Border;
				if (curr_tile.type_ == Tile::Type::NewLine) {
					_tile_arr[i - 1].type_ = Tile::Type::Border;
					_tile_arr[i + 1].type_ = Tile::Type::Border;
				}

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
	Display() {}
};

_NSTD_END
#endif // ifndef _NSTD_MAZE_