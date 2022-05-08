
#include "stable.h"
void addtotable(struct Stable *head , char symbol[] , int value)  /* Adds a new element to the stable table */ 
{
  struct  Stable *temp = NULL;
  temp = (struct Stable*)malloc(sizeof(struct Stable));
  strcpy(temp->symbol,symbol);
  temp ->value = value;
  strcpy(temp->attributes,"0000");
  temp ->baseaddress = value - value %16;
  temp ->offset = value % 16 ;
  head ->next = temp;
}
int attributetype(char type[]) /* Returns which prompt is in between string, extern, data, code and entry */
{
  if (!strcmp(type,".data") || !strcmp(type,".string"))
    return 0;
  if (!strcmp(type,".code"))
    return 1;
  if (!strcmp(type,".entry"))
    return 2;
  if (!strcmp(type,".extern"))
    return 3;
  return -1;
}
void addattributes(struct Stable *tail , int type, char symbol[])
{
  struct  Stable *temp = NULL;
  if(type == -1)
     return;
  temp = (struct Stable*)malloc(sizeof(struct Stable));
  temp = tail;
    while (temp != NULL)
    {
        if (!strcmp(temp ->symbol,symbol))
        {
            (temp->attributes)[type] ++;
            return;
        }
        else
        temp = temp->next;  
    }   
}
const char *printAttributes (struct Stable *tail)  /* retun the attributes for symbol at the current node */
{
  char *p;
  char new[MAX];
  memset(new , '\0' , MAX);
  if(tail->attributes[0] > 48)
     strncat(new, "data ", 5);
  if(tail->attributes[1] > 48)
     strncat(new, "code ", 5);
  if(tail->attributes[2] > 48)
     strncat(new, "entry ", 6);
  if(tail->attributes[3] > 48)
     strncat(new, "extern ", 7);
  p = new;
     return p;
  
}
void printentries(int i, char *argv[],struct Stable *tail) /* Print the entries file */
{
  FILE *fp;
  char fname[MAX];
  struct  Stable *temp = NULL;
  strcpy(fname,argv[i]);
  strncat(fname, ".ent.txt", 8);
  fp = fopen(fname,"w");
  temp = (struct Stable*)malloc(sizeof(struct Stable));
  temp = tail-> next;
  fprintf(fp, "Symbol    Value   BaseAddress   Offset    Attributes \n" );
  while(temp != NULL)
  {   
    if(temp->attributes[2] > 48)
    fprintf(fp," %-10s%-11d%-12d%-8d%-11s\n",
    temp->symbol,  temp->value, temp->baseaddress ,temp->offset ,printAttributes(temp));
    temp = temp-> next;
  }
  fclose(fp);
}
void printexternals(int i, char *argv[],struct Stable *tail) /* Print the externals file */
{
  FILE *fp;
  char fname[MAX];
  struct  Stable *temp = NULL;
  strcpy(fname,argv[i]);
  strncat(fname, ".ext", 4);
  fp = fopen(fname,"w");
  temp = (struct Stable*)malloc(sizeof(struct Stable));
  temp = tail-> next;
  fprintf(fp, "Symbol    Value   BaseAddress   Offset    Attributes \n" );
  while(temp != NULL)
  {
    if(temp->attributes[3] > 48)
    fprintf(fp," %-10s%-11d%-12d%-8d%-11s\n",
    temp->symbol,  temp->value, temp->baseaddress ,temp->offset ,printAttributes(temp));

    temp = temp-> next;
  }
  fclose(fp);
} 
