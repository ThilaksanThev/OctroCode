#ifndef IMPORTER_H
#define IMPORTER_H
#include "Cmd.h"
#include <vector>
#include <map>
#include "SynSettings.h"

class Importer
{
    std::vector<std::pair<std::string,std::string>> stdlib;
    SynSettings* syn;
    bool again = false;
    public:
        Importer(std::vector<std::pair<std::string,std::string>> stdlib,SynSettings* syn);
        void loadCode(Cmd*);
    protected:
        void load(Cmd*);
        Cmd* getCode(std::string code);
        void library(Cmd*);
    private:


};

#endif // IMPORTER_H
