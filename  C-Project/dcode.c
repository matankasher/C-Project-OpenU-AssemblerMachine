#include "dcode.h"
const char* funct(char op[]) /*Returns the funct number of the instructions as a binary number*/
{
  char *p;
  char new[5];
  memset(new , '\0' , 5);
  if(!strcmp(op, "add") || !strcmp(op, "clr") || !strcmp(op, "jmp"))
    strcpy(new,"1010");
  else
  if(!strcmp(op, "sub") || !strcmp(op, "not") || !strcmp(op, "bne"))
    strcpy(new,"1011");
  else
  if(!strcmp(op, "inc") || !strcmp(op, "jsr"))
    strcpy(new,"1100");
  else
  if(!strcmp(op, "dec"))
    strcpy(new,"1101");
  else
    strcpy(new,"0000");
 p = new;
  return p;
}
void addtoDecode(struct Decode* head) /*Adds a new element to the decode table*/

{
  struct  Decode *temp = NULL;
  temp = (struct Decode*)malloc(sizeof(struct Decode));
  strcpy(temp->sregister, "0000");
  strcpy(temp->saddress, "00");
  strcpy(temp->dregister, "0000");
  strcpy(temp->daddress, "00");
  head ->next = temp;
}
int opcode(char op[]) /*Returns the opcode type in decimal base*/
{
  if(!strcmp(op, "mov"))
    return 0;
  if(!strcmp(op, "cmp"))
    return 1;
  if(!strcmp(op, "add") || !strcmp(op, "sub"))
    return 2;
  if(!strcmp(op, "lea"))
    return 4;
  if(!strcmp(op, "clr") || !strcmp(op, "not") || !strcmp(op, "inc") || !strcmp(op, "dec"))
    return 5;
  if(!strcmp(op, "jmp") || !strcmp(op, "bne") || !strcmp(op, "jsr"))
    return 9;
  if(!strcmp(op, "red"))
    return 12;
  if(!strcmp(op, "prn"))
    return 13;
  if(!strcmp(op, "rts"))
    return 14;
  if(!strcmp(op, "stop"))
    return 15;
return -1;
}
void DecodeOpFunct(struct Decode* head, char theopcode[]) /*Checks the type of opcode using the function we wrote above and then updates the relevant bit in the row*/
{
  
  int index = opcode(theopcode);
  char newop [21] = "01000000000000000000";
  newop[19 - index] = '1';
  if (index != -1)
      strcpy(head->opcode,newop);
  else
      strcpy(head->opcode,"00000000000000000000");
  strcpy(head->funct, funct(theopcode));
}
/*Functions for updating the relevant values in the table:*/
void Decodesymbol(struct Decode* head, char symbol[])
{
  strcpy(head->symbol , symbol);
}
void DecodeSrg(struct Decode* head, char srg[])
{
  strcpy(head->sregister,srg);
}
void Decodedrg(struct Decode* head, char drg[])
{
  strcpy(head->dregister,drg);
}
void DecodeSadd(struct Decode* head, char sadd[])
{
  strcpy(head->saddress,sadd);
}
void Decodedadd(struct Decode* head, char dadd[])
{
  strcpy(head->daddress,dadd);
}
