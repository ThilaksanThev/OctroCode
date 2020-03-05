#include "Algorithms.h"



Algorithms::Algorithms()
{
}


Algorithms::~Algorithms()
{
}

bool Algorithms::contain(char str, std::string keys)
{
	for (auto var : keys)
	{
		if (var == str)
			return true;
	}
	return false;
}

unsigned int Algorithms::toHex(std::string s)
{
	unsigned int rv = 0;
	for (size_t i = 0; i < s.length(); i++)
	{
		unsigned char f = 0;
		if(Algorithms::contain(s[i],"0123456789"))
			f = s[i]-'0';
		else if (Algorithms::contain(s[i], "ABCDEF"))
		{
			f = (s[i] - 'A') + 10;
		}
		else if (Algorithms::contain(s[i], "abcdef"))
		{
			f = (s[i] - 'a') + 10;
		}
		rv += f * pow(16,s.length()-i-1);
	}
	return rv;
}

unsigned int Algorithms::faculty(unsigned int x)
{
	return x?x*faculty(x-1):1;
}

std::string Algorithms::increaseName(std::string s)
{
	std::string rv;
	int i=s.length() - 1; 
	while (i>=0?
		s[i] >= '0'&&s[i] <= '9':0)
		i--;
	i++;

	int count = 2;
	if (i != s.length())
	{
		count = std::stoi(s.substr(i, s.length() - i));
		count++;
	}

	rv = s.substr(0, i)+std::to_string(count);
	
	return rv;
}

bool Algorithms::contain(std::string & s, std::vector<std::string>& v)
{
	for (auto a : v)
		if (a == s)
			return true;
	return false;
}

double Algorithms::modulo(double a, double b)
{
	return a-b*((int)a/b);
}
