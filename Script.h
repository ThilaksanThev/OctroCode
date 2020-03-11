#pragma once
#include <string>
class Script
{
public:
	Script();
	~Script();
	void loadFromFile(std::string path);
};