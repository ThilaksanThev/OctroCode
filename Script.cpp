#include "Script.h"
#include "Cmd.h"
#include "Tokens.h"
#include "SynSettings.h"

#include <fstream>

#include "Deref.h"
#include "Ficulator.h"
#include "Importer.h"
#include "Writer.h"
#include "Log.h"

Script::Script(){}
Script::~Script(){}
void Script::loadFromFile(std::string file)
{
	SynSettings ss;
	ss.setOrderOfOperation({
		{true,{".."}},
		{true,{"^"}},
		{true,{"*","/","%"} },
		{true,{"+","-"}},
		{true,{"<<",">>"}},
		{true,{"<","<=",">",">="}},
		{true,{"==","!="}},
		{true,{"&&"}},
		{true,{"||"}},
		{false,{":"}},
		{false,{"?"}},
		{true,{"->"}},
		{false,{"=","*=","-=","+=","/=","%=","<<=",">>="}},
		{true,{","}},
		{true,{";"}}
		});

	//Priority
	Cmd* dummy = new Cmd(&ss);//for prio error msg
	dummy->file = file;
	std::fstream f(file,std::ios::in);
	std::vector<Cmd*> codes;
	if(!f.is_open()){
		Log::error("PRI L0","couldn't open prio file : "+file,dummy);
	}
	while (!f.eof())
	{
		std::string line =  "";
		std::getline(f,line);
		Cmd* code = new Cmd(&ss);
		code->file = line;
		Tokens t(0);
		if(!t.loadFromFile(line)){
			Log::error("PRI L1","couldn't load from file "+line,dummy);
		}
		code->loadCode(&t);
		codes.push_back(code);
		dummy->line++;
	}
	delete dummy;

	//Importer
	Importer importer({{"stdlib","lib stdlib\"1.0\"1{extern print;}"}},&ss);
	Cmd* code = importer.loadCode(codes);
	codes.clear();
	Deref deref({
		{"CODE",{"this","{*}"}},
		{"OPERATION",{"","*"}},
		{"OPERATOR",{"","*"}},
		{"SUFFIX",{"","*"}},
		{"PREFIX",{"","*"}},
		{"FUN",{"0",""}},
		{"RETURN",{"","0"}},
		{"IF",{"1,2","{0[1 2]}"}},
		{"WHILE",{"1","{0{1}}"}},
		{"CLASS",{"this",""}}
	});
	deref.markCodeUnits(code);
	deref.extractFunctions(code,code);
	std::vector<Cmd*> vars;
	deref.resolve(code,vars);
	Deref::varSet(code);
	Deref::initErase(code);
	Ficulator ficulator(code);
	code->print("");
	getchar();
	Writer writer(code);
	writer.write("test.o");
	system("./OctroWriter.out");
	system("./OctroExecuter.out");
    return;
}

/*
#add string bool isWstring = false
#add <class> <type> <name> ?default setting

#add string bool isWstring(parameter){
	asdasd
}
#add string bool isWstring(parameter){
	asdasd
}


	Optimizer:
		if else <- resolve if the condition is const
		function inlining

	class adding new members
	inheritage

	syn.addKeyword(new Keyword("var $name"));
	syn.addKeyword(new Keyword("fun #'('{~}%{$name}[^param$parameter#\"=\"{~~/stdoperation}-|param]/code"));
	syn.addKeyword(new Keyword("return /value"));
	syn.addKeyword(new Keyword("true"));
	syn.addKeyword(new Keyword("false"));
	syn.addKeyword(new Keyword("if [/operation] /cmd &else{/else}"));
	syn.addKeyword(new Keyword("class$name#':'{$inheritage^ipos#','{$inheritage|ipos}}/cmd"));

	syn.addKeyword(new Keyword("meth #'('{~}%{$name}#'('{~}%{$name}#'('{~}%{$name}#'('{~}%{$name}[^param$parameter#\"=\"{~~/stdoperation}-|param]/code"));


	syn.addKeyword(new Keyword("for [/operation~$type^start/operation|start]/cmd"));
	syn.addKeyword(new Keyword("while [/operation] /cmd"));
	syn.addKeyword(new Keyword("do /cmd &while{/operation}"));
	syn.addKeyword(new Keyword("repeat[/operation]/cmd"));
	syn.addKeyword(new Keyword("enum$name[^param$parameter-|param]"));
	syn.addKeyword(new Keyword("switch[/variable]/code"));
	syn.addKeyword(new Keyword("case[/variable]/code"));
	syn.addKeyword(new Keyword("default/code"));


	syn.define("elif","else if");
*/
