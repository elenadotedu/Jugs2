#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>


class Settings
{
public:
	static Settings* instance();
	int getNumJugs() const;
	int getGoal() const;
	int getGoalIndex() const;
	int getCapacity(int index) const; //capacity of specific jug
	std::vector<int>* getCapacities() const; //a list of capacities
	int getStartWater(int index) const; //a list of starting water amounts
	std::vector<int>* getStartWater() const; //water amount in specific jug
	std::vector<int>* getGoalWater() const; //water amount in specific jug

	void setNumJugs(int num_jugs);
	void setGoal(int goal);
	void setGoalIndex(int goal_index);
	void addCapacity(int capacity);
	void addStartWater(int start_water);

	~Settings();
private:
	Settings();
	static Settings* m_instance;

	int m_num_jugs;
	int m_goal;
	int m_goal_index;

	std::vector<int>* m_capacities;
	std::vector<int>* m_start_water;
	std::vector<int>* m_goal_water;
};

#endif