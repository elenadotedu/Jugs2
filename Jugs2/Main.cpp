#include <iostream>
#include <fstream>
#include "Graph.h"
#include "Combination.h"
#include "Heap.h"
#include "Settings.h"

using namespace std;

vector<Combination> closedList; //list of nodes already closedList
Heap openList; //min heap containing nodes to visit

int findInClosedList(Combination combination);


int main()
{
	cout << "======================================" << endl;
	cout << "*           COMBINING JUGS           *" << endl;
	cout << "======================================" << endl << endl;

	// -----------------------------------------
	// Initialize the settings

	cout << "Enter the number of jugs." << endl;
	int num_jugs;
	cin >> num_jugs;

	//Save number of jugs to settings
	Settings::instance()->setNumJugs(num_jugs);

	cout << "Enter capacities. Press enter after each one." << endl;

	for (int i = 0; i < num_jugs; i++)
	{
		int capacity;
		cin >> capacity;
		Settings::instance()->addCapacity(capacity);
	}

	cout << "Enter initial water amounts. Press enter after each one." << endl;

	for (int i = 0; i < num_jugs; i++)
	{
		int water;
		cin >> water;
		Settings::instance()->addStartWater(water);
	}

	cout << "Enter goal amount." << endl;
	int goal;
	cin >> goal;
	Settings::instance()->setGoal(goal);

	cout << "In which jug would you like to have the goal? ( 0 to " << num_jugs - 1 << " )" << endl;
	int goal_index;
	cin >> goal_index;
	Settings::instance()->setGoalIndex(goal_index);

	// -----------------------------------------
	// Figure out the path

	bool goal_found = false;

	//create graph
	Graph graph;


	Combination start(Settings::instance()->getStartWater(), graph.heads[0]->h, graph.heads[0]->value);
	Combination goal_combination(Settings::instance()->getGoalWater());

	openList.push(start);

	//traverse graph till find a goal
	while (!openList.empty() && !graph.impossible)
	{
		Combination current = openList.top();
		openList.pop();

		current.print();

		if (current == goal_combination)
		{
			//print path to file
			ofstream output_file;
			output_file.open("output.txt");
			current.printPath(output_file);
			output_file.close();

			//print to screen
			current.printPath();

			goal_found = true;
			break;
		}

		//save as closedList
		int closedListIndex = findInClosedList(current);
		if (closedListIndex < 0)
		{
			closedList.push_back(current);
		}
		else
		{
			//if current node is better than closedList node, replace closedList with current
			if (closedList[closedListIndex].getG() + closedList[closedListIndex].getH() > current.getG() + current.getH())
			{
				closedList[closedListIndex] = current;
			}
			else {
				//do nothing, move to next
				continue;
			}
		}

		//if there is a goal in one of the jugs
		if (current.findJugIndexByWaterAmount(Settings::instance()->getGoal()) >= 0)
		{
			vector<Combination> new_combinations = graph.combineToStayOnGoal(current, Settings::instance()->getGoalIndex());
			for (int j = 0; j < new_combinations.size(); j++)
			{
				int closedListIndex = findInClosedList(new_combinations[j]);
				if (closedListIndex < 0 || closedList[closedListIndex].getG() + closedList[closedListIndex].getH() > new_combinations[j].getG() + new_combinations[j].getH())
				{
					openList.push(new_combinations[j]);
				}
			}
		}
		else
		{
			//if no goal yet, get child nodes from the graph
			vector<GraphNode* > children = graph.getChildren(current.graph_value);
			//for every child, try to make that child
			for (int i = 0; i < children.size(); i++)
			{
				//create combinations that add up to a child
				vector<Combination> new_combinations = graph.combineToMakeChild(current, children[i]);

				for (int j = 0; j < new_combinations.size(); j++)
				{
					int closedListIndex = findInClosedList(new_combinations[j]);
					if (closedListIndex < 0 || closedList[closedListIndex].getG() + closedList[closedListIndex].getH() > new_combinations[j].getG() + new_combinations[j].getH())
					{
						openList.push(new_combinations[j]);
					}
				}
			}
		}
	
	}

	if (!goal_found)
		cout << "Impossible" << endl;

	cout << endl;
	system("pause");
}


int findInClosedList(Combination combination)
{
	for (int i = 0; i < closedList.size(); i++)
	{
		if (combination == closedList[i])
			return i;
	}
	return -1;
}