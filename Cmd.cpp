#include "Cmd.h"
#include "Algorithms.h"
#include "SynSettings.h"

#define settings ((SynSettings*)synSettings)

#define get(x) s->children.size()<=x?0:s->children[x]
#define value(x) (s->children.size()<=x?"":s->children[x]->value)
#define isWord(x) s->children.size()<=x?0:(s->children[x]->type==Tokens::LETTER)
#define isSpecial(x) s->children.size()<=x?0:(s->children[x]->type==Tokens::SPECIAL)
#define isNumber(x) s->children.size()<=x?0:(s->children[x]->type==Tokens::NUMBER)
#define isString(x) s->children.size()<=x?0:(s->children[x]->type==Tokens::Type::STRING)
#define isBracket(x) s->children.size()<=x?0:(s->children[x]->type==Tokens::BRACKET)

#define namespacesize settings->stack["name"].size();
#define namespacepop(count)\
	while (count < settings->stack["name"].size())\
	settings->stack["name"].pop_back();

Cmd::Cmd(void* ss)
{
	synSettings = ss;
}


Cmd::~Cmd()
{
	for (auto x : children)
		delete x;
}


/*void Cmd::loadOperator_Optimize()
{
	return;
	std::string& a = children[0]->marker["class"];
	std::string& b = children[1]->marker["class"];

	if (value == "?")
	{

		std::cout << std::endl;
	}
	if (a == "")
	{
		if (children[0]->type == "VAR")
		{

			marker["class"] = b;
			type = children[0]->type;
			value = children[0]->value;
			setValue(children[1]->getValue());
			settings->stack["name"].push_back(this);
			for (auto x : children)
				delete x;
			children.clear();
			return;
		}
	}
	if (a == "Number")
	{
		if (b == "Number")
		{
			type = "NUMBER";
			marker["class"] = "Number";

			if(value=="+")
				setValue( std::to_string(std::stod(children[0]->getValue())+ std::stod(children[1]->getValue())));
			else if (value == "-")
				setValue( std::to_string(std::stod(children[0]->getValue()) - std::stod(children[1]->getValue())));
			else if (value == "/")
				setValue( std::to_string(std::stod(children[0]->getValue()) / std::stod(children[1]->getValue())));
			else if (value == "*")
				setValue( std::to_string(std::stod(children[0]->getValue()) * std::stod(children[1]->getValue())));
			else if (value == "%")
				setValue( std::to_string(Algorithms::modulo(std::stod(children[0]->getValue()), std::stod(children[1]->getValue()))));
			else if (value == "^")
				setValue( std::to_string(pow(std::stod(children[0]->getValue()) , std::stod(children[1]->getValue()))));
			else if (value == "*")
				setValue( std::to_string(std::stod(children[0]->getValue()) * std::stod(children[1]->getValue())));
			else if (value == "<")
				setValue( std::to_string(std::stod(children[0]->getValue()) < std::stod(children[1]->getValue())));
			else if (value == "<=")
				setValue( std::to_string(std::stod(children[0]->getValue()) <= std::stod(children[1]->getValue())));
			else if (value == ">")
				setValue( std::to_string(std::stod(children[0]->getValue()) > std::stod(children[1]->getValue())));
			else if (value == ">=")
				setValue( std::to_string(std::stod(children[0]->getValue()) >= std::stod(children[1]->getValue())));
			else if (value == "==")
				setValue( std::to_string(std::stod(children[0]->getValue()) == std::stod(children[1]->getValue())));
			else if (value == "!=")
				setValue( std::to_string(std::stod(children[0]->getValue()) != std::stod(children[1]->getValue())));
			else if (value == "&&")
				setValue( std::to_string(std::stod(children[0]->getValue()) && std::stod(children[1]->getValue())));
			else if (value == "||")
				setValue( std::to_string(std::stod(children[0]->getValue()) || std::stod(children[1]->getValue())));
			else return;

			delete children[0];
			delete children[1];
			children.clear();
			return;
		}
	}

	return;
}
*/
bool Cmd::loadOperator(Tokens * s, size_t & i)
{
	size_t start = i;
	for (auto o: settings->getOperators())
	{
		std::string op = "";
		while(isSpecial(i)&&op.size()<o.size())
		{
			op += value(i);
			if (op == o)
			{
				type = "OPERATOR";
				value = op;
				i++;
				return true;
			}
			i++;
		}
		i = start;
	}
	i = start;
	return false;
}


bool Cmd::loadClassCode(Tokens * s, size_t & i)
{
	enum AccessType {
		PUBLIC,
		PRIVATE,
		PROTECTED
	};
	AccessType at = PROTECTED;
	while(i < s->children.size())
	{
		size_t start = i;
		bool found = false;
		if (isWord(i))
		{
			if (value(i) == "public"&&!found)
			{
				i++;
				if (isSpecial(i) ? value(i) == ":" : 0)
				{
					i++;
					at = PUBLIC;
					found = true;

					auto c = new Cmd(synSettings);
					c->type = "PUBLIC";
					children.push_back(c);
				}
				else
					i = start;
			}
			if (value(i) == "private" && !found)
			{
				i++;
				if (isSpecial(i) ? value(i) == ":" : 0)
				{
					i++;
					at = PRIVATE;
					found = true;
					auto c = new Cmd(synSettings);
					c->type = "PRIVATE";
					children.push_back(c);
				}
				else
					i = start;
			}
			if (value(i) == "protected" && !found)
			{
				i++;
				if (isSpecial(i) ? value(i) == ":" : 0)
				{
					i++;
					at = PROTECTED;
					found = true;
					auto c = new Cmd(synSettings);
					c->type = "PROTECTED";
					children.push_back(c);
				}
				else
					i = start;
			}
		}
		if (!found)
		{
			auto c = new Cmd(synSettings);
			c->loadOperation(s, i,c);
			children.push_back(c);
		}
	}
	return true;
}

bool Cmd::loadKeyword(Tokens * s, size_t & i)
{
	size_t start = i;
	if (value(i) == "var")
	{
		i++;
		if (isWord(i))
		{
			type = "VAR";
			value = value(i);
			i++;
			return true;
		}
	}
	else if (value(i) == "import")
	{
        i++;
		if (isString(i))
		{
            type = "IMPORT";
			value = value(i);
			i++;
			return true;
		}
	}
	else if (value(i) == "lib")
	{
        i++;

            type = "LIBRARY";
			value = value(i);
			i++;
			marker["version"] = value(i);
			i++;
            marker["size"] = value(i);
			i++;
            //load code
            Cmd* c = new Cmd(synSettings);
            c->loadCode(s->children[i]);
            children.push_back(c);

			i++;
			return true;

	}
	else if (value(i) == "fun")
	{
		i++;
		type = "FUN";
		if (isWord(i))
		{
			value = value(i);
			i++;
		}
		if (isBracket(i) && value(i) == "(")
		{
			auto t = s;
			s = s->children[i];
			//going into the children of this token

			size_t e = 0;
			do {
				if (isSpecial(e) && value(e) == ",") {
					e++;
				}
				if (isWord(e))
				{
					parameter.push_back(value(e));
					e++;
				}
			} while (isSpecial(e) && value(e) == ",");



			//going out of the children of this token
			s = t;
			i++;
		}

		Cmd* c = new Cmd(synSettings);
		c->loadOperation(s,i,c);
		children.push_back(c);

		return true;

	}
	else if (value(i) == "return")
	{
		i++;
		type = "RETURN";
		Cmd* c= new Cmd(synSettings);
		c->loadOperation(s, i,c);
		children.push_back(c);


		return true;
	}
	else if (value(i) == "true"||value(i)=="false")
	{
		type = "NUMBER";
		value = value(i)=="true"?"1":"0";
		i++;
		marker["class"] = "Number";
		return true;
	}
	else if (value(i) == "if"||value(i)=="while")
	{
		type = (value(i) == "if") ? "IF" : "WHILE";
		i++;
		//condition
		Cmd* cmd = new Cmd(synSettings);
		cmd->loadMember(s,i,cmd);
		children.push_back(cmd);


		//code
		cmd = new Cmd(synSettings);
		cmd->loadOperation(s,i,cmd);
		children.push_back(cmd);

		if(type=="if")
		if (isWord(i) && value(i) == "else") {
			i++;
			//code
			cmd = new Cmd(synSettings);
			cmd->loadOperation(s, i,cmd);
			children.push_back(cmd);
		}
		return true;

	}
	else if (value(i) == "class")
	{
		i++;
		type = "CLASS";
		if (isWord(i))
		{
			value = value(i);
			i++;
		}
		size_t e = 0;
		loadClassCode(s->children[i],e);
		i++;
		return true;
	}
	else if (value(i) == "extern")
	{
		i++;
		if (isWord(i))
		{
			type = "VAR";
			value = value(i);
			marker["extern"] = "true";
			i++;
			return true;
		}
	}
	i = start;
	return false;
}

bool Cmd::loadMember(Tokens * s, size_t & i,Cmd*& c)
{
	size_t start = i;
	if (isWord(i))
	{
		if (loadKeyword(s, i))
		{
			return true;
		}

		type = "NAME";
		value = value(i);
		i++;
		return true;
	}
	if (isNumber(i))
	{
		type = "NUMBER";
		value = value(i);
		setValue(value(i));
		i++;
		marker["class"] = "Number";
		return true;
	}
	if (isString(i))
	{
		type = "STRING";
		value = value(i);
		setValue(value(i));
		i++;
		marker["class"] = "String";
		return true;
	}
	if (isBracket(i)&&value(i)=="{")
	{
		loadCode(s->children[i]);

		i++;
		marker["class"] = "Code";
		return true;
	}
	if (isBracket(i) && value(i) == "(")
	{
		size_t e = 0;
		loadOperation(s->children[i],e,c);
		i++;
		return true;
	}
	i = start;
	return false;
}
bool Cmd::loadPrefix(Tokens * s, size_t & i, Cmd *&c)
{
	/*
		++
		--
		+
		-
		!
		~
		*
		&

	*/

	if (isSpecial(i))
	{
		if (value(i) == "+")
		{
			i++;
			value = "+";
			if (isSpecial(i)?value(i) == "+":0)
			{
				i++;
				value = "++";

			}
		}
		else if (value(i) == "-")
		{
			i++;
			value = "-";

			if (isSpecial(i)?value(i) == "-":0)
			{
				i++;
				value = "--";

			}
		}
		else if (value(i) == "!")
		{
			i++;
			value = "!";
		}
		else if (value(i) == "~")
		{
			i++;
			value = "~";

		}
		else if (value(i) == "*")
		{
			i++;
			value = "*";

		}
		else if (value(i) == "&")
		{
			i++;
			value = "&";

		}
		else
		{
			//gibt es einen member?
			auto member = new Cmd(synSettings);
			if (!member->loadMember(s, i, member)) {
				delete member;
				return false;
			}
			c->children.push_back(member);
			return loadSuffix(s, i, c);
		}
		type = "PREFIX";
		Cmd* cmd = new Cmd(synSettings);
		cmd->loadPrefix(s, i, cmd);
		children.push_back(cmd);
		return true;
	}
	//gibt es einen member?
	auto member = new Cmd(synSettings);
	if (!member->loadMember(s, i, member)) {
		delete member;
		return false;
	}
	c->children.push_back(member);
	return loadSuffix(s, i, c);
}
bool Cmd::loadSuffix(Tokens * s, size_t & i, Cmd *& c)
{
	bool suffix = false;
	if (isSpecial(i))
	{
		if (value(i) == "+")
		{
			i++;
			if (isSpecial(i) ? value(i) == "+" : 0)
			{
				i++;
				value = "++";
				suffix = true;
			}
			else i--;
		}
		else if (value(i) == "-")
		{
			i++;
			if (isSpecial(i) ? value(i) == "+" : 0)
			{
				i++;
				value = "--";
				suffix = true;

			}
			else i--;
		}
		else if (value(i) == ".")
		{
			i++;
			value = value(i);
			suffix = true;
			i++;
		}
	}
	else if (isBracket(i))
	{
		value = value(i);
		suffix = true;
		if (value(i) == "(")
		{
			size_t e = 0;
			//parameter;
			if (s->children[i]->children.size()) {
				Cmd* cd = new Cmd(synSettings);
				cd->loadOperation(s->children[i], e, cd);
				children.push_back(cd);
			}
		}
		i++;
	}

	if (suffix)
	{
		type = "SUFFIX";
		//rekursiv suffix,schiebe object zum unteren suffix
		Cmd* cmd = new Cmd(synSettings);
		cmd->children.push_back(this);
		cmd->loadSuffix(s, i, cmd);
		c = cmd;
		return true;
	}
	//es gibt keinen suffix ,nehme das erste object (den member) und ersetzte ihn mit mir
	auto cmd = c->children[0];
	c->children.clear();
	c = cmd;
	delete this;
	return true;
}
bool Cmd::loadOperation_Order( size_t lvl)
{
	std::vector<std::string>& x = settings->getOperators(lvl);
	if (settings->getOperatorDirection(lvl))
	{
		for (size_t i = 1; i < children.size();)
		{
			if (Algorithms::contain(children[i]->value, x))
			{
				children[i]->children.push_back(children[i - 1]);
				children[i]->children.push_back(children[i + 1]);

				//children[i]->loadOperator_Optimize();

				children.erase(children.begin() + i + 1);
				children.erase(children.begin() + i - 1);
			}
			else i += 2;
		}
	}
	else
	{
		for (int i = children.size() - 2; i >= 0;)
		{
			if (Algorithms::contain(children[i]->value, x))
			{
				children[i]->children.push_back(children[i - 1]);
				children[i]->children.push_back(children[i + 1]);


				//children[i]->loadOperator_Optimize();

				children.erase(children.begin() + i + 1);
				children.erase(children.begin() + i - 1);
			}
			i -= 2;
		}
	}
	return false;
}
bool Cmd::loadOperation_Gathering(Tokens * s, size_t & i)
{
	if (s->children.size() <= i)//End of Brace
		return false;
	while (1) {
		if (s->children.size() <= i)
		{
			//expecting Member
			return false;
		}
		if (isSpecial(i) ? value(i) == ";" : 0)//End of Command
		{

			//ERROR: Expect Member
			i++;
			return false;
		}
		Cmd* c = new Cmd(synSettings);
		if (!c->loadPrefix(s, i,c))
		{
			//unknown object
			delete c;
			return false;
		}
		children.push_back(c);

		if (s->children.size() <= i)//End of Brace
		{
			return true;
		}
		if (isSpecial(i) ? value(i) == ";" : 0)//End of Command
		{
			i++;
			return true;
		}
		c = new Cmd(synSettings);
		if (!c->loadOperator(s, i))
		{
			delete c;
			return true;
		}
		children.push_back(c);
	}
}
bool Cmd::loadOperation(Tokens * s, size_t & i,Cmd*& c)
{
	if (loadOperation_Gathering(s, i)) {
		type = "OPERATION";
		for (size_t e = 0; e < settings->operationLevelCount(); e++)
		{
			loadOperation_Order(e);
		}
		if (children.size()) {
			c = children[0];

			bool first = true;
			for (auto x : children)
				if(!first)
					delete x;
				else first = false;
			children.clear();
			delete this;
			return true;
		}
		return true;
	}
	return false;
}


void Cmd::loadCode(Tokens * s)
{
	type = "CODE";
	for (size_t i = 0; i < s->children.size();)
	{
		Cmd* c = new Cmd(synSettings);
		if (c->loadOperation(s, i,c)) {

			children.push_back(c);
		}
		else {
			delete c; getchar(); exit(0); }
	}
	initParent();
}

std::string Cmd::getValue()
{
	return marker["value"];
}

void Cmd::setValue(std::string s)
{
	marker["value"] = s;
}


void Cmd::print(std::string tab)
{
	std::cout << tab << type << std::endl;
	if(value.size())
		std::cout <<"\t"<< tab <<"value: "<< value << std::endl;
	for (auto x : parameter)
		std::cout << "\t" << tab <<"param: "<< x << std::endl;
	for (auto x : marker)
		std::cout << "\t" << tab << "mark: " << x.first<<"="<<x.second << std::endl;
	for (auto x : markerCmd)
		std::cout << "\t" << tab << "mark: " << x.first << "=" << x.second->type<<":"<<x.second->value << std::endl;

	for (auto x : children)
		x->print(tab + "\t");
}

void Cmd::initParent()
{
	for (auto x : children) {
		x->parent = this;
		x->initParent();
	}
}


