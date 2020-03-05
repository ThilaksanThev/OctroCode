#include "Cleaner.h"

Cleaner::Cleaner(Cmd* c)
{
    recursiv(c);
}


void Cleaner::recursiv(Cmd* c){
    for(int i = 0;i<c->children.size();i++){
        auto& x = c->children[i];

        if(x->type!="VAR"&&x->type!="SUFFIX"&&x->type!="PREFIX"&&x->type!="OPERATOR"&&x->type!="LIBRARY")
            recursiv(x);
        if(x->type!="VAR"&&x->type!="FUN"&&x->type!="CLASS"&&x->type!="LIBRARY"){
            Cmd* pop = new Cmd(nullptr);
            pop->type="POP";
            pop->children.push_back(x);
            auto v = x;
            x = pop;
            if(v->type!="SUFFIX"&&v->type!="PREFIX"&&v->type!="OPERATOR")
            {
                delete pop;
                c->children.erase(c->children.begin()+i);
                i--;
            }
        }

    }
}
