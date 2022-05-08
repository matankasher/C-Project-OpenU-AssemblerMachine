
#include <stdio.h>
#include "firstcheck.h"

int NotExistSymbol(struct  Stable* tail , char sname[]) /*Checks if the symbol already exists*/
{
  struct  Stable* tailf = NULL;
  tailf = (struct Stable*)malloc(sizeof(struct Stable));
  tailf = tail;
  while (tailf != NULL)  /*Run on all the symbols we have already found*/
  { 
    if (!strcmp(tailf->symbol , sname))
    {
      return 0;
    }
    tailf = tailf->next;
  }
  return 1;
}
int DcData(char line[] , int index, struct  Decode* dhead) /*Summarize the number of values in the data prompt*/
{
  int dccounter = 1;
  while (line[index] != '\0')
  {
      if(line[index] == 44)
        dccounter++;
      index++;
  }
  return dccounter ;
}
int DcString(char line[] , int index, struct  Decode* dhead) /*Summarize the number of chars in the string prompt*/
{
  int dccounter = 0;
  while(isspace(line[index]))
    index ++;
  if (line[index] == 34)
  {
    index = index + 1;
    while (line[index] != 34 && line[index] != '\n' )
    {
      index++;
      dccounter++;
    }
  }
  return dccounter + 1;
  
}
int DataOrString(char line[] , int index, struct  Decode* dhead) /*Checks whether this is a data prompt or a string prompt, if so returns the space to be allocated for them*/
{
  int aindex = 0;
  char att[MAX];
  if(line[index] == '\n')
     return 0;
  memset(att, '\0' , MAX);
  while(isspace(line[index]))
      index ++;
  while (!isspace(line[index]) && line[index] != '\n')
      {
      att[aindex] = line[index];
      aindex++;
      index++;
      }
    if(!strcmp(".data",att))
        return(DcData(line , index, dhead));
    if (!strcmp(".string",att))
        return(DcString(line , index, dhead));
    return 0;
  
}
int IsSymbol(char line[] , struct  Stable* temp,struct  Stable* tail, struct  Decode* dhead) /*Checks whether a new symbol is being declared*/
{
  int index = 0 , sindex = 0;
  char sname[MAX];
  memset(sname, '\0' , MAX);
  if(line[index] == '\n')
     return 0;
  while(isspace(line[index]))
    index ++;
  while (!isspace(line[index]) && line[index] != '\n'  && line[index] != ':')
  {
    sname[sindex] = line[index];
    sindex++;
    index++;
  }
  if(line[index] == ':')
  {
    if (NotExistSymbol(tail , sname))
    {
      strcpy(temp->symbol,sname);
      strcpy(temp->attributes,"0000");
      temp->attributes[0] = temp->attributes[0] + DataOrString(line,index+1,dhead);
    return 1;
     }
     else
     {
       printf("error: %s already exist \n" , sname );
     }
   }
  return 0;
  
}
void ExternSymbol(struct  Stable* temp, char line[]) /*Inserts the label in the extern prompt to the symbol table*/
{
  char sname[MAX];
  int index = 0 , sindex = 0;
  memset(sname, '\0' , MAX);
    while(isspace(line[index]))
      index ++;
  while (!isspace(line[index]) )
      index++;
      while(isspace(line[index]))
      index ++;
  while (!isspace(line[index]) && line[index] != '\n')
      {
      sname[sindex] = line[index];
      sindex++;
      index++;
      }
  strcpy(temp->symbol,sname);
  temp ->value = 0;
  temp ->baseaddress = 0;
  temp ->offset = 0;
  temp ->attributes[3] = 49;


}
int EntryOrExtern(char line[]) /*Checks whether it is extern prompt or entry prompt*/
{
  int aindex = 0, index = 0 ;
  char att[MAX];
  if(line[index] == '\n')
     return 0;
  memset(att, '\0' , MAX);
  while(isspace(line[index]))
      index ++;
  while (!isspace(line[index]) && line[index] != '\n')
      {
      att[aindex] = line[index];
      aindex++;
      index++;
      }
    if(!strcmp(".entry",att))
        return(2);
    if(!strcmp(".extern",att))
        return(1);
    return 0;
  
}
const char *Rbinary(int n) /*Converts the decimal number to a binary number*/
{
  int i;
  char *p;
  char binary[5] = "0000";
  for(i=3;n>0;i--)
  {
     binary[i]=n%2 +48 ;   
     n=n/2;
  }    
  p = binary;
  return p; 
} 
int DestinationMode(char line[], int index ,struct  Decode* dhead ) /*Checks which address is in the destination*/
{
     char num[MAX];
     int nindex = 0, i;
     memset(num, '\0' , MAX);
     while(isspace(line[index]))
        index++;
     if(line[index] == '#') /* Immediate address */
     { 
        strcpy(dhead->daddress,"00");
        strcpy(dhead->dregister,"0000");
     }
     else
     if(line[index] == 'r') /*Direct register address*/
     {
        index++;
        while (line[index] > 47 && line[index] < 58)
        {
          num[nindex] = line[index];
          index++;
          nindex++;
        }
        i = atoi(num);
        if (i < 16)
        {
          strcpy(dhead->dregister,Rbinary(i));
          strcpy(dhead->daddress,"11");
        }
        return(index);
     }       
     else /* Direct address or index address */
     {
      strcpy(dhead->dregister,"0000");
     if(line[index] == 'r') 
     {
        index++;
        while (line[index] > 47 && line[index] < 58)
        {
          num[nindex] = line[index];
          index++;
          nindex++;
        }
        i = atoi(num);
        if (i < 16)
        {
          strcpy(dhead->dregister,Rbinary(i));
        }
     }
     else
     {
       while (line[index] != '[' && line[index] != ',' && !isspace(line[index]))
       {
          num[nindex] = line[index];
          index++;
          nindex++;
       }
       strcpy(dhead->dsymbol,num);
     }
       if (line[index] =='[') /* index address */
       {  
         memset(num, '\0' , MAX);
         nindex = 0;
         strcpy(dhead->daddress,"10");
         index++;
         if(line[index] == 'r') 
         {
           index++;
           while (line[index] > 47 && line[index] < 58)
           {
              num[nindex] = line[index];
              index++;
              nindex++;
           }
           i = atoi(num);
           if (i < 16)
           {
             strcpy(dhead->dregister,Rbinary(i));
           }
         }
         strcpy(dhead->daddress,"10");
       }
       else
         strcpy(dhead->daddress,"01"); /* Direct address */
     return index;
     }
    return 0;
}
int SourceMode(char line[], int index ,struct  Decode* dhead ) /*Checks which address is in the source*/
{
     char num[MAX];
     int nindex = 0, i;
     memset(num, '\0' , MAX);
     while(isspace(line[index]))
      index++;
     if(line[index] == '#') /* Immediate address */
     { 
        strcpy(dhead->saddress,"00");
        strcpy(dhead->sregister,"0000");
        while(!isspace(line[index]) && line[index] != ',')
           index++;
        while(isspace(line[index]))
           index++;
        return (index);
     }
     else
     if(line[index] == 'r') /*Direct register address*/
     {
        index++;
        while (line[index] > 47 && line[index] < 58)
        {
          num[nindex] = line[index];
          index++;
          nindex++;
        }
        i = atoi(num);
        if (i < 16)
        {
          strcpy(dhead->sregister,Rbinary(i));
          strcpy(dhead->saddress,"11");
        }
        while(isspace(line[index]))
           index++;
        return(index);
     }       
     else /* Direct address or  index address */
     {
      strcpy(dhead->sregister,"0000");
     if(line[index] == 'r')  
     {
        index++;
        while (line[index] > 47 && line[index] < 58)
        {
          num[nindex] = line[index];
          index++;
          nindex++;
        }
        i = atoi(num);
        if (i < 16)
        {
          strcpy(dhead->sregister,Rbinary(i));

        }
     }
     else
     {
       
       while (line[index] != '[' && line[index] != ',' && !isspace(line[index])) /* index address */
       {
          num[nindex] = line[index];
          index++;
          nindex++;
       }
      strcpy(dhead->ssymbol,num);
     }
       if (line[index] =='[') 
       {  
         memset(num, '\0' , MAX);
         nindex = 0;
         strcpy(dhead->saddress,"10");
         index++;
         if(line[index] == 'r') 
         {
           index++;
           while (line[index] > 47 && line[index] < 58)
           {
              num[nindex] = line[index];
              index++;
              nindex++;
           }
           i = atoi(num);
           if (i < 16)
           {
             strcpy(dhead->sregister,Rbinary(i));
           }
         }
       }
       else
         strcpy(dhead->saddress,"01");       /* Direct address */

       while(!isspace(line[index]) && line[index] != ',')
          index++;
       while(isspace(line[index]))
          index++;
     return index;
     }
}
int Checkopcode(char line[], int index ,struct  Decode* dhead ) /* Return the index of the opcode, and update the funct in the decode table */
{
  int oindex = 0;
  char opName[MAX];
  memset(opName, '\0' , MAX);
   while(isspace(line[index]))
      index++;
   while(!isspace(line[index]))
   {
      opName[oindex] = line[index];
      oindex++;
      index++;
   }
   DecodeOpFunct(dhead,opName);
   return index;
}
int Instruction(char line[],int index,struct  Decode* dhead) /* update the address and registers acordding the opcode */
{
  int j,counter = 0 ;
  strcpy(dhead->dsymbol,"NULL");
  strcpy(dhead->ssymbol,"NULL");
    if ((strcmp(dhead->opcode,"01000000000000000001")&& strcmp(dhead->opcode,"01000000000000000010") 
        && strcmp(dhead->opcode,"01000000000000000100")&& strcmp(dhead->opcode,"01000000000000010000")))
       {
         if ((!strcmp(dhead->opcode,"01000100000000000000")|| !strcmp(dhead->opcode,"01001000000000000000")))
         {
             strcpy(dhead->saddress,"00");
             strcpy(dhead->daddress,"00");
             strcpy(dhead->dregister,"0000");
             strcpy(dhead->sregister,"0000");
             return 1;
         }
         else
         {
             strcpy(dhead->saddress,"00");
             strcpy(dhead->sregister,"0000");
             DestinationMode(line,index,dhead);
             if(!strcmp(dhead->daddress,"11"))
                return 2;
             if(!strcmp(dhead->dsymbol,"NULL"))
               return 3;
             return 4;
         }
         
       }
      else
      {
        j = SourceMode(line,index,dhead);
        while(isspace(line[j]))
          j++;
        DestinationMode(line,j+1,dhead);
        if(!strcmp(dhead->daddress,"11"))
           counter--;
        if(!strcmp(dhead->saddress,"11"))
           counter--;
        if (strcmp(dhead->dsymbol,"NULL"))
          counter++;
        if (strcmp(dhead->ssymbol,"NULL"))
          counter++;   
        return (4 + counter);
      }
  }
int FirstCheck(int IJ, char *argv[], struct  Stable* shead, struct  Decode* dhead, int *IC, int *DC)
{
  int value, index = 0, errorflag = 0;
  int tempDC = 0 , tempIC = 100 ; 
  char line[MAX];
  char fname[MAX];
  FILE *fpr;
  struct  Stable* tail= NULL;
  struct  Decode* endd= NULL;
  tail = (struct Stable*)malloc(sizeof(struct Stable));
  tail = shead;
  memset(line, '\0' , MAX);
  strcpy(fname,argv[IJ]);
  strncat(fname, ".am", 3); 
  fpr = fopen(fname,"r");
  while (fgets(line, MAX, fpr))
  {
   struct  Decode* dtemp= NULL;
   struct  Stable* temp = NULL;
   dtemp = (struct Decode*)malloc(sizeof(struct Decode));
   temp = (struct Stable*)malloc(sizeof(struct Stable));
   strcpy(dtemp->symbol,"null");
    if(IsSymbol(line,temp,tail,dtemp))
    {
      dhead->next = dtemp;
      dhead =  dhead->next;
       strcpy(dtemp -> symbol, temp->symbol);
        temp ->value = tempIC;
        temp ->baseaddress = tempIC - tempIC %16;
        temp ->offset = tempIC % 16;
        shead ->next = temp;
        shead =  shead->next;
        while(isspace(line[index]))
             index++;
        while(!isspace(line[index]))
             index++;
        if (DataOrString(line,index,dtemp))
        {
              value = DataOrString(line,index,dtemp);
                tempDC = tempDC + value;
                tempIC  = tempIC + value;    
        }
        else
        {
          index = Checkopcode(line,index,dtemp);
          tempIC = tempIC + Instruction(line,index,dtemp);
        }
    }
    else if(DataOrString(line , 0,dhead))
    {
      value = DataOrString(line , 0,dhead);
      dhead->next = dtemp;
      dhead =  dhead->next;
      tempDC = tempDC + value;
      tempIC = tempIC + value;
    }
    else if(EntryOrExtern(line) == 1)
    {
     ExternSymbol(temp, line);
     shead ->next = temp;
     shead =  shead->next;
    }
    else if(!EntryOrExtern(line)) /* Instruction call  */
    {
      while(isspace(line[index]))
        index++;
      index =  Checkopcode(line,index,dtemp);
      if (!strcmp(dtemp->opcode,"00000000000000000000"))
      {
        printf("\n Invalid instruction line:  %s \n ", line);
         return 1;   
      }

      else
      {
        tempIC = tempIC + Instruction(line,index,dtemp);
        dhead->next = dtemp;
        dhead =  dhead->next;
      }
    }      
    index = 0 ;
  }
  fclose(fpr);
  endd = (struct Decode*)malloc(sizeof(struct Decode)); 
  dhead->next = endd;
  *IC = tempIC;
  *DC = tempDC;
  return errorflag;
}