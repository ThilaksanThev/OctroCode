#pragma once
#include <string>
#include "Tokens.h"
#include <map>

class Cmd
{
public:
	void* synSettings;
	int line = 0;
	std::string file = "";

	Cmd* parent = nullptr;

	std::string type = "";
	std::vector<Cmd*> children;
	std::string value;

	//getting delete later^^
	std::map<std::string, std::string> marker;
	std::map<std::string, Cmd*> markerCmd;
	std::vector<std::string> parameter;//if this is a function

	Cmd(void* synSettings);
	~Cmd();

	bool loadOperator(Tokens*s, size_t& i);
	bool loadClassCode(Tokens*, size_t&);
	bool loadKeyword(Tokens*s, size_t& i);
	bool loadMember(Tokens*s, size_t& i,Cmd*&);
	bool loadPrefix(Tokens*s, size_t& i, Cmd*&);
	bool loadSuffix(Tokens*s, size_t& i, Cmd*&);
	bool loadOperation_Order(size_t lvl);
	bool loadOperation_Gathering(Tokens*s, size_t& i);
	bool loadOperation(Tokens*s,size_t& i,Cmd*&);
	void loadCode(Tokens*);

	void print(std::string tab);

	void initParent();

	
};