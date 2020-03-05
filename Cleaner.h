#ifndef CLEANER_H
#define CLEANER_H

#include "Cmd.h"

/*
    Meine Aufgabe:

    lösche Names
    füge an alle Operationen pop hinzu
*/

class Cleaner
{
    public:
        Cleaner(Cmd*);

    protected:
        void recursiv(Cmd*);
    private:
};

#endif // CLEANER_H
