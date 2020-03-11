#include <iostream>
#include "Cmd.h"

class Log
{
private:
    /* data */
public:
    Log(/* args */) {}
    ~Log() {}

    

    static void error(std::string origin,std::string message,Cmd*);
//    static void info(std::string origin,std::string message,int line);
};