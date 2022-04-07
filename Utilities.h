#pragma once

#include "Image.h"
#include "Geometry.h"
#include <map>
#include <string>
#include <vector>
#include <deque>
#include <functional>
#include <unordered_map>

struct Hex {
	Hex() {
		for (int i = 0; i < key.size(); i++) {
			map[key[i]] = i;
		}
	}
	int HexToInt(std::string in) {
		int out = 0;
		for (int i = 0; i < in.size(); i++) {
			out += in[i] * pow(16, i);
		}
		return out;
	}
	std::string IntToHex(int in) {
		std::string out;
		while (in) {
			out += key[in % 16];
			in /= 16;
		}
		return out;
	}
	std::map<char, int> map;
	std::string key = "0123456789ABCDEF";
};

std::vector<std::vector<double>> PtListToXYList(std::vector<Point> pList) {
	std::vector<double> xList, yList;
	for (Point p : pList) {
		xList.push_back(p.x);
		yList.push_back(p.y);
	}
	return { xList, yList };
}

template <class Item>
void Sort(std::vector<Item>& list, std::function<bool(const Item& l, const Item& r)> c) {
	bool done = false;
	while (!done) {
		done = true;
		for (int i = 1; i < list.size(); i++) {
			if (c(list[i - 1], list[i])) {
				std::swap(list[i - 1], list[i]);
				done = false;
			}
		}
	}
}

std::string CharVectorToString(std::vector<char> in) {
	std::string out;
	for (char c : in) {
		out.push_back(c);
	}
	return out;
}

std::vector<char> StringToCharVector(std::string in) {
	std::vector<char> out;
	for (char c : in) {
		out.push_back(c);
	}
	return out;
}

std::deque<bool> AppendBoolDeque(std::vector<std::deque<bool>> in) {
	std::deque<bool> out;
	for (std::deque<bool> deque : in) {
		while (!deque.empty()) {
			out.push_back(deque.front());
			deque.pop_front();
		}
	}
	return out;
}

template<typename I, class Hasher = std::hash<I>>
std::unordered_map<I, int, Hasher> GenFreq(std::vector<I> in) {
	std::unordered_map<I, int, Hasher> out;
	for (I i : in) out[i]++;
	return out;
}


struct Hash {
	std::size_t operator () (const Point& p) const {
		std::size_t x = std::hash<double>{}(p.x);
		std::size_t y = std::hash<double>{}(p.y);
		return x ^ (y << 1);
	}
	std::size_t operator() (const Segment& s) const {
		std::size_t p1 = Hash{}(s.p1);
		std::size_t p2 = Hash{}(s.p2);
		return p1 ^ (p2 << 1);
	}
	std::size_t operator() (const Particle& p) const {
		std::size_t origin = Hash{}(p.p);
		std::size_t theta = std::hash<double>{}(p.theta);
		return origin ^ (theta << 1);
	}
	std::size_t operator() (const Pixel& p) const {
		std::size_t point = Hash{}(Point(p.x, p.y));
		std::size_t RGB = 0;
		for (int i : p.RGB) RGB ^= (std::hash<int>{}(i) << i);
		return point ^ (RGB << 1);
	}
	std::size_t operator() (Matrix m) const {
		std::size_t out = 0;
		for (Point p : m.getMatrix()) out ^= Hash{}(p);
		return out;
	}
	std::size_t operator() (PersonalImage image) const {
		std::size_t out = 0;
		for (Pixel p : image.GetImage()) out ^= (Hash{}(p) << 1);
		return out;
	}
};