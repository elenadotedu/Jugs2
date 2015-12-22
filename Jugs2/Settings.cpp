#include "Settings.h"
#include <iostream>

// Global initialization to facilitate singleton design pattern
Settings* Settings::m_instance = 0;  

/* ------------------------*
   CONSTRUCTORS
*  ------------------------*/

Settings::Settings() : m_num_jugs(0), m_goal(0), m_goal_index(0)
{
	m_capacities = new std::vector<int>();
	m_start_water = new std::vector<int>();
	m_goal_water = new std::vector<int>();
}

/* ------------------------*
   INSTANCE
*  ------------------------*/
Settings* Settings::instance()
{
		if (!m_instance)   // Only allow one instance of class to be generated.
			m_instance = new Settings;

		return m_instance;
}

/* ------------------------*
    GETTERS 
*  ------------------------*/

int Settings::getNumJugs() const {
	return m_num_jugs;
}

int Settings::getGoal() const {
	return m_goal;
}

int Settings::getGoalIndex() const {
	return m_goal_index;
}

int Settings::getCapacity(int index) const {

	//if index is invalid, return -1
	if (index < 0 || index > m_capacities->size() - 1)
		return -1;
	else
		return m_capacities->at(index);
}

int Settings::getStartWater(int index) const {

	//if index is invalid, return -1
	if (index < 0 || index > m_start_water->size() - 1)
		return -1;
	else
		return m_start_water->at(index);
}

std::vector<int>* Settings::getStartWater() const {
	return m_start_water;
}

std::vector<int>* Settings::getCapacities() const {
	return m_capacities;
}

std::vector<int>* Settings::getGoalWater() const {
	if (m_goal_water->size() <= 0)
	{
		for (int i = 0; i < m_num_jugs; i++)
		{
			if (i == m_goal_index)
				m_goal_water->push_back(m_goal);
			else
				m_goal_water->push_back(0);
		}
	}
	return m_goal_water;
}

/* ------------------------*
   SETTERS
*  ------------------------*/

void Settings::setNumJugs(int num_jugs)
{
	m_num_jugs = num_jugs;
}

void Settings::setGoal(int goal)
{
	m_goal = goal;
}

void Settings::setGoalIndex(int goal_index)
{
	m_goal_index = goal_index;
}

void Settings::addCapacity(int capacity)
{
	m_capacities->push_back(capacity);
}

void Settings::addStartWater(int start_water)
{
	m_start_water->push_back(start_water);
}

/* ------------------------*
   DESTRUCTOR
*  ------------------------*/

Settings::~Settings() {
	delete m_instance;
	delete m_capacities;
	delete m_start_water;
}

