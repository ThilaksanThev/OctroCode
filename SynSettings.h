#ifndef SYNSETTINGS
#define SYNSETTINGS
#include <vector>
#include <string>
#include "Cmd.h"

class SynSettings
{
	std::vector<std::string> operators;
	std::vector<std::pair<bool,std::vector<std::string>>> orderOfOperation;

public:

	SynSettings();
	~SynSettings();

	void setOrderOfOperation(std::vector<std::pair<bool,std::vector<std::string>>> o);
	size_t operationLevelCount();
	std::vector<std::string>& getOperators();
	std::vector<std::string>& getOperators(size_t);
	bool& getOperatorDirection(size_t);
};
#endif // SYNSETTINGS
