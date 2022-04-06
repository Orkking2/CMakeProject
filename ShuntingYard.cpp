#include "ShuntingYard.h"

std::deque<Token> ParseExpression(std::string& string) {
	std::deque<Token> tokens;

	for (const auto* c = string.c_str(); *c; c++) {
		if (isblank(*c)) {
			// pass
		}
		else if (isdigit(*c)) {
			const auto* b = c;
			auto* e = c;

			while (isdigit(*e)) {
				e++;
			}
			const auto s = std::string(b, e);
			tokens.push_back(Token{ Token::Type::Number, s });
			c += s.size();
		}
		else if (isalpha(*c)) {
			tokens.push_back(Token{ Token::Type::Variable, std::string(1, *c) });
		}
		else {
			Token::Type t = Token::Type::Unknown;
			int pr = -1;
			bool ra = false;
			switch (*c) {
			default:                                    break;
			case '(':   t = Token::Type::LeftParen;     break;
			case ')':   t = Token::Type::RightParen;    break;
			case '_':   t = Token::Type::Operator;      pr = 5; ra = true; break;
			case '^':   t = Token::Type::Operator;      pr = 4; ra = true; break;
			case '*':   t = Token::Type::Operator;      pr = 3; break;
			case '/':   t = Token::Type::Operator;      pr = 3; break;
			case '%':   t = Token::Type::Operator;      pr = 3; break;
			case '+':   t = Token::Type::Operator;      pr = 2; break;
			case '-':   t = Token::Type::Operator;      pr = 2; break;
			}
			const auto s = std::string(1, *c);
			tokens.push_back(Token{ t, s, pr, ra });
		}
	}

	// ShuntingYard

	std::deque<Token> queue;
	std::vector<Token> stack;

	for (Token token : tokens) {
		switch (token.type) {
		case Token::Type::Number:
			queue.push_back(token);
			break;

		case Token::Type::Operator:
		{
			const auto o1 = token;
			while (!stack.empty()) {
				const auto o2 = stack.back();
				if ((!o1.rightAssociative && o1.precedence <= o2.precedence) || (o1.rightAssociative && o1.precedence < o2.precedence)) {
					stack.pop_back();
					queue.push_back(o2);

					continue;
				}
				break;
			}
			stack.push_back(o1);
			break;
		}
		case Token::Type::LeftParen:
			stack.push_back(token);
			break;

		case Token::Type::RightParen:
		{
			bool match = false;
			while (!stack.empty() && stack.back().type != Token::Type::LeftParen) {
				queue.push_back(stack.back());
				stack.pop_back();
				match = true;
			}
			stack.pop_back();

			if (!match && stack.empty()) {
				printf("RightParen error (%s)\n", token.str.c_str());
				return {};
			}
			break;
		}
		default:
			printf("error (%s)\n", token.str.c_str());
			return {};
		}
	}
	while (!stack.empty()) {
		if (stack.back().type == Token::Type::LeftParen) {
			printf("Mismatched parentheses error\n");
			return {};
		}

		queue.push_back(std::move(stack.back()));
		stack.pop_back();
	}
	return queue;
}

double EvaluateExpression(std::deque<Token> queue, std::map<char, double> vars) {
	std::vector<int> stack;
	while (!queue.empty()) {
		const auto token = queue.front();
		queue.pop_front();
		switch (token.type) {
		case Token::Type::Number:
			stack.push_back(std::stoi(token.str));
			break;
		case Token::Type::Variable:
			stack.push_back(vars[token.str[0]]);
			break;
		case Token::Type::Operator:
		{
			const auto r = stack.back();
			stack.pop_back();
			if (token.str[0] == '_') {
				stack.push_back(-r);
				break;
			}
			const auto l = stack.back();
			stack.pop_back();

			switch (token.str[0]) {
			default:
				printf("Operator error [%s]\n", token.str.c_str());
				exit(0);
				break;
			case '^':	stack.push_back(static_cast<int>(pow(l, r)));	break;
			case '*':	stack.push_back(l * r);							break;
			case '/':	stack.push_back(l / r);							break;
			case '+':	stack.push_back(l + r);							break;
			case '-':	stack.push_back(l - r);							break;
			case '%':	stack.push_back(l % r);							break;
			}
		}
		break;

		default:
			printf("Token error\n");
			exit(0);
		}
	}
	return stack.back();
}