#include "Deref.h"
#include "Log.h"

Deref::Deref(std::vector<std::pair<std::string,std::pair<std::string,std::string>>> codes){
	for(auto& x :codes){
        this->codes[x.first] = x.second;
    }
    /*
        Jeder Typ hat einen CODE bezeichner und einen LIFETIME bezeichner

        Der Code bezeichner,zeichnet aus was als "Code" bezeichnet werden kann
        Alle funktionen werden solange herausgeholt,bis sie auf einen "Code" treffen,
        dort werden sie dann in der ersten Zeile hineingepushed

        Der Lifetime bezeichner,zeichnet aus wann welche Variablen von welchem Code zugelassen sind
    */
}
void Deref::markCodeUnits(Cmd* c){
    auto whatiscode = codes[c->type].first;
    if(whatiscode.find("this")!=std::string::npos)
        c->marker["codeunit"]="true";
    Tokens tok(0);
    tok.loadFromString(whatiscode);
    for (auto& x: tok.children)
    {
        if(x->type==Tokens::NUMBER){
            int index = std::stoi(x->value);
            if(index<c->children.size()){
                if(c->children[index]->type!="CODE"){
                    auto s = c->children[index];
                    c->children[index] = new Cmd(c->children[index]->synSettings);
                    c->children[index]->type="CODE";
                    c->children[index]->file=s->file;
                    c->children[index]->type=s->line;
                    c->children[index]->children.push_back(s);
                }
                c->children[index]->marker["codeunit"]="true";
            }
        }
    }
    for(auto& x:c->children)
        markCodeUnits(x);

}
void Deref::extractFunctions(Cmd* c,Cmd* lastCodeUnit){
    if(c->marker["codeunit"]=="true")
            lastCodeUnit = c;
    for (size_t i = 0; i < c->children.size(); i++){
        Cmd*& cmd = c->children[i];
        //cmd->print("");
        extractFunctions(cmd,cmd->marker["codeunit"]=="true"?cmd:lastCodeUnit);
        while(c->children[i]!=cmd){
            i++;
        }
        if(cmd->type=="FUN"){
            auto init = new Cmd(c->synSettings);
            init->file = cmd->file;
            init->line = cmd->line;
            init->parent = cmd->parent;
            init->type = "INIT";
            init->markerCmd["fun"]=cmd;
            cmd->markerCmd["init"]=init;
            auto fun = cmd;
            cmd = init;
            lastCodeUnit->children.insert(lastCodeUnit->children.begin(),fun);
        }
    }
}
void Deref::resolve(Cmd* code,std::vector<Cmd*>& vars){
    //Diese Variablen sind nun fuer alle verfuegbar
    if(code->type == "VAR"||code->type == "FUN"||code->type == "CLASS"){
        vars.push_back(code);
        //verbinde extern mit global
        if(code->marker["extern"]=="true"&&code->marker["library"]==""){
            bool found = false;
            for (int i = vars.size() - 1; i >= 0; i--)
            {
                if(!found)
                    if( vars[i]->value==code->value&&vars[i]->marker["global"]=="true"){
                        code->markerCmd["extern"]= vars[i];
                        found = true;
                    }
            }
            if(!found){
                Log::error("DEF R0","Globale variable "+code->value+" doesn't exist.",code);
                return;
            }
        }
    }
    //loese den namen auf 
    if(code->type == "NAME"){
        bool found = false;
        for (int i = vars.size() - 1; i >= 0; i--)
        {
            if(!found)
            if( vars[i]->value==code->value
                &&(vars[i]->type=="VAR"?vars[i]->file==code->file:1)){
                code->markerCmd["var"]=vars[i];
                found = true;
            }
        }
        if(!found){
            code->type="VAR";
            vars.push_back(code);
        }
        
    }
    //resolve die INITs
    if(code->type=="INIT"){//seperat
        //this parameter
        auto& param = code->markerCmd["fun"]->parameter;
        param.insert(param.begin(),"this");
        //parameter to to CMD
        int count = 0;
        for(auto& x:param){
            Cmd* a = new Cmd(code->synSettings);
            a->type = "VAR";
            a->value = x;
            if(x.size()?x[0]=='$':0) //das Dollarzeichen gehoert nicht dazu
                a->value = x.substr(1);
            
            //das 0lte Child vom FUnction ist der CODE
            code->markerCmd["fun"]->children[0]->children.insert(code->markerCmd["fun"]->children[0]->children.begin()+count,a);
            a->marker["pseudo"] = "true";count++;   
        }
        resolve(code->markerCmd["fun"]->children[0],vars);
    }
    if(code->type=="CLASS"){//seperat
        //inits nach hinten
        std::vector<Cmd*> inits;
        for (size_t i = 0; i < code->children.size(); i++)
        {
            if(code->children[i]->type=="INIT")
            {
                //nach hinten
                inits.push_back(code->children[i]);
                code->children.erase(code->children.begin()+i);
                i--;
            }
        }
        for(auto&x :inits)
            code->children.push_back(x);
        for(auto& x:code->children)
            resolve(x,vars);
    }
    
    Tokens range(0);
    range.loadFromString(codes[code->type].second);
        
    for(auto&a :range.children)
    resolve(code,vars,a);
    
}
void Deref::resolve(Cmd* code,std::vector<Cmd*>& vars,Tokens* x){
        // number
        if(x->type==Tokens::NUMBER)
        {
            int num = std::stoi(x->value);
            if(num<code->children.size())
                resolve(code->children[num],vars);
        }
        // * all children
        else if(x->value=="*")
        {
            for(auto& c :code->children)
                resolve(c,vars);
        }
        //{} closed up
        else if(x->value=="{"){  
            std::vector<Cmd*> tmp = vars;
            for(auto& c :x->children)
                resolve(code,tmp,c);
        }
        //[x,y] einmal fuer x und einmal fuer y unabhaengig von einander
        else  if(x->value=="["){
            for(auto& a:x->children){
                std::vector<Cmd*> tmp = vars;
                resolve(code,tmp,a);
            }
        }
}
void Deref::varSet(Cmd* code){
    for(auto& x:code->children){
        if(x->children.size()>0?
            x->children[0]->type=="VAR"
            &&x->children[0]->marker["pseudo"]!="true"
            &&x->children[0]->marker["extern"]!="true"
        :0){
            if(x->type=="OPERATOR"&&x->value=="="){
                //a=b, setze b als child von a ein
                x->children[0]->children.push_back(x->children[1]);
                auto var = x->children[0];
                auto del = x;
                del->children.erase(del->children.begin());
                del->children.erase(del->children.begin());
                x = var;
                delete del;
            }else {
                Log::error("DEF VS0","Uninitialized Variable '"+x->children[0]->value+"' must be initialized with an '='.",x->children[0]);
                //VAR MUSS EINEN = OPERATOR HABEN
                return;
            }
        }else if(x->type=="VAR"&&x->marker["pseudo"]!="true"&&x->marker["extern"]!="true"){
            Log::error("DEF VS1","Uninitialized Variable '"+x->value+"'.",x);
            //VAR MUSS EINEN = OPERATOR HABEN
            return;
        }
        varSet(x);
    }
}
void Deref::initErase(Cmd* code){
    for(auto& x:code->children)
    {
        if(x->type=="INIT"){
            x->type = "NAME";
            x->markerCmd["var"] =x->markerCmd["fun"];
            x->markerCmd.erase("fun");
        }
        initErase(x);
    }
}
