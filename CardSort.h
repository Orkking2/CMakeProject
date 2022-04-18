#pragma once

#include <unordered_map>

struct hasher {
	std::size_t operator() (const char& c) {
		return std::hash<char>{}(c);
	}
};

