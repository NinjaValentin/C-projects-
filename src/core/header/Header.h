#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <limits.h>
#include <stdbool.h>

#define SIGNAURE 463788
#define SEEK_NEXT_TRUE 1037 //��� 1� + ������ ����� 2� + 8� ������ �����+2� ����
#define SEEK_NEXT_FALSE 13

typedef struct Caption
{
	char Name[256];
	unsigned short Namelen;
	unsigned long long Fsize;
	unsigned char *buf;
	short ComFlag;
} caption;

typedef struct sym
{
	unsigned char ch;
	int count;
	char code[256];
	struct sym *left;
	struct sym *right;
} sym;

typedef struct SymTable
{
	char code[256];
	int size;
} SymTable;

int chekingArch(char **argv, int i, int j);
int chekingSign(char *argv);
void removeFile(char **argv);
void addToArch(char **argv);
int findSameNameF(FILE *Arch, char **argv, int j, char *newname);
void addFileToArch(FILE *Arch, char **argv, int j);
void cutname(char *newname);
void writeSig(FILE *Arch);

void countSimbls(FILE *file, unsigned int *sim_table);
void pr(sym *symbols, int u);
unsigned long long compression(FILE *InFile, FILE *Arch);
int comp(const void *a, const void *b);
void makeCodes(sym *root, int num);
unsigned long long WriteBits(FILE *InFile, SymTable *Alf, FILE *Out);
void decoding(FILE *OutFile, unsigned char *buf, unsigned int *simTable, unsigned long long charcount);

void addFileToArch(FILE *, char **, int);
void removeFile(char **argv);
void extract(FILE *Arch, char **argv);
void addToArch(char **argv);
void getinf(FILE *Arch);
void extractAll(FILE *Arch);