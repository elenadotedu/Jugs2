#include "Combination.h"
#include "Settings.h"

Move::Move(int f, int t)
{
	from = f;
	to = t;
}

Combination::Combination()
{
	int NUM_JUGS = Settings::instance()->getNumJugs();

	graph_value = 0;
	water.resize(NUM_JUGS);
	for (int i = 0; i < NUM_JUGS; i++)
	{
		water[i] = 0;
	}
}

Combination::Combination(std::vector<int>* wat)
{
	int NUM_JUGS = Settings::instance()->getNumJugs();
	h = INT_MAX;
	graph_value = 0;
	water.resize(NUM_JUGS);
	for (int i = 0; i < NUM_JUGS; i++)
	{
		water[i] = wat->at(i);
	}
}

Combination::Combination(std::vector<int>* wat, int _h, int _graph_value)
{
	int NUM_JUGS = Settings::instance()->getNumJugs();

	h = _h;
	graph_value = _graph_value;

	water.resize(NUM_JUGS);
	for (int i = 0; i < NUM_JUGS; i++)
	{
		water[i] = wat->at(i);
	}

}

bool Combination::operator==(Combination second){
	int NUM_JUGS = Settings::instance()->getNumJugs();

	for (int i = 0; i < NUM_JUGS; i++)
	{
		if (water[i] != second.water[i])
		{
			return false;
		}
	}
	return true;
}

bool Combination::operator!=(Combination second)
{
	int NUM_JUGS = Settings::instance()->getNumJugs();
	for (int i = 0; i < NUM_JUGS; i++)
	{
		if (water[i] != second.water[i])
		{
			return true;
		}
	}
	return false;
}

void Combination::addMove(int first, int second)
{
	int secondCapacity = Settings::instance()->getCapacity(second);

	if (second == -1)
		water[first] = 0;
	else if (first == -1)
		water[second] = secondCapacity;
	else
	{
		int sum = water[first] + water[second];

		if (sum >= Settings::instance()->getCapacity(second))
		{
			water[first] = sum - secondCapacity;
			water[second] = secondCapacity;
		}
		else
		{
			water[first] = 0;
			water[second] = sum;
		}
	}
	path.push_back(Move(first, second));
}

int Combination::findJugIndexByWaterAmount(int water_amount)
{
	int NUM_JUGS = Settings::instance()->getNumJugs();
	for (int i = 0; i < NUM_JUGS; i++)
	{
		if (water_amount == water[i])
			return i;
	}
	return -1;
}

int Combination::findJugIndexByCapacity(int capacity)
{
	int NUM_JUGS = Settings::instance()->getNumJugs();

	for (int i = 0; i < NUM_JUGS; i++)
	{
		if (Settings::instance()->getCapacity(i) == capacity)
			return i;
	}
	return -1;
}

bool Combination::hasWater(int jug_index)
{
	if (water[jug_index] != 0)
		return true;
	return false;
}

bool Combination::isFull(int jug_index)
{
	if (water[jug_index] == Settings::instance()->getCapacity(jug_index))
		return true;
	return false;
}

void Combination::printPath()
{
	std::cout << "[";

	if (path.size() > 0)
	{
		for (int i = 0; i < path.size() - 1; i++)
		{
			std::cout << "( ";

			std::cout << path[i].from << ", " << path[i].to;

			std::cout << " ), ";
		}
		//last item (no comma at the end)
		std::cout << "( ";

		std::cout << path[path.size() - 1].from << ", " << path[path.size() - 1].to;

		std::cout << " )";
	}

	std::cout << "]";
}

void Combination::printPath(std::ofstream &out)
{
	out << "[";

	if (path.size() > 0)
	{
		for (int i = 0; i < path.size() - 1; i++)
		{
			out << "( ";

			out << path[i].from << ", " << path[i].to;

			out << " ), ";
		}
		//last item (no comma at the end)
		out << "( ";

		out << path[path.size() - 1].from << ", " << path[path.size() - 1].to;

		out << " )";
	}

	out << "]";
}

int Combination::getG()
{
	return path.size();
}

int Combination::getH()
{
	return h;
}

void Combination::print()
{
	int NUM_JUGS = Settings::instance()->getNumJugs();
	std::cout << std::endl << "-------------------------------" << std::endl;
	std::cout << "Water: ";
	for (int i = 0; i < NUM_JUGS; i++)
	{
		std::cout << water[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "Path: ";
	printPath();
	std::cout << std::endl << "g = " << getG() << " h = " << getH() << std::endl;
	std::cout << std::endl << "-------------------------------" << std::endl;
}
