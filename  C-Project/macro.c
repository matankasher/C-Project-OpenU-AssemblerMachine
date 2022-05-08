#include "macro.h"
/*We implement a table of macros*/
void addToMtable(struct Macro *head , char name[] , char content[]) /*implements a table of macros*/
{
  struct  Macro *temp = NULL;
  temp = (struct Macro*)malloc(sizeof(struct Macro));
  memset(temp->mname, '\0' , MAX);
  memset(temp->mcontent , '\0' , MAX);
  strcpy(temp->mname,name);
  strcpy(temp->mcontent,content);
  head ->next = temp;
}
void printMacroTable(struct Macro *tail) /*Prints the macro table*/
{
  struct  Macro *temp = NULL;
  temp = (struct Macro*)malloc(sizeof(struct Macro));
  temp = tail-> next;
  while(temp != NULL)
  {
    printf("\n macro name:  %s \n  content:  %s \n", temp->mname , temp->mcontent);
    temp = temp-> next;
  }
} 

int IsMacroOrEndm(char line[]) /*Checks whether it is the beginning of a macro or the end of a macro*/
{
  int index = 0 , mindex = 0 ;
  char macro [MAX];
  memset(macro , '\0' , MAX);
  while(isspace(line[index]))
       index ++;
  while (!isspace(line[index]) && line[index] != '\n')
  {
    macro[mindex] = line[index];
    mindex++;
    index++;
  }
  if (!strcmp(macro, "macro"))
    return 1;
    if (!strcmp(macro, "endm"))
    return 2;
  return 0;
}
void InsertName(struct Macro *temp, char line[]) /*Enter the macro name in the macros table*/
{
  int index = 0 , nindex = 0 ;
  char name [MAX];
  memset(name , '\0' , MAX);
  while(isspace(line[index]))
    index ++;
  while (!isspace(line[index]) && line[index] != '\n')
    index++;
    while(isspace(line[index]))
    index ++;
  while (!isspace(line[index]) && line[index] != '\n')
  {
    name[nindex] = line[index];
    nindex++;
    index++;
  }
  strcpy(temp->mname,name);
}
void InsertContent(struct Macro *temp, FILE *fp) /*Inserts the macro contents into the macros table*/
{
  char line [MAX];
  char content [MAX];
  memset(line , '\0' , MAX);
  memset(content , '\0' , MAX);
  fgets(line, MAX, fp);
 while(IsMacroOrEndm(line) != 2)
  {
    strncat(content, line , MAX);   
    fgets(line, MAX, fp);
  }
  strcpy(temp->mcontent,content);
}
int PreReadFile(int i, char *argv[],struct Macro *head) /*Performing the first pass on the file (inserting the macros into the macro table,
                                                        copying the corresponding rows from the table to the file, etc.)*/
{
  char line [MAX];
  FILE *fp;
  memset(line , '\0' , MAX);
  fp = fopen(argv[i],"r");
  if(fp == NULL)
  {
     printf("error: cant open the file: %s \n \n" , argv[i]);  
     return 1;
  }
    while(fgets(line, MAX, fp))
  {
   struct  Macro* temp = NULL;
   temp = (struct Macro*)malloc(sizeof(struct Macro)); 
   if(IsMacroOrEndm(line) == 1)
      { 
        InsertName(temp , line);
        InsertContent(temp , fp);
        head -> next = temp ;
        head = temp;
      }
  }
  return 0;
}
int IsMacroCall(char line[], FILE *fpw,struct Macro *tail) /*Copy the contents of the corresponding macro to the file from the table, if it is a macro command*/
{
  int index = 0, mindex = 0;
  char Mname [MAX];
  struct  Macro *temp = NULL;
  temp = (struct Macro*)malloc(sizeof(struct Macro));
  temp = tail;
  memset(Mname, '\0' , MAX);
  while(isspace(line[index]))
    index ++;
  while (!isspace(line[index]) && line[index] != '\n')
  {
    Mname[mindex] = line[index];
    mindex++;
    index++;
  }
  while (temp != NULL)
  {
    if (!strcmp(temp->mname , Mname))
    {
    fprintf(fpw, "%s", temp->mcontent);
    return 1;
    }
    temp = temp ->next;
  }
  return 0;
}
void PreWriteFile(int i, char *argv[],struct Macro *tail)
{
  int macroflag = 0; 
  FILE *fpw;
  FILE *fpr;
  char line [MAX];
  char fname[MAX];
  char fname2[MAX];
  memset(line , '\0' , MAX);
  strcpy(fname,argv[i]);
  strcpy(fname2,argv[i]);
  strncat(fname2, ".am", 3);
  fpr = fopen(fname,"r");
  fpw = fopen(fname2,  "w");
  if(fpr == NULL)
     printf("error: cant open the file: %s \n \n" , argv[1]); 
  while(fgets(line, MAX, fpr))
  {
    if(!macroflag)
    {   
       if(!IsMacroCall(line, fpw, tail))
       {
         if(IsMacroOrEndm(line) == 0)
         {
            fprintf(fpw,"%s",line);
         }
         else 
         {
            macroflag = 1;
         }
       }
    }
    else
    {
      if(IsMacroOrEndm(line) == 2)
        macroflag = 0;
    }        
  } 
  fclose(fpw);
  fclose(fpr);
}