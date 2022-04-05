#pragma once

#include <deque>
#include <string>
#include <vector>

struct Token;

std::deque<Token> ParseExpression(std::string& string);

double EvaluateExpression(std::deque<Token> queue, std::map<char, double> vars);

