#include "Writer.h"
#include <fstream>
#include "Algorithms.h"

Writer::Writer(Cmd* c){
    loadStrings(c);   
    loadLibraries(c);   
    loadExternals(c);
    initExternals();

    for(auto&x :c->children)
        output += write(x);
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
void Writer::loadExternals(Cmd* c){
    if(c->type=="VAR"){
        if(c->marker["extern"]=="true"){
            if(c->marker["library"]!=""){
                for(auto& x:libs){
                    if(x.name==c->marker["library"]){
                        x.externals.push_back(c);
                    }
                }
            }
        }
    }
    for(auto&x :c->children){
        loadExternals(x);
    }
}
void Writer::initExternals(){
    stackposition = 7;
    for(auto&x:libs){
        for(auto&y:x.externals){
            y->type = "FRONT";
            y->value = std::to_string(stackposition);
            stackposition++;
        }
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
    file << output.substr(0,output.size()-1);
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



std::string Writer::write(Cmd* code){
    
    if(code->type=="VAR")
        return writeVar(code);
    if(code->type=="NUMBER")
        return writeNumber(code);
    if(code->type=="STRING")
        return writeString(code);
    if(code->type=="OPERATOR")
        return writeOperator(code);
    if(code->type=="SUFFIX")
        return writeSuffix(code);
    if(code->type=="PREFIX")
        return writeSuffix(code);
    if(code->type=="NAME")
        return writeName(code);
    if(code->type=="POP")
        return writePop(code);
    if(code->type=="CLASS")
        return writeClass(code);

    return "";
}
std::string Writer::writeVar(Cmd* code){
    //there are 3 types of vars
    std::cout << code->value << std::endl;
    if(code->marker["library"]!="")
        return "";
    code->marker.clear();
    code->value = std::to_string(stackposition);
    stackposition++;
    code->type="FRONT";

    return write(code->children[0]);
}
std::string Writer::writeString(Cmd* code){
    return "copyfront 1\nsvs "+code->value+"\n";
}
std::string Writer::writeNumber(Cmd* code){
    return "copyfront 0\nsvi "+code->value+"\n";
}
std::string Writer::writeOperator(Cmd* code){
    std::string rv = "";
    for(auto x:code->children)
        rv+=write(x);
    if(code->value=="+")return rv+"add\n";
    if(code->value=="-")return rv+"sub\n";
    if(code->value=="*")return rv+"multi\n";
    if(code->value=="/")return rv+"div\n";
    if(code->value=="%")return rv+"mod\n";
    if(code->value=="+=")return rv+"setadd\n";
    if(code->value=="-=")return rv+"setsub\n";
    if(code->value=="*=")return rv+"setmulti\n";
    if(code->value=="/=")return rv+"setdiv\n";
    if(code->value=="%=")return rv+"setmod\n";
    if(code->value=="=")return rv+"set\n";
    if(code->value=="==")return rv+"equals\n";
    if(code->value=="!=")return rv+"unequals\n";
    if(code->value=="<")return rv+"smaller\n";
    if(code->value=="<=")return rv+"smallersame\n";
    if(code->value==">")return rv+"bigger\n";
    if(code->value==">=")return rv+"biggersame\n";
    if(code->value=="&&")return rv+"and\n";
    if(code->value=="||")return rv+"or\n";
}
std::string Writer::writeClass(Cmd* code){
    std::string rv = "";
    for(auto a:code->children)
        rv+=write(a);
    return rv+"collect "+std::to_string(code->children.size())+"\n";
}
std::string Writer::writeName(Cmd* code){
    auto x = code->markerCmd["var"];
    if(x->type=="FRONT")return "repushfront "+x->value+"\n";
    return "";
}
std::string Writer::writePrefix(Cmd* code){
    std::string rv = write(code->children[0]);
    if(code->value=="!")return rv+"not\n";
    if(code->value=="*")return rv+"deref\n";
    if(code->value=="&")return rv+"address\n";
    return "";
}
std::string Writer::writeSuffix(Cmd* code){

    if(code->value=="("){
        std::string rv = "";
        for(auto a:code->children)
            rv+=write(a);
        rv+="exec "+std::to_string(code->children.size()-1)+"\n";
        return rv;
    }
    return "";
}
std::string Writer::writePop(Cmd* code){
    return write(code->children[0])+"pop 1\n";
}