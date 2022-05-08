#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

struct Decode {
    char symbol[MAX];
    char opcode [21];
    char funct [5]; 
    char sregister [5];
    char saddress[3];
    char dregister [5];
    char daddress [3];
    char ssymbol [MAX];
    char dsymbol [MAX];
    char dataorstring[MAX];
    struct Decode* next;
};
const char* funct(char op[]);
void addtoDecode(struct Decode* head);
int opcode(char op[]);
void DecodeOpFunct(struct Decode* head, char theopcode[]);
void Decodesymbol(struct Decode* head, char symbol[]);
void DecodeSrg(struct Decode* head, char srg[]);
void Decodedrg(struct Decode* head, char drg[]);
void DecodeSadd(struct Decode* head, char sadd[]);
void Decodedadd(struct Decode* head, char dadd[]);