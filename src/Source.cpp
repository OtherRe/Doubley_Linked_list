#include <iostream>
#include "LinkedList.h"
using namespace std;

int main()
{
	aisdi::LinkedList<int> collection;
  aisdi::LinkedList<int> other = { 100, 200, 300, 400 };
  other = std::move(collection);

	for (auto i: other)
		cout<<i<<endl;
  
	return 0;
}
