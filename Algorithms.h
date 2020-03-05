#ifndef ALGORITHMS
#define ALGORITHMS
#include <string>
#include <vector>
#include <math.h>
class Algorithms
{
public:
	Algorithms();
	~Algorithms();

	static bool contain(char str,std::string keys);
	static unsigned int toHex(std::string s);
	
	static unsigned int faculty(unsigned int);
	static std::string increaseName(std::string);
	static bool contain(std::string&, std::vector<std::string>&);
	static double modulo(double a, double b);
};
#endif // ALGORITHMS
