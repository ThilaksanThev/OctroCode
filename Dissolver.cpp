#include "Dissolver.h"


Dissolver::Dissolver(Cmd* code)
{
	processExternals(code);
	processFunctions(code);
	processRegular(code);
}
/*
	OPERATOR
	PREFIX
	SUFFIX
	RETURN
*/
/*
	VAR
	NAME
*/
/*
	FUN
	CLASS
*/
/*
	IF
	WHILE
	CODE
*/
void Dissolver::processExternals(Cmd* code)
{
	for (auto x : code->children) {
		if (x->type == "VAR"&&x->marker.find("extern")!=x->marker.end()) {
			x->marker["id"] = std::to_string(functionid++);
		}
	}
	for (auto x : code->children)
		processExternals(x);
}

void Dissolver::processFunctions(Cmd* code)
{
	for (auto x : code->children) {
		if (x->type == "FUN"||(x->type=="VAR"
			&& x->marker.find("extern") == x->marker.end()
			&&x->marker.find("pseudo") == x->marker.end())) {
			x->marker["id"] = std::to_string(functionid++);
		}
	}
}

void Dissolver::processRegular(Cmd*& code)
{
	/*
		Gehe alles durch
		Ersetze alle name variables durch Stackpositions,oder this.sachen
	*/
	for (auto& x : code->children) {
		if (x->type == "FUN") {
			processRegularFunction(x);
		}
		processRegular(x);
		if (x->type == "NAME") {
			if (x->markerCmd.find("var") != x->markerCmd.end()) {
				auto var = x->markerCmd["var"];
				if (var->marker.find("id") != var->marker.end()) {
					x->type = "STACKFRONT";
					x->value = var->marker["id"];
				}
			}
		}
	}
}

void Dissolver::processRegularFunction(Cmd*& fun)
{
	size_t position = 0;
	auto code = fun->children[0];
	for (auto x : code->children) {
		processRegularFunctionRecursion(fun, position, x);
	}
}

void Dissolver::processRegularFunctionRecursion(Cmd*& fun, size_t& position, Cmd*& code)
{
	size_t startpos = position;
	for (auto x : code->children)
	{
		processRegularFunctionRecursion(fun, position, x);
	}
	/*
			auf der suche nach NAME in allem rekursiv

			Wenn man in Code rausgeht wird position zurückgesetzt
			bei if und while same hier
			bei classen same hier
		*/
	
	if (code->type == "VAR") {
		code->marker["pos"] = std::to_string(position++);
		code->markerCmd["space"] = fun;
	}
	if (code->type == "NAME") {
		if (code->markerCmd.find("var") != code->markerCmd.end()) {
			auto var = code->markerCmd["var"];
			if (var->marker.find("id") != var->marker.end()) {
				code->type = "STACKFRONT";
				code->value = var->marker["id"];
			}else if (var->marker.find("pos") != var->marker.end()) {
				code->type = "STACKBACK";
				code->value = std::to_string(position -1- std::stoi(var->marker["pos"]));
			}
		}
	}

	if (code->type == "OPERATOR" || code->type == "IF" || code->type == "WHILE" || code->type == "CLASS") {
		position = startpos;
	}
}
