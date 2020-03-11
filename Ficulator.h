#pragma once
#include "Cmd.h"
/*
    Das Ziel des Ficulatoren ist es,funktionen zu classen umzuwandeln falls nötig

    Schritte:
        1: find alle funktionen
        2: finde alle "NAME" tags und versuche sie aufzulösen
*/
class Ficulator
{
    std::vector<Cmd*> functiontodos;

	std::map<Cmd*,Cmd*> functionspace;//In which functionsspace is a variable defined?
	void registration(Cmd* current,Cmd* function = nullptr);
	
	void findName(Cmd* functionspace,Cmd* current);
	void solveName(Cmd* functionspace,Cmd* current);
	bool doesTheClassContainVar(Cmd* cls,Cmd* var,Cmd* current = nullptr);
	void solveName_class(Cmd* functionsspace,Cmd* current,Cmd* cls);
	void solveName_outsideClasss(Cmd* functionsspace,Cmd* current,Cmd* cls);


	Cmd* findClass(Cmd* functionsspace);
	void solveName_thereIsNoClasss(Cmd* functionsspace,Cmd* current);
	Cmd* getThisParameter(Cmd* function);
	void replace(Cmd*original,Cmd*replacement,Cmd*current);
public:
	Ficulator(Cmd*);
};
