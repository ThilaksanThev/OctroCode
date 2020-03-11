#pragma once
#include "Cmd.h"
#include <vector>

struct Library{
    std::string name;
    std::string version;
    std::string size;
    std::vector<Cmd*> externals;
};
class Writer
{
    std::vector<std::string> strings;
    std::vector<Library> libs;
    
    size_t stackposition;//front
    std::string output;
public:
    Writer(Cmd*);
    ~Writer() {}

    //collect
    void loadStrings(Cmd*);
    void loadLibraries(Cmd*);
    void loadExternals(Cmd*);//Die externals den libraries zuordnen
    void initExternals();

    //write
    void write(std::string path);
    std::string escape(std::string);

    std::string write(Cmd* code);
    std::string writeVar(Cmd*);
    std::string writeNumber(Cmd*);
    std::string writeString(Cmd*);

    std::string writePop(Cmd*);
    std::string writeOperator(Cmd*);
    std::string writeClass(Cmd*);
    std::string writeName(Cmd*);
    std::string writePrefix(Cmd*);
    std::string writeSuffix(Cmd*);

};