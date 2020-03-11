#include "Log.h"
void Log::error(std::string origin,std::string message,Cmd* c){
    std::cout << "\a<!>[Octro]Compiler error in line:"<<c->line<<" file:"<<c->file<<":\n\t<!>"<<origin<<":"<<message<<std::endl;
}
//void Log::info(std::string origin,std::string message,int line){

//}
