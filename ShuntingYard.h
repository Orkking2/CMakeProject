#pragma once

#include <deque>
#include <string>
#include <vector>
#include <map>

struct Token {
	enum struct Type {
		Unknown,
		Number,
		Operator,
		LeftParen,
		RightParen,
		Variable,
	};

	Token(Type t, const std::string& s, int prec = -1, bool ra = false) : type{ t }, str(s), precedence{ prec }, rightAssociative{ ra }	{}

	const Type type;
	const std::string str;
	const int precedence;
	const bool rightAssociative;
};

std::deque<Token> ParseExpression(std::string& string);

double EvaluateExpression(std::deque<Token> queue, std::map<char, double> vars);