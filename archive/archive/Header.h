#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <limits.h>
#include <stdbool.h>


#define SIGNAURE 463788
#define SEEK_NEXT_TRUE 1037 //имя 1б + длинна имени 2б + 8б размер файла+2б флаг
#define SEEK_NEXT_FALSE 13

typedef struct Caption {
	char Name[256];
	unsigned short Namelen;
	unsigned long long  Fsize;	
	unsigned char *buf;
	short ComFlag;
}caption;

typedef struct sym {
	unsigned char ch;
	int count;
	char code[256];
	struct sym *left;
	struct sym *right;
}sym;

typedef struct SymTable {
	char code[256];
	int size;
}SymTable;

/*Функции для архивации*/
int ChekingArch(char **argv, int i, int j);
int ChekingSign(char *argv);
void RemoveFile(char **argv);
void AddToArch(char **argv);
int FindSameNameF(FILE *Arch, char **argv, int j, char *newname);
void AddFileToArch(FILE *Arch, char **argv, int j);
void cutname(char *newname);

/*функции для сжатия*/
void CountSimbls(FILE *file, unsigned int *sim_table);
void pr(sym *symbols, int u);
unsigned long long  compression(FILE *InFile, FILE *Arch);
int comp(const void* a, const void* b);
void makeCodes(sym *root, int num);
unsigned long long  WriteBits(FILE *InFile, SymTable *Alf, FILE *Out);
void decoding(FILE *OutFile, unsigned char *buf, unsigned int *simTable, unsigned long long  charcount);

