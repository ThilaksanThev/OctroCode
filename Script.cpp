#include "Script.h"
#include "Tokens.h"
#include "Cmd.h"
#include "SynSettings.h"

#include <fstream>
#include "Deref.h"
#include "Ficulator.h"
#include "Dissolver.h"
#include "Cleaner.h"
#include "Importer.h"
#include "Writer.h"

Script::Script()
{
	int a =0,b=1,c = 2;

}


Script::~Script()
{
}
void Script::loadFromFile(std::string file)
{

	Tokens t(0);
	t.loadFromFile(file);
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
	std::cout << "CMD\n\n\n" << std::endl;

	Cmd code(&ss);
	code.loadCode(&t);
	Importer importer({{"stdlib","lib stdlib\"1.0\"1{extern print;}"}},&ss);
	importer.loadCode(&code);
	Deref deref(&code);
	Ficulator ficulator(&code);
	Cleaner cleaner(&code);
	//code.print("");
	Writer writer(&code);
	writer.write("test.o");
    return;
	getchar();
	system("clear");

    /*Dissolver dis(&code);
	code.print("");*/

	//startup("OctroExecuter.exe");

	while (1);

	getchar();

	return;
	std::cout << "\n\n\n\n\n\n" << std::endl;

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
