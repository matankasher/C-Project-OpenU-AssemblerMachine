/* Copyright (c) 2022 Maor Lahat and Matan Kasher */
#include "prog.h"
int main(int argc, char *argv[])
{
   int flag, i;
   int IC, DC;
   for (i = 1; i < argc; i++)
   {
    struct  Macro* Mtail= NULL;
    struct  Macro* Mhead = NULL;
    struct  Stable* Stail= NULL;
    struct  Stable* Shead = NULL;
    struct  Decode* Dtail= NULL;
    struct  Decode* Dhead = NULL;
    Mhead = (struct Macro*)malloc(sizeof(struct Macro));
    Mtail = (struct Macro*)malloc(sizeof(struct Macro));
    Mtail = Mhead;
    Shead = (struct Stable*)malloc(sizeof(struct Stable));
    Stail = (struct Stable*)malloc(sizeof(struct Stable));
    Stail = Shead;
    Dhead = (struct Decode*)malloc(sizeof(struct Decode));
    Dtail = (struct Decode*)malloc(sizeof(struct Decode));
    Dtail = Dhead;
    flag = PreReadFile(i,argv,Mhead);
    if (!flag)
    {
    PreWriteFile(i,argv,Mtail);
    flag = FirstCheck(i,argv,Shead,Dhead,&IC,&DC);
    if(!flag)
     SecondCheck(i,argv,Stail,Dtail,&IC,&DC); 
    printentries(i,argv, Stail);
    printexternals(i,argv, Stail);
    }
   }
   return 1;
}

