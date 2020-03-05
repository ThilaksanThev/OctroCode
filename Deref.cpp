#include "Deref.h"

Deref::Deref(Cmd* code,Deref* parent)
{
	this->parent = parent;
	
	processFun(code);
	processCLS(code);

	

	A_processCode(code);
	B_processCode(code);

	/*for (auto x : functions)
		x->print("");
	for (auto x : variables)
		x->print("");*/
	allfunctions.insert(allfunctions.end(), functions.begin(), functions.end());
	if (parent)
		parent->allfunctions.insert(
			parent->allfunctions.end(),
			allfunctions.begin(),
			allfunctions.end());
	else {
		for (auto x : allfunctions)
			code->children.insert(code->children.begin(),x);
	}
}


void Deref::A_processCmd(Cmd*& cmd)
{
	if (cmd->type == "OPERATOR"
		|| cmd->type == "SUFFIX"
		|| cmd->type == "PREFIX"
		|| cmd->type == "RETURN") {
		for (auto x : cmd->children)
			A_processCmd(x);
	}
	if (cmd->type == "FUN") {
		std::cout << cmd->value << std::endl;
		functions.push_back(cmd);
	}
}
void Deref::A_processCode(Cmd*& code)
{
	for (auto x : code->children)
	{
		A_processCmd(x);
	}
}

void Deref::B_processCode(Cmd*& code)
{
	for (size_t i = 0; i < code->children.size(); i++)
	{
		if (code->children[i]->type == "VAR"
			&& code->children[i]->marker.find("extern") != code->children[i]->marker.end())
			variables.push_back(code->children[i]);
		if (code->children[i]->type == "VAR"
			&&code->children[i]->marker.find("pseudo")==code->children[i]->marker.end()
			&&code->children[i]->marker.find("extern") == code->children[i]->marker.end())
			std::cout << "ERROR0" << std::endl;
		auto s = B_processCmd(code->children[i]);
		for (auto x : s) {
			code->children.insert(code->children.begin() + i,
				x);
			i++;
			x->parent = code;
		}
		
	}
}
std::vector<Cmd*> Deref::B_processCmd(Cmd*& cmd)
{
	if (cmd->type == "VAR"&&cmd->marker.find("extern")!=cmd->marker.end()) {
		auto c = cmd;
		variables.push_back(c);
		cmd = new Cmd(nullptr);
		cmd->type = "NAME";
		cmd->markerCmd["var"] = c;
		return {c};
	}
	/*
		OPERATOR
		PREFIX
		SUFFIX
		RETURN
	*/
	std::vector<Cmd*> rv;
	if (cmd->type == "OPERATOR"
		|| cmd->type == "SUFFIX"
		|| cmd->type == "PREFIX"
		|| cmd->type == "RETURN") {
		for (auto& x : cmd->children) {
			auto d = B_processCmd(x);
			for (auto x : d)
				rv.push_back(x);
		}
		//erstes element ein VAR?
		if (cmd->children[0]->type == "VAR") {
			if (cmd->type == "OPERATOR"&&cmd->value=="=")
			{
				cmd->children[0]->children.push_back(cmd->children[1]);
				auto var = cmd->children[0];
				cmd->children.clear();
				
				cmd->type = "NAME";
				cmd->markerCmd["var"] = var;
				rv.push_back(var);
				variables.push_back(var);
			}
			else {
				std::cout << "ERROR1" << std::endl;
			}
		
		}	
		else if (cmd->children[1]->type == "VAR")
			std::cout << "ERROR2" << std::endl;

	}
	/*
		VAR
		NAME
	*/
	else if (cmd->type == "NAME") {
		auto a = search(cmd->value);
		if (a)
			cmd->markerCmd["var"] = a;
		else
		{
			cmd->type = "VAR";
		}
	}

	/*
	IF
	WHILE
	CODE
	*/
	else if (cmd->type == "IF")
	{
		//condition
		auto d = B_processCmd(cmd->children[0]);
		for (auto x : d)
			rv.push_back(x);
		//code
		Deref deref(cmd->children[1],this);
		//else
		if(cmd->children.size()>1)
			Deref deref2(cmd->children[2],this);
	}
	else if (cmd->type == "WHILE")
	{
		//condition
		Deref(cmd->children[0],this);
		
		//code
		Deref(cmd->children[1], this);
	}
	else if(cmd->type=="CODE")
		Deref deref(cmd,this);

	/*
		FUN
		CLASS
	*/
	else if (cmd->type == "FUN") {
		Deref deref(cmd,this);
		auto var = cmd;
		cmd = new Cmd(nullptr);
		cmd->type = "INIT";
		cmd->markerCmd["var"] = var;
		var->markerCmd["init"] = cmd;
	}
	else if (cmd->type == "CLASS") {
		variables.push_back(cmd);
		Deref deref(cmd,this);
		auto var = cmd;
		cmd = new Cmd(nullptr);
		cmd->type = "NAME";
		cmd->children.push_back(var);
	}



	return rv;
}

Cmd* Deref::search(std::string name)
{
	for (auto& i : functions)
	{
		if(i->value==name)
			return i;
	}
	for (int i = variables.size() - 1; i >= 0; i--)
	{
		if (variables[i]->value == name)
			return variables[i];
	}
	if (parent)
		return parent->search(name);
	return nullptr;
}

void Deref::processFun(Cmd*& code)
{
	if (code->type != "FUN")
		return;
	std::vector<Cmd*> c;
	for (auto x : code->parameter) {
		Cmd* a = new Cmd(nullptr);
		a->type = "VAR";
		a->value = x;
		c.push_back(a);
		a->marker["pseudo"] = "true";
		variables.push_back(a);
	}
	code = code->children[0];
	for (size_t i = 0; i < c.size(); i++)
	{
		code->children.insert(code->children.begin()+i,
			c[i]);
	}
	code->parameter.clear();
}

void Deref::processCLS(Cmd*& code)
{
	if (code->type != "CLASS")
		return;
	for (auto x : code->children) {
		if (x->type == "FUN") {
			x->parameter.insert(x->parameter.begin(), "this");
		}
	}
}


