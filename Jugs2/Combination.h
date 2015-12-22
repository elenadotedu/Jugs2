#ifndef COMBINATION_H
#define COMBINATION_H

#include <vector>
#include <iostream>
#include <fstream>

struct Move {
	int from;
	int to;
	Move(int from, int to);
};

class Combination {
public:

	//ea jugs may contain various values, but this value
	//corresponds to the one that matters (this is the value on
	//the node of the graph, to which combination is attached to)
	int graph_value; 

	int h;

	//constructors
	Combination();
	Combination(std::vector<int>* wat);
	Combination(std::vector<int>* wat, int _h, int _graph_value);

	//adds a move to the path, rearranges water in the jugs appropriately
	void addMove(int first, int second);

	//searches the index of the jug by the amount of water in it
	int findJugIndexByWaterAmount(int water_amount);

	//searches the index of the jug by it's capacity
	int findJugIndexByCapacity(int capacity);

	//returns true if jug has water in it, full otherwise
	bool hasWater(int jug_index);

	//returns true if water in the jug = capacity
	bool isFull(int jug_index);

	//returns g
	int getG();

	//returns h
	int getH();

	//prints the combination (water amounts, g, h, path) to the screen
	void print();

	//prints path to the screen
	void printPath();

	//prints the path to the file
	void printPath(std::ofstream &out);

	//returns true if water amounts in the jugs are the same
	bool operator==(Combination second);

	//returns true if at least 1 jug contains a different amount of water
	bool operator!=(Combination second);

	//amounts of water in each jug
	std::vector<int> water; // how much water in ea jug

	std::vector<Move> path; //path to get to this combination

private:
	
};

#endif