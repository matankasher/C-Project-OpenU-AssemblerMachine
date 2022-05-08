#include "secondcheck.h"
int  BinaryConvert(char st[], int index)  /* convert binary to hexdecimal */
{
  int i = 0, binary = 0 , j = 1;
   for (i = 0; i < 4; i++)
   {
     binary = binary + (st[index] -48) * j;
     j = j * 2;
     index --;
   }
   if (binary  < 10 )
     return (binary + 48);  
   return (binary + 87);
}
const char *BinaryWide(int n)   /* convert decimal to binary */
{
  int i;
  char *p;
  char binary[21] = "01000000000000000000";
  for(i=19;n>0;i--)
  {
     binary[i]= n%2 +48 ;   
     n=n/2;
  }    
  p = binary;
  return p; 
} 
void PrintFile(FILE *fpw ,char st[], int IC)  /* insert the line to our output file */
{
     fprintf(fpw , "0%d  A%c-B%c-C%c-D%c-E%c \n",IC,BinaryConvert(st, 3), BinaryConvert(st, 7)
              , BinaryConvert(st, 11) ,BinaryConvert(st, 15), BinaryConvert(st, 19));
}
const char *Printfunct(char funct[], char sregister[], char saddress[],char dregister[],char daddress[]) /* Placement the parts of the line to one. */
{
  char *p;
  char binary[21] = "01000000000000000000";
  binary[4]  = funct[0];
  binary[5]  = funct[1];
  binary[6]  = funct[2];
  binary[7]  = funct[3];
  binary[8]  = sregister[0];
  binary[9]  = sregister[1];
  binary[10] = sregister[2];
  binary[11] = sregister[3];
  binary[12] = saddress[0];
  binary[13] = saddress[1];
  binary[14] = dregister[0];
  binary[15] = dregister[1];
  binary[16] = dregister[2];
  binary[17] = dregister[3];
  binary[18] = daddress[0];
  binary[19] = daddress[1];
  p = binary;
  return p; 
}
void printBaseOffset(FILE *fpw,char symbol[],int counter,struct  Stable* stail) /* print to output file the base and the offset of the symbol */
{
  struct  Stable* temp = NULL;
  char printbinary[MAX];
  temp = (struct Stable*)malloc(sizeof(struct Stable));
  temp = stail;
  memset(printbinary, '\0' , MAX);
  while (temp != NULL)
      {
        if (!strcmp(temp->symbol,symbol))
            break;
        temp = temp->next;  
      }
  if (temp != NULL)
  {
    strcpy(printbinary,BinaryWide(temp->baseaddress));
    printbinary[1] ='0';
    if (temp->attributes[3] > 48) /* if it's extern symbol */
       printbinary[3] ='1';
    else
    printbinary[2] ='1';
    PrintFile(fpw ,printbinary, counter);
    counter++;
    strcpy(printbinary,BinaryWide(temp->offset));
    printbinary[1] ='0';
    if (temp->attributes[3] > 48) /* if it's extern symbol */
       printbinary[3] ='1';
    else
    printbinary[2] ='1';
    PrintFile(fpw ,printbinary, counter);
  }
}
void AddCodeATT(char symbol[],struct  Stable* stail) /* add code attribute to symbol that showed at instruction line */
{
  struct  Stable* temp = NULL;
  temp = (struct Stable*)malloc(sizeof(struct Stable));
  temp = stail;
  while (temp != NULL)
      {
        if (!strcmp(temp->symbol,symbol))
            break;
        temp = temp->next;  
      }
      temp->attributes[1]++;
}
const char *negative(char num[]) /* find the Complementary to 2 for our negavie number */
{
  int i , j = 20 , n = atoi(num);
  char *p;
  char binary[21] = "01000000000000000000";
  for(i=19;n>0;i--)
  {
     binary[i]= n%2 +48 ;   
     n=n/2;
  }   
  while (j > 4)
  {
    if (binary[j] == 49)
    {
      j--;
      while (j > 3 )
      {
        if (binary[j] == 48)
            binary[j] ++;
        else
            binary[j] --;
        j--;
      }    
    }
    j--;
  }
    p = binary;
  return p; 
}
int SourceR(char opcode[]) /* if we have source to our opcode */
{
  if (strcmp(opcode,"01000000000000000001")&& strcmp(opcode,"01000000000000000010") 
        && strcmp(opcode,"01000000000000000100")&& strcmp(opcode,"01000000000000010000"))
    return 0;
  return 1;
  
}
int destnationR(char opcode[]) /* if we have destnation to our opcode */
{
if ((!strcmp(opcode,"01000100000000000000"))|| !strcmp(opcode,"01001000000000000000"))
    return 0;
  return 1;
  
}
void SecondCheck(int IJ, char *argv[],struct  Stable* stail, struct  Decode* dtail ,int *IC , int *DC)
{                
  int entryFlag = 0, counter = 100, nindex = 0, index = 0, sindex = 0, datavalue , i , minusflag = 0;
  char line[MAX];
  char symbol[MAX];
  char printbinary[MAX];
  char num[MAX];
  char fname[MAX];
  char fname2[MAX];
  FILE *fpr, *fpw;
  struct  Stable* shead = NULL;
  shead = (struct Stable*)malloc(sizeof(struct Stable));
  memset(line, '\0' , MAX);
  strcpy(fname,argv[IJ]);
  strncat(fname, ".am", 3);
  strcpy(fname2,argv[IJ]);
  strncat(fname2, ".ob", 3);
  fpr = fopen(fname,"r");
  fpw = fopen(fname2,"w");
  dtail = dtail->next; 
  fprintf(fpw , "   %d  %d \n",*IC - *DC -100 , *DC);
  while (fgets(line, MAX, fpr)) /* read next line from the file */
  {
    if (strcmp(dtail->symbol,"null"))  /* if we have label define in the line */
    {
      while(!isspace(line[index]))
       index++;
    }
    datavalue = DataOrString(line,index,dtail);
    if ( datavalue > 0)   /* if we have a data or string order in the line */
    {
      
        while(isspace(line[index]))
        index++;  
        if (line[index + 1] == 's') /* it's string order */
        {
           while(!isspace(line[index]))
             index++;
           while(isspace(line[index]))
             index++;    
           index++;
           for ( i = 1; i < datavalue; i++)
           {
               strcpy(printbinary,BinaryWide(line[index]));
               PrintFile(fpw,printbinary,counter);
               index++;
               counter++;
           }           
          PrintFile(fpw,"01000000000000000000",counter);
          counter++;
        }
        
      else  /* It's data order */
      {
        while(!isspace(line[index]))
          index++;    
        for ( i = 0; i < datavalue; i++)
        {
          memset(num, '\0' , MAX);
          nindex = 0;
          while(isspace(line[index]))
          index++;
          minusflag = 0;
          if (line[index] == 45 )
            {
              minusflag = 1;
              index ++;
            }
          else if (line[index] == 43 )
              index++;
          while(line[index] > 47 && line[index] < 58)
          {
            num[nindex] = line[index];
            nindex ++;
            index++;
          }
          if (atoi(num) != 0) 
          {
             if (!minusflag)   /* Check the number if it's positive of negative number */
               strcpy(printbinary,BinaryWide(atoi(num)));
            else
               strcpy(printbinary,negative(num));
            PrintFile(fpw,printbinary,counter);
            counter++;
           while(isspace(line[index]))
              index++;  
            index++; 
          }
        }
      } 
     dtail = dtail->next; 
    } 
    else   
    if((!(EntryOrExtern(line) == 1)))   /* If it is'nt extern label */
      {
        if(EntryOrExtern(line) == 2)   /* If it's entry label */
        {
          
           memset(symbol, '\0' , MAX);
           index = 0;
           sindex = 0;
           shead = stail; 
           while(isspace(line[index]))
           index ++;
           while(!isspace(line[index]))
           index ++;
           while(isspace(line[index]))
           index ++;
           while(!isspace(line[index]))
           {
              symbol[sindex] = line[index];
              index++;
              sindex++;
           }
           while (shead != NULL && !entryFlag)
           {
             if (!strcmp(shead->symbol,symbol))
             {
                shead->attributes[2] ++;
                entryFlag++;
             }
             shead =shead->next;  
           }
          if(!entryFlag)
             printf("\n  error! the symbol : %s dosent exist \n",symbol);
          entryFlag = 0;
        }
        else   /* If it's instruction line */
        {
          PrintFile(fpw ,dtail->opcode, counter);
          counter++;
           if ((strcmp(dtail->opcode,"01000100000000000000")&& strcmp(dtail->opcode,"01001000000000000000")))
          {
              strcpy(printbinary,Printfunct(dtail->funct,dtail->sregister,dtail->saddress,dtail->dregister,dtail->daddress));
              PrintFile(fpw ,printbinary, counter);
              counter++;   
          }       
          if (strcmp(dtail->ssymbol,"NULL"))
          {
            printBaseOffset(fpw,dtail->ssymbol,counter,stail);
            AddCodeATT(dtail->ssymbol,stail);
            counter = counter + 2;
          } 
          
          if( (!strcmp(dtail->saddress,"00") && (SourceR(dtail->opcode))) || (!strcmp(dtail->daddress,"00") && destnationR(dtail->opcode)) )
          {  /* If we have direct address */
            minusflag = 0;
            memset(num, '\0' , MAX);
            nindex = 0;
            while (line[index] != 35)
                index++;
            index++;
            if (line[index] == 45 )
            {
              minusflag = 1;
              index ++;
            }
            else if (line[index] == 43 )
              index++;
            while (line[index] > 47 && line[index] < 58)
            {
              num[nindex] = line[index];
              index++;
              nindex++;
            }
            if (!minusflag)
               strcpy(printbinary,BinaryWide(atoi(num)));
            else
               strcpy(printbinary,negative(num));
            PrintFile(fpw,printbinary,counter);
            counter++;
          }
          if (strcmp(dtail->dsymbol,"NULL") )
          {
            printBaseOffset(fpw,dtail->dsymbol,counter,stail);
            AddCodeATT(dtail->dsymbol,stail);
            counter = counter + 2;
          }
             dtail = dtail->next;
        }   
      }
      index = 0 ;
      minusflag = 0 ;

  }
  fclose(fpr);
  fclose(fpw);
}