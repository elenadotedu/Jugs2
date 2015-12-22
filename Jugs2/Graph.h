#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <iostream>
#include "Combination.h"
#include "Settings.h"

struct GraphEdge;

struct GraphNode {
	int value;
	int h;
	int g;
	bool orphan;
	bool visited;
	Combination* best_combination;

	//these arrays must be parallel to each other
	std::vector <GraphNode* > children;
	std::vector <GraphNode* > parents;
	std::vector <GraphEdge* > edges_to_children;
	std::vector <GraphEdge* > edges_to_parents;

	GraphNode(int _value);
};

struct GraphEdge {
	int cost;
	
	//numbers to add and subtract
	int first;
	int second;
	int third;

	//+,- or o (plus with overflow)
	char operation;
	
	GraphEdge(int _cost, int _first, int _second, int _third, char _op);
};

class Graph {
public:
	std::vector <GraphNode* > heads;
	int impossible;

	Graph();
	std::vector<GraphNode*> Graph::getChildren(int num);
	std::vector<Combination> Graph::combineToMakeChild(Combination combination, GraphNode* child);
	void Graph::combineToMakeChild_ByAddition(std::vector<Combination>& combinations);
	std::vector<Combination> Graph::combineToStayOnGoal(Combination combination, int goal_index);
	//void print();

private:
	std::vector<GraphNode* > num_array;
	bool addNode(int value, GraphNode* mother, GraphEdge* edge, std::vector<GraphNode* > &current);

	int max_jug;

	//void print_r(GraphNode* node, std::vector<bool> &printed); //print graph
	int getH(int num); //get h to the goal for a specific numeric value 
	void assignH(); //assigns h values to all graph nodes
};

#endif