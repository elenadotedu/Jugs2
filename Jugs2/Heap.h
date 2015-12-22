#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <queue> //for push_heap
#include "Combination.h"

struct Heap
{
	Heap();
	std::vector<Combination> elements;
	void push(Combination node);
	Combination top();
	void pop();
	bool empty();
	int size();
};

struct comparator{
	bool operator() (Combination first, Combination second){
		return first.getG() + first.getH() > second.getG() + second.getH();
	};
};


#endif