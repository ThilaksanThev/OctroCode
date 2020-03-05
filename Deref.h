#ifndef DEREF
#define DEREF

#include "Cmd.h"

class Deref
{
	Deref* parent = nullptr;

	std::vector<Cmd*> functions;
	std::vector<Cmd*> variables;

	/*
		Process A:
		Hunt for functions in code
	*/
	void A_processCmd(Cmd*&);
	void A_processCode(Cmd*&);



	/*
		Process B:
		variablen hunting
		create variables.
		create references.
		process functions and classes.
		process code and ifs.
	*/
	void B_processCode(Cmd*&);
	std::vector<Cmd*> B_processCmd(Cmd*&);
	Cmd* search(std::string name);

	void processFun(Cmd*&);
	void processCLS(Cmd*&);

public:
	std::vector<Cmd*> allfunctions;

	Deref(Cmd* code,Deref* parent = nullptr);
};

#endif // DEREF
