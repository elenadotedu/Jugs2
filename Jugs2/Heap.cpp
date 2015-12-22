#include "Heap.h"

Heap::Heap()
{

};
void Heap::push(Combination node)
{
	elements.push_back(node);
	push_heap(elements.begin(), elements.end(), comparator());
	Combination front = elements.front();
};
Combination Heap::top()
{
	return elements.front();
};
void Heap::pop()
{
	std::pop_heap(elements.begin(), elements.end(), comparator());
	elements.pop_back();
};
bool Heap::empty()
{
	return elements.empty();
};
int Heap::size()
{
	return elements.size();
};