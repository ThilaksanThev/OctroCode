#ifndef FICULATOR
#define FICULATOR
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

	//suche FiC variablen
	//void process(Cmd* c,Cmd* function);


	//Suche nach NameTags in jeder function
	void process1(Cmd* fun,Cmd* current);
	void process3(Cmd* vaterfun,Cmd* init,Cmd* variable,std::string parametername);
	//Sucht nacht der init,ersetzt diesen mit dem Replacement
	//returntype: erfolgreich?
	bool process3_1(Cmd* current,Cmd* init,Cmd* replacement);

	void process4(Cmd* vaterfun,Cmd* init,Cmd* variable,std::string parametername);
    //sucht nach der classe,ansonten nullptr
    Cmd* process4_1(Cmd* current,Cmd* init);
    //exesitert der name schon als variable in der class?
    bool process4_2(Cmd* cls,std::string parametername);

public:
	Ficulator(Cmd*);
};
#endif
