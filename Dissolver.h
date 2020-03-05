#ifndef DISSOLVER
#define DISSOLVER

#include "Cmd.h"
class Dissolver
{
	size_t functionid = 0;
	std::vector<std::string> externals;
	/*
		globals [funs,vars]
		locals [vars]
		class [vars,fun]
	*/

	void processExternals(Cmd*);
	void processFunctions(Cmd*);

	void processRegular(Cmd*&);
	void processRegularFunction(Cmd*& fun);
	void processRegularFunctionRecursion(Cmd*& fun,size_t& position,Cmd*& code);
public:
	Dissolver(Cmd*);
};
#endif // DISSOLVER
