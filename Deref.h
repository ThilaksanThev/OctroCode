#pragma once
#include "Cmd.h"

class Deref
{
	std::map<std::string,std::pair<std::string,std::string>> codes;

public:
	Deref(std::vector<std::pair<std::string,std::pair<std::string,std::string>>> codes);
	void extractFunctions(Cmd*,Cmd* lastCodeUnit);
	void markCodeUnits(Cmd*);
	
	void resolve(Cmd*,std::vector<Cmd*>& vars);
	void resolve(Cmd*,std::vector<Cmd*>& vars,Tokens*);
	
	static void initErase(Cmd* code);
	static void varSet(Cmd* code);

};