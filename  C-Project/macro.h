#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100

struct Macro {
    char mname[MAX];
    char mcontent[MAX];
    struct Macro* next;
};
void addToMtable(struct Macro *head , char name[] , char content[]);
void printMacroTable(struct Macro *tail);
int IsMacroOrEndm(char line[]);
void InsertName(struct Macro *temp, char line[]);
void InsertContent(struct Macro *temp, FILE *fp);
int PreReadFile(int i, char *argv[],struct Macro *head);
int IsMacroCall(char line[], FILE *fpw,struct Macro *tail);
void PreWriteFile(int i, char *argv[],struct Macro *tail);