#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100

struct Stable {
    char symbol [MAX]; 
    int value;
    int baseaddress;
    int offset;
    char attributes [5] ;
    struct Stable* next;
};
void addtotable(struct Stable *head , char symbol[] , int value);
int attributetype(char type[]);
void addattributes(struct Stable *tail , int type, char symbol[]);
const char *printAttributes (struct Stable *tail);
void printentries(int i, char *argv[],struct Stable *tail);
void printexternals(int i, char *argv[],struct Stable *tail);
