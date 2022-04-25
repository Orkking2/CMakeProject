#include <iostream>
#include "LinkedList.h"

int main()
{
	_Linked_list<char> l;
	char* string = "string";
	l.push_array_back(string, '\0');
	std::cout << l.get_array() << "\n";
	l.pop_back();
	std::system("pause");

	_Linked_item<char> item(*string, NULL, NULL);
	_Linked_item<char>* item_ptr = &item;
	item_ptr = nullptr;
}