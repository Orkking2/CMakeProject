#include <iostream>
#include "LinkedList.h"
#include <cstdlib>

int main()
{
	_Linked_list<char> l;
	char* string = "string";
	l.push_array_back(string, '\0');
	std::cout << l.get_array() << "\n";
	std::system("pause");
}