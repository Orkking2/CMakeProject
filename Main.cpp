#include "Deque.h"


int main()
{
	Deque<char> d;
	d.push_array_back("string", '\0');
	while (d.front() != '\0') {
		std::cout << d.pop_front();
	}
}