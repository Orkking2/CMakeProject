#include <iostream>
#include "LinkedList.h"

int main()
{
	_LINKED_ARRAY<char> l;
	char* string = "string";
	l.push_array_back(string, '\0');
	std::cout << l.get_array() << "\n";
	l.remove_front();
	l.push_front(' ');
	std::cout << l.get_array() << "\n";

	std::system("pause");	
}