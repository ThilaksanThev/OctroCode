#include "Importer.h"
#include "Log.h"

Importer::Importer(std::vector<std::pair<std::string,std::string>> stdlib,SynSettings* syn)
{
    for(auto& x:stdlib)
        this->stdlib.push_back(x);
    this->syn = syn;
}
Cmd* Importer::loadCode(std::vector<Cmd*> codes){
    for(auto& code : codes){

        load(code);
        library(code);
    }
    //alles zusammenklatschen in eine Datei
    Cmd* returnvalue = new Cmd(syn);
    returnvalue->type = "CODE";
    for (auto code:codes)
    {
        for (auto x :code->children)
        {
            returnvalue->children.push_back(x);
        }
        code->children.clear();
        delete code;
    }
    codes.clear();
    return returnvalue;
}
void Importer::load(Cmd* code){
    for(int i=0;i<code->children.size();i++){
        if(code->children[i]->type=="IMPORT"){
            std::string path = code->children[i]->value;
            Tokens t(0);
            bool find = false;
            for(auto& x:stdlib)
            {
                if(x.first==path)
                {
                    t.loadFromString(x.second);
                    find = true;
                }
            }
            if(!find){
                if(!t.loadFromFile(path)){
                    Log::error("IMP L0","couldn't import:"+path,code->children[i]);
                    return;
                }
            }
            Cmd c(syn);
            c.file = code->file;
            c.line = code->line;
            c.loadCode(&t);
            delete code->children[i];
            code->children.erase(code->children.begin()+i);
            for(int a=0;a<c.children.size();a++)
                code->children.insert(code->children.begin()+a,c.children[a]);
            c.children.clear();
        }else
            load(code->children[i]);
    }
}
void Importer::library(Cmd* code){
    for(int i=0;i<code->children.size();i++){
        auto lib = code->children[i];
        if(lib->type=="LIBRARY"){
            std::vector<Cmd*> children;
            if(lib->children.size()){
                if(lib->children[0]->type=="CODE"){
                    for(auto x:lib->children[0]->children)
                        children.push_back(x);
                    lib->children[0]->children.clear();
                    delete lib->children[0];
                }else if(lib->children[0]->type=="VAR"){
                    children.push_back(lib->children[0]);
                }
                lib->children.erase(lib->children.begin());
            }
            for(int x=0;x<children.size();x++){
                children[x]->marker["library"] = lib->value;
                code->children.insert(code->children.begin()+i+x+1,children[x]);
            }
        }else
            library(code->children[i]);
    }
}
