#include "SynSettings.h"



std::vector<std::string>& SynSettings::getOperators()
{
	return operators;
}

size_t SynSettings::operationLevelCount()
{
	return orderOfOperation.size();
}

std::vector<std::string>& SynSettings::getOperators(size_t lvl)
{
	return orderOfOperation[lvl].second;
}

bool & SynSettings::getOperatorDirection(size_t lvl)
{
	return orderOfOperation[lvl].first;
}

SynSettings::SynSettings()
{
}


SynSettings::~SynSettings()
{
}
#include <iostream>
void SynSettings::setOrderOfOperation(std::vector<std::pair<bool,std::vector<std::string>>> o)
{
	orderOfOperation = o;

	//order the operators , the biggest first
	operators.clear();
	for (auto a : o)
		for (auto b : a.second) {
			size_t pos = 0;
			bool run = true;
			while (run) {
				if (operators.size() > pos)
					if (operators[pos].size() < b.size()) {
						operators.insert(operators.begin() + pos, b);
						run = false;
					}
					else pos++;
				else {
					operators.push_back(b);
					run = false;
				}
			}
		}
	/*for (auto x : operators)
		std::cout << x << std::endl;*/
}
