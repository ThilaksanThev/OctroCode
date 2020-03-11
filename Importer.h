#pragma once
#include "Cmd.h"
#include <vector>
#include <map>
#include "SynSettings.h"

class Importer
{
    std::vector<std::pair<std::string,std::string>> stdlib;
    SynSettings* syn;
    public:
        Importer(std::vector<std::pair<std::string,std::string>> stdlib,SynSettings* syn);
        Cmd* loadCode(std::vector<Cmd*> codes);
    protected:
        void load(Cmd*);
        void library(Cmd*);
};