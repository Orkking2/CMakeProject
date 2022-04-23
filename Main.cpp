#include <iostream>
#include "LinkedList.h"

int main()
{
	char* c = "stirng";
	_Linked_list<char> d;
	d.push_array_back(c, '\0');
	std::cout << d.front();
	_Linked_list<char> f;
	f.set_to_arr(d.get_array());
	std::cout << f.get_array();
}