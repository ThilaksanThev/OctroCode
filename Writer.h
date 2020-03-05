#pragma once
#include "Cmd.h"

struct Library{
    std::string name;
    std::string version;
    std::string size;
};
class Writer
{
    std::vector<std::string> strings;
    std::vector<Library> libs;
    
public:
    Writer(Cmd*);
    ~Writer() {}

    //collect
    void loadStrings(Cmd*);
    void loadLibraries(Cmd*);


    //write
    void write(std::string path);
    std::string escape(std::string);
};