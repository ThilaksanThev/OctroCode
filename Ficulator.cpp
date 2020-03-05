#include "Ficulator.h"

Ficulator::Ficulator(Cmd* c)
{
	registration(c);
	for (auto& x : c->children) {
		if (x->type == "FUN")
			functiontodos.push_back(x);
	}
	for(auto x: functiontodos){
        process1(x,x);
    }
}
void Ficulator::registration(Cmd* current, Cmd* function)
{
	if (current->type == "VAR")
		functionspace[current] = function;
	else if (current->type == "INIT")
		functionspace[current->markerCmd["var"]] = function;
	if (current->type == "FUN")
		function = current;

	for (auto x : current->children)
		registration(x, function);
}

/*void Ficulator::process(Cmd* c,Cmd* f)
{
	for (auto x : c->children)
		process(x,f);

	if (c->type == "NAME") {
		auto fs = functionspace[c->markerCmd["var"]];
		if (fs == f) {
			//Eine Lokale variable #lame
		}
		else if (fs == nullptr) {
			//Eine Global
		}
		else {
			//Jetzt wird es spannend
			//process1(c,f->markerCmd["INIT"]);

			c->type = "SUFFIX";
			c->marker.clear();
			c->markerCmd.clear();
			//this.name;
			auto n = new Cmd(nullptr);
			n->type = "NAME";
			n->value = "this";
			c->children.push_back(n);

		}
	}
}*/

void Ficulator::process1(Cmd* fun,Cmd* current){
/*
Gehe alle funktionen durch:
	Suche nach einem „Name“ tag
	Ist das eine Variable,die nicht global ist und nicht in dieser funktion ist?
	Ja:	Leite prozess 1.2 ein
*/
    for(auto x:current->children)
    {
        if(x->type=="NAME")
        {
            auto fs = functionspace[x->markerCmd["var"]];
            if (fs == fun) {
                //Eine Lokale variable #lame
            }
            else if (fs == nullptr) {
                //Eine Global
            }
            else{
                auto var = x->markerCmd["var"];
                //this.name
                x->type = "SUFFIX";
                x->marker.clear();
                x->markerCmd.clear();
                x->value = ((char)32)+x->value;
                auto n = new Cmd(nullptr);
                n->type = "NAME";
                n->value = "this";
                x->children.push_back(n);

                //process2
                if(fs->marker["method"]=="true")
                    process4(fs,fun->markerCmd["init"],var,x->value);
                else
                    process3(fs,fun->markerCmd["init"],var,x->value);
            }
        }else
        process1(fun,x);
    }
}
void Ficulator::process3(Cmd* vaterfun,Cmd* init,Cmd* variable,std::string parametername){
/*
	Suche in der Vaterfunktion nach dem init dieser funktion.
	Erstelle eine Classe um es herum:
		In dieser Classe gibt es den „init“ der funktion
		Es gibt den gewünschten Parameter als : hex(00)+name
	Vaterfunktion auf die to-do liste setzen.
*/
    Cmd* cls = new Cmd(nullptr);
    cls->type = "CLASS";
    cls->children.push_back(init);
    init->markerCmd["var"]->marker["method"] = "true";
    init->parent = cls;
    {
        Cmd* var = new Cmd(nullptr);
        var->type = "VAR";
        functionspace[var] = vaterfun;
        var->parent = cls;
        var->value = parametername;
        {
            Cmd* name = new Cmd(nullptr);
            name->type = "NAME";
            name->value = variable->value;
            name->markerCmd["var"] = variable;
            name->parent = var;
            var->children.push_back(name);
        }
        cls->children.push_back(var);
    }
    process3_1(vaterfun,init,cls);
    functiontodos.push_back(vaterfun);
}
bool Ficulator::process3_1(Cmd* current,Cmd* init,Cmd* replacement){
    for(Cmd*& x:current->children){
        //std::cout <<x->type<< x << " "<<x->t<<init<<std::endl;
        if(x==init){
            x=replacement;
            return true;
        }
        if(process3_1(x,init,replacement))
            return true;
    }
    return false;
    std::cout << "stop"<<std::endl;
}
void Ficulator::process4(Cmd* vaterfun,Cmd* init,Cmd* variable,std::string parametername){
/*
	Suche nach der ParentNode der init node (muss „class“ sein)
	Ist variable innerhalb der classe?also hex(00)+name
	wenn nein:
		Es gibt den gewünschten marker h(00)
		füge Vaterfunktion zu to-do liste.
*/
    Cmd* cls = process4_1(vaterfun,init);
    if(cls==nullptr){
        //ERROR methode nicht gefunden;
        1/0;
        return;
    }
    if(!process4_2(cls,parametername)){
        Cmd* var = new Cmd(nullptr);
        var->type = "VAR";
        functionspace[var] = vaterfun;
        var->parent = cls;
        var->value = parametername;
        {
            Cmd* name = new Cmd(nullptr);
            name->type = "NAME";
            name->value = variable->value;
            name->markerCmd["var"] = variable;
            name->parent = var;
            var->children.push_back(name);
        }
        cls->children.push_back(var);

        functiontodos.push_back(vaterfun);
    }

}
Cmd* Ficulator::process4_1(Cmd* current,Cmd* init){
    for(auto x : current->children){
        if(current->type=="CLASS"&&init == x)
            return current;
        auto a = process4_1(x,init);
        if(a!=nullptr)
            return a;
    }
    return nullptr;
}
bool Ficulator::process4_2(Cmd* cls,std::string parametername){
    for(auto x:cls->children){
        if(x->type=="VAR"&&x->value == parametername)
            return true;
    }
    return false;
}
