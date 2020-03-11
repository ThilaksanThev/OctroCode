#include "Ficulator.h"

Ficulator::Ficulator(Cmd* c)
{
    c->initParent();
	registration(c);

	for(auto x: functiontodos){
        findName(x,x);
    }
}
void Ficulator::registration(Cmd* current, Cmd* function)
{
	if (current->type == "VAR"||current->type=="CLASS")
		functionspace[current] = function;
	if (current->type == "FUN"){
        functionspace[current] = function;
		function = current;
        functiontodos.push_back(current);
    }
	for (auto x : current->children)
		registration(x, function);
}

void Ficulator::findName(Cmd* fs,Cmd* current){
    if(current->type=="NAME"){
        solveName(fs,current);
    }
    else for(auto& x :current->children){
        findName(fs,x);
    }
}
void Ficulator::solveName(Cmd* fs,Cmd* current){
    //current = NAME
    //fs = FUNCTION
    auto cls = findClass(fs);//class der funktion
    auto var = current->markerCmd["var"];

    if(functionspace[var]==fs){
        //Ein direkter Lokal
    }
    else if(cls!=nullptr){
        if(doesTheClassContainVar(cls,var))
        {
            solveName_class(fs,current,cls);
        }else if(functionspace[var]==nullptr){
            //Ein Global 
        }else {
            solveName_outsideClasss(fs,current,cls);
        }
    }else if(functionspace[var]==nullptr)
    {
        //Ein global
    }
    else {
        solveName_thereIsNoClasss(fs,current);
    }
}
Cmd* Ficulator::findClass(Cmd* fs){
    if(fs->parent->type=="CLASS")
        return fs->parent;
    return nullptr;
}
void Ficulator::solveName_thereIsNoClasss(Cmd* fs,Cmd* current){
    //CLASS erstellt
    Cmd* cls = new Cmd(fs->synSettings);
    cls->type = "CLASS";
    cls->value = fs->value;
    cls->line = fs->line;
    cls->file = fs->file;
    
    cls->parent = fs->parent;
    fs->parent = cls;

    //fuege VAR "name" dazu
    Cmd* var = new Cmd(fs->synSettings);
    var->type = "VAR";
    var->value = fs->value;
    var->line = fs->line;
    var->file = fs->file;
    var->parent = cls;
    var->value = current->value;
    var->markerCmd["repush"] = current->markerCmd["var"];
    cls->children.push_back(var);

    //function umbennen
    fs->value = "operatorEXEC";
    cls->children.push_back(fs);

    //this.name
    current->type="SUFFIX";
    auto name = new Cmd(current->synSettings);
    name->file = current->file;
    name->line = current->line;
    name->parent = current;
    name->type="NAME";
    name->value = "this";
    current->children.push_back(name);
    name->markerCmd["var"] = getThisParameter(fs);

    replace(fs,cls,fs->parent);
}
void Ficulator::solveName_outsideClasss(Cmd* fs,Cmd* current,Cmd* cls){
    //repush on class
    auto var = new Cmd(current->synSettings);
    var->file = current->file;
    var->line = current->line;
    var->parent = current;
    var->type="VAR";
    var->value = " "+current->value;
    var->markerCmd["repush"] = current->markerCmd["var"];
    cls->children.push_back(var);
        
    //this.name
    current->type="SUFFIX";
    current->value = " "+current->value;
    auto name = new Cmd(current->synSettings);
    name->file = current->file;
    name->line = current->line;
    name->parent = current;
    name->type="NAME";
    name->value = "this";
    current->children.push_back(name);
    name->markerCmd["var"] = getThisParameter(fs);
}
bool Ficulator::doesTheClassContainVar(Cmd* cls,Cmd* var,Cmd* current){
    if(current==nullptr)
        current = cls;
    for(auto& x:current->children){
        if(x==var)
            return true;
        if(x->marker["codeunit"]!="true")
            if(doesTheClassContainVar(cls,var,x))
                return true;
    }
    return false;
}
void Ficulator::solveName_class(Cmd* fs,Cmd* current,Cmd* cls){
    //this.name
    current->type="SUFFIX";
    auto name = new Cmd(current->synSettings);
    name->file = current->file;
    name->line = current->line;
    name->parent = current;
    name->type="NAME";
    name->value = "this";
    current->children.push_back(name);
    name->markerCmd["var"] = getThisParameter(fs);
}

Cmd* Ficulator::getThisParameter(Cmd* function){
    auto code = function->children[0];
    for(auto& x:code->children){
        if(x->type=="VAR"&&x->value=="this"&&x->marker["pseudo"]=="true"){
            return x;
        }
    }
    return nullptr;
}
void Ficulator::replace(Cmd*original,Cmd*replacement,Cmd*current){
    for(auto& x: current->children){
        if(x==original){
            x=replacement;
            return;
        }
        replace(original,replacement,x);
    }
}