#include "Writer.h"
#include <fstream>
#include "Algorithms.h"

Writer::Writer(Cmd* c){
    loadStrings(c);   
    loadLibraries(c);   
    
    c->print("");
}
void Writer::loadStrings(Cmd* c){
    if(c->type=="STRING")
    {
        auto s = strings.size();
        strings.push_back(c->value);
        c->value = std::to_string(s);
    }
    for(auto&x :c->children){
        loadStrings(x);
    }
}
void Writer::loadLibraries(Cmd* c){
    if(c->type=="LIBRARY"){
        libs.push_back({c->value,c->marker["version"],c->marker["size"]});
    }
    for(auto&x :c->children){
        loadLibraries(x);
    }
}
void Writer::write(std::string path){
    std::fstream file(path,std::ios::out);
    file<< "\"0.0.1\"";
    for(auto x:strings)
        file<<"\"" <<escape(x)<<"\"";
    file << std::endl;
    for(auto x:libs){
        file <<"lib\""<<x.name<<"\"\""<<x.version<<"\""<<x.size<<"\n";
    }
    file.close();
}
std::string Writer::escape(std::string x){
    for (size_t i = 0; i < x.length(); i++)
    {
        if(x[i]<32||x[i]=='"'||x[i]=='\''||x[i]>=127){
            char a = x[i]/16; 
            char b = x[i]%16;
            if(a<10)
                a += '0';
            else
                a += 'A'-10;
            if(b<10)
                b += '0';
            else
                b += 'A'-10;
            x = x.substr(0,i)+"\\x"+a+b+x.substr(i+1,x.length()-i-1);
            i+=3;
        }
    }
    return x;
}