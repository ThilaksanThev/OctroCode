#include "Tokens.h"
#include <fstream>
#include <string>
#include "Algorithms.h"

Tokens::Tokens(size_t l)
{
	line = l;
}

Tokens::Tokens(Tokens & t)
{
	this->parent = parent;
	if (parent == nullptr)
		this->parent = t.parent;
	this->type = t.type;
	this->value = t.value;
	this->line = t.line;
	for (auto x : t.children)
	{
		Tokens* tt = new Tokens(*x);
		tt->parent = this;
		children.push_back(tt);

	}
}


Tokens::~Tokens()
{
	for (auto x : children)
		delete x;
}

void Tokens::loadFromFile(std::string path)
{
	std::fstream file(path, std::ios::in);
	std::string line, full;
	while (!file.eof())
	{
		std::getline(file,line);
		full += line + '\n';
	}
	file.close();
	loadFromString(full);
}

void Tokens::loadFromString(std::string code)
{
	size_t i = 0;
	loadFromString(code,i);
}
void Tokens::loadFromString(std::string& s,size_t& i)
{
	for (; i < s.size();)
		if (!loadWhite(s, i))
			if(!loadNumber(s, i))
				if(!loadLetter(s,i))
					if (!loadSpecial(s, i))
						if (!loadBracket(s, i))
							if (!loadString(s, i))
								if (Algorithms::contain(s[i], ")]}"))
								{
									i++;
									return;
								}


}
bool Tokens::loadNumber(std::string& s, size_t& i)
{
	if ('0' > s[i] || '9' < s[i])
		return false;
	std::string value = "";
	while(s.length()>i?'0' <= s[i] && '9' >= s[i]:0)
	{
		value += s[i];
		i++;
	}

	if(s.length() > i?(s[i]=='.')&& (s.length()>i+1?s[i+1]!='.':1):0)
	{
		value += s[i];
		i++;
		while (s.length() > i?'0' <= s[i] && '9' >= s[i]:0)
		{
			value += s[i];
			i++;
		}
	}
	Tokens* t = new Tokens(line);
	t->type = Tokens::Type::NUMBER;
	t->value = value;
	t->parent = this;
	children.push_back(t);
	return true;
}

bool Tokens::loadLetter(std::string& s, size_t& i)
{
	if (!('A' <= s[i] && 'Z' >= s[i] || 'a' <= s[i] && 'z' >= s[i]||'_'==s[i]))
		return false;
	std::string value = "";
	while (s.length() > i?'A' <= s[i] && 'Z' >= s[i] || 'a' <= s[i] && 'z' >= s[i]||s[i]=='_'||s[i]>='0'&&s[i]<='9':0)
	{
		value += s[i];
		i++;
	}

	Tokens* t = new Tokens(line);
	t->type = Tokens::Type::LETTER;
	t->value = value;
	t->parent = this;
	children.push_back(t);
	return true;
}

bool Tokens::loadSpecial(std::string& s, size_t& i)
{
	if (!Algorithms::contain(s[i], "<>|^°!§$%&/\\#+-*,:;.-~@?="))
		return false;
	if (s[i] == '/')
	{
		if (s.length() > i + 1)
		{
			if (s[i+1] == '/')
			{
				i++;
				while (s.length() > i ? s[i] != '\n' : 0)i++;
				{
					line++;
					return true;
				}
			}
			else if (s[i+1] == '*')
			{
				i+=2;
				while (s.length() > i + 1 ? s[i] != '*' || s[i + 1] != '/' : 0)
				{
					if (s[i] == '\n')
						line++;
					i++;
				}
				i+=2;
				return true;
			}
		}
	}
	std::string value = "";
	value = s[i];
	i++;
	Tokens* t = new Tokens(line);
	t->type = Tokens::Type::SPECIAL;
	t->value = value;
	t->parent = this;
	children.push_back(t);
	return true;
}

bool Tokens::loadBracket(std::string & s, size_t & i)
{
	if (!Algorithms::contain(s[i], "{(["))
		return false;
	char ch = s[i];
	i++;

	Tokens* t = new Tokens(line);
	t->type = Tokens::Type::BRACKET;
	t->value = ch;
	t->parent = this;
	t->loadFromString(s, i);
	children.push_back(t);
	line = t->line;
	return true;
}

bool Tokens::loadString(std::string& s, size_t& i)
{
	if (!Algorithms::contain(s[i],"\"\'"))
		return false;
	std::string value = "";
	i++;
	bool escape = false;
	while (s.size()>i?!Algorithms::contain(s[i], "\"\'")||escape:0)
	{
		if (s[i] == '\n')
			line++;
		if (!escape)
		{
			if(s[i] == '\\')
				escape = true;
			else value += s[i];
		}
		else
		{
			if (s[i] == 'a')
				value += "\a";
			else if (s[i] == 't')
				value += "\t";
			else if (s[i] == 'n')
				value += "\n";
			else if (s[i] == 'x')
			{
				i++;
				std::string hex;
				while ((s.size() > i&& hex.size() < 2)? Algorithms::contain(s[i], "0123456789ABCDEFabcdef") : 0)
				{
					hex += s[i];
					i++;
				}
				i--;
				value += (unsigned char)Algorithms::toHex(hex);
			}
			else
				value += s[i];
			escape = false;
		}
		i++;
	}
	i++;

	Tokens* t = new Tokens(line);
	t->type = Tokens::Type::STRING;
	t->value = value;
	t->parent = this;
	children.push_back(t);
	return true;
}

bool Tokens::loadWhite(std::string & s, size_t & i)
{
	if (!Algorithms::contain(s[i], " \t\n\r"))
		return false;
	if (s[i] == '\n')
	{
		line++;
	}
	i++;
	return true;
}

std::string Tokens::information(std::string s)
{
	std::string rv = s;
	if (type == Tokens::Type::NUMBER)
		rv+="NUMBER ";
	if (type == Tokens::Type::LETTER)
		rv+="LETTER ";
	if (type == Tokens::Type::SPECIAL)
		rv += "SPECIAL ";
	if (type == Tokens::Type::STRING)
		rv += "STRING ";
	if (type == Tokens::Type::BRACKET)
		rv += "BRACKET ";
	rv += value+"\n";

	for (auto x : children)
		rv+=x->information(s+"\t");
	return rv;
}


