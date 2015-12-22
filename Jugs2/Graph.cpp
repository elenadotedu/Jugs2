#include "Graph.h"

GraphNode::GraphNode(int _value)
{
	value = _value;
	orphan = true;
	h = INT_MAX; //very large value by default
	g = INT_MAX; //very large value by default
	visited = false;
}

GraphEdge::GraphEdge(int _cost, int _first, int _second, int _third, char _operation)
{
	cost = _cost; //the cost required to take this edge

	//values that are being operated with to move to the next node
	first = _first;
	second = _second;
	third = _third;

	//operation performed on the values to move to the next node
	operation = _operation;
}

Graph::Graph() {

	//initialize arrays needed to generated the graph
	std::vector<GraphNode* > freshly_made;
	std::vector<GraphNode* > current;

	//get values from settings
	Settings* settings = Settings::instance();
	int num_jugs = settings->getNumJugs();
	int goal = settings->getGoal();

	//figure out max jug
	max_jug = 0;
	for (int i = 0; i < num_jugs; i++)
	{
		if (max_jug < settings->getCapacity(i))
		{
			max_jug = settings->getCapacity(i);
		}
	}

	//allocate an array to store each water amount that can be made
	//(from 0 to max_jug).
	num_array.resize(max_jug + 1);

	for (int i = 0; i < max_jug + 1; i++)
	{
		//for each possible water amount from 0 to max_jug, create a graph node;
		//g and h values will be largest possible since at this point
		//it is not known whether this amount can be made.
		num_array[i] = new GraphNode(i);
	}

	//initialize graph with start values, create heads
	for (int j = 0; j < num_jugs; j++)
	{
		int water_amount = settings->getStartWater(j);

		//if starting with the goal, then there is nothing to do, stop
		if (water_amount == goal)
			return;

		//if this amount is not yet linked to the graph head
		//(or if it exists at a higher level)
		if (num_array[water_amount]->orphan)
		{
			//add water amount to the heads (root) of the graph
			heads.push_back(num_array[water_amount]);

			//mark node as connected to other nodes in graph
			num_array[water_amount]->orphan = false;

			//add node to freshly made, so that it can be processed
			//i.e. operations will be performed to see which water amounts can be made 
			//from these freshly made amounts
			freshly_made.push_back(num_array[water_amount]);
		}
	}

	//process nodes, i.e. perform operations to combine freshly made amounts
	//and to obtain new values.
	while (!freshly_made.empty())
	{
		for (int i = 0; i < freshly_made.size(); i++)
		{
			//get freshly made amount that needs to be calculated
			int to_calculate = freshly_made[i]->value;

			//combine with jug capacities
			for (int j = 0; j < num_jugs; j++)
			{
				int value;

				//1. addition (cost = 2 - ie. fill jug, add to current jug)
				value = to_calculate + settings->getCapacity(j);
				GraphEdge* edge = new GraphEdge(1, to_calculate, settings->getCapacity(j), 0, '+');
				addNode(value, freshly_made[i], edge, current);

				//2. addition with overflow (cost = 2, ie fill jug, add to the current. That will create overflow, remaining water stays in first jug)
				for (int k = 0; value > max_jug && k < num_jugs; k++)
				{
					if (k == j)
						continue;

					int value2 = value - settings->getCapacity(k);
					GraphEdge* edge = new GraphEdge(2, to_calculate, settings->getCapacity(j), settings->getCapacity(k), 'o');
					addNode(value2, freshly_made[i], edge, current);
				}

				//3. subtraction (cost = 1, ie empty current jug into the other jug)
				value = to_calculate - settings->getCapacity(j);
				edge = new GraphEdge(1, to_calculate, settings->getCapacity(j), 0, '-');
				addNode(value, freshly_made[i], edge, current);
			}
		}
		freshly_made = current;
		current.clear();
	}

	//set the impossible flag
	if (num_array[goal]->orphan)
		impossible = true;
	else
		//assign h values upon graph creation
		assignH();
}

bool Graph::addNode(int value, GraphNode* mother, GraphEdge* edge, std::vector<GraphNode* > &current)
{
	bool added_node = false;

	//if value is within bounts
	if (value >= 0 && value <= max_jug)
	{
		//if node is not yet linked to anything, then this is a
		//new value, so needs to be processed as freshly made
		if (num_array[value]->orphan)
		{
			//push to current, so that it can be expanded
			current.push_back(num_array[value]);
			added_node = true;
		}
		//GraphEdge* edge = new GraphEdge(cost);

		//link mother to the node
		mother->children.push_back(num_array[value]);
		mother->edges_to_children.push_back(edge);

		//link child to mother
		num_array[value]->parents.push_back(mother);
		num_array[value]->edges_to_parents.push_back(edge);

		//not orphaned anymore
		num_array[value]->orphan = false;
	}

	return added_node;
}

int Graph::getH(int num)
{
	return num_array[num]->h;
}

void Graph::assignH()
{
	//mark ea graph node as unassigned
	std::queue<GraphNode* > unassigned;

	//start with goal
	num_array[Settings::instance()->getGoal()]->h = 0;
	unassigned.push(num_array[Settings::instance()->getGoal()]);

	//walk backwards through the graph and assign h
	while (!unassigned.empty())
	{
		GraphNode* current = unassigned.front();
		unassigned.pop();

		//go through parents and assign h values
		for (int i = 0; i < current->parents.size(); i++)
		{
			GraphNode* parent = current->parents[i];
			GraphEdge* edge_to_parent = current->edges_to_parents[i];

			//new h is a combination of cost and the h of parent
			int new_h = current->h + edge_to_parent->cost;

			//if there isn't already another (shorter) path
			//that has gone through this node, set h to just calculated hs
			if (parent->h > new_h)
			{
				parent->h = new_h;
				unassigned.push(parent);
			}
		}
	}
}

std::vector<GraphNode*> Graph::getChildren(int num)
{
	return num_array[num]->children;
}

std::vector<Combination> Graph::combineToMakeChild(Combination combination, GraphNode* child)
{
	int NUM_JUGS = Settings::instance()->getNumJugs();

	std::vector<Combination> combinations;

	//get the current combination's graph node
	GraphNode* parent = num_array[combination.graph_value];

	//this is the value to be used for calculations
	int to_calculate = combination.graph_value;

	//the value trying to make
	int goal_value = child->value;


	int child_index;

	//find child index
	for (int i = 0; i < parent->children.size(); i++)
	{
		if (parent->children[i]->value == child->value)
		{
			child_index = i;
			break;
		}
	}

	//get appropriate edge
	GraphEdge* edge = parent->edges_to_children[child_index];
	int capacity1 = edge->second;
	int capacity2 = edge->third;
	int to_calculate_index = combination.findJugIndexByWaterAmount(to_calculate);
	int capacity_index1 = combination.findJugIndexByCapacity(capacity1);

	if (edge->operation == '+')
	{
		//if goal is just one of the capacities, then it's simple, just add water to that jug
		if (goal_value == capacity1)
		{
			Combination child_combination(&combination.water, child->h, goal_value);
			child_combination.addMove(-1, capacity_index1);
			combinations.push_back(child_combination);
		}
		else
		{
			//if the jug with capacity that needs to be added has the current amount
			if (to_calculate_index == capacity_index1 || Settings::instance()->getCapacity(to_calculate_index) < to_calculate + capacity1)
			{
				//decide where to pour
				for (int k = 0; k < NUM_JUGS; k++)
				{
					if (k == capacity_index1 || Settings::instance()->getCapacity(k) <= to_calculate + capacity1)
						continue;

					Combination child_combination(&combination.water, child->h, goal_value);
					child_combination.path = combination.path;

					//if there water in the jug, pour it out first
					if (child_combination.hasWater(k))
					{
						child_combination.addMove(k, -1);
					}

					child_combination.addMove(to_calculate_index, k);

					//check if don't need to fill it up
					if (!child_combination.isFull(capacity_index1))
						child_combination.addMove(-1, capacity_index1);

					child_combination.addMove(capacity_index1, k);
					combinations.push_back(child_combination);

				}
			}
			else
			{
				Combination child_combination(&combination.water, child->h, goal_value);
				child_combination.path = combination.path;

				if (!child_combination.isFull(capacity_index1))
					child_combination.addMove(-1, capacity_index1);

				child_combination.addMove(capacity_index1, to_calculate_index);

				combinations.push_back(child_combination);
			}
		}
	}
	else if (edge->operation == '-')
	{
		//if (to_calculate - capacity1 == goal_value)
		//{
			int to_calculate_jug = combination.findJugIndexByWaterAmount(to_calculate);

			//create combination 
			Combination child_combination(&combination.water, child->h, goal_value);
			child_combination.path = combination.path;
					
			if (child_combination.hasWater(capacity_index1))
			{
				child_combination.addMove(capacity_index1, -1);
			}

			child_combination.addMove(to_calculate_jug, capacity_index1);
			combinations.push_back(child_combination);
		//}
	}

	//if addition with overflow
	else if (edge->operation == 'o')
	{
		Combination child_combination1(&combination.water, child->h, goal_value);
		child_combination1.path = combination.path;

		Combination child_combination2(&combination.water, child->h, goal_value);
		child_combination2.path = combination.path;

		int capacity_index2 = combination.findJugIndexByCapacity(capacity2);
		
		if (combination.hasWater(capacity_index2))
		{
			child_combination1.addMove(capacity_index2, -1);
		}

		//need to check if first one fits into the jug
		//in this situation the amount will end up in the capacity index1
		if (to_calculate <= capacity2)
		{
			child_combination1.addMove(to_calculate_index, capacity_index2);

			if (!child_combination1.isFull(capacity_index1))
				child_combination1.addMove(-1, capacity_index1);

			child_combination1.addMove(capacity_index1, capacity_index2);
			combinations.push_back(child_combination1);
		}

		//check if can fit capacity into the other jug
		//in this case target amount will end up in to_calculate_index
		if (capacity1 <= capacity2)
		{
			//check if the jug is already full
			if (!child_combination2.isFull(capacity_index1))
				child_combination2.addMove(-1, capacity_index1);

			child_combination2.addMove(capacity_index1, capacity_index2);
			child_combination2.addMove(to_calculate_index, capacity_index2);
			combinations.push_back(child_combination2);
		}
	}
	return combinations;
}

std::vector<Combination> Graph::combineToStayOnGoal(Combination combination, int goal_index)
{
	int NUM_JUGS = Settings::instance()->getNumJugs();
	int GOAL = Settings::instance()->getGoal();

	std::vector<Combination> combinations;
	Combination child_combination(&combination.water, 0, GOAL);
	child_combination.path = combination.path;

	int current_goal_index = combination.findJugIndexByWaterAmount(GOAL);

	if (goal_index != current_goal_index)
	{
		if (child_combination.hasWater(goal_index))
			child_combination.addMove(goal_index, -1);
		
		child_combination.addMove(current_goal_index, goal_index);
	}

	for (int i = 0; i < NUM_JUGS; i++)
	{
		if (i == goal_index)
			continue;

		//if there is water in other jugs, pour it out
		if (child_combination.hasWater(i))
			child_combination.addMove(i, -1);
	}

	combinations.push_back(child_combination);
	return combinations;
}