#pragma once
#ifndef _MAZE_
#define _MAZE_

#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include "LinkedList.h"

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
	int* _dim;
	_Linked_list<Tile> _tiles;
public:
	maze(int dim_x, int dim_y, char wall_char = '+', char space_char = ' ', std::string file_name = "Maze.txt") {

		_dim = { dim_x * dim_y, dim_x, dim_y };

		std::string courier_str;

		std::ifstream maze_file(file_name);
		if (maze_file.is_open()) maze_file >> courier_str;
		else std::cout << "Failed to open file: " << file_name << "\n";
		
		for (char c : courier_str) {
			Tile::Type t;
			switch (c) {
			case space_char: t = Tile::Type::Space;
			case wall_char:  t = Tile::Type::Wall;
			case '\n':       t = Tile::Type::NewLine;
			default:         t = Tile::Type::Unknown;
			}
			_tiles.push_back(Tile(t, c));
		}

		// Start/exit logic


	}
	_Linked_list<Tile> get_tiles() {
		return _tiles;
	}
	int* get_dim() {
		return _dim;
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
	_Linked_list<Player> _players;
public:
	Display(){}
};


#endif // ifndef _MAZE_