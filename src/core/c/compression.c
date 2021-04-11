#include "Header.h"
#include "Errors.h"

void countSimbls(FILE *file, unsigned int *simTable) {
	unsigned int ch;
	while ((ch = fgetc(file)) < 256) {
		simTable[ch]++;
	}
}

sym *makeTree(sym *psym[], int k)
{
	sym *temp;
	int i, j;

	temp = (sym*)malloc(sizeof(sym));
	if (k == 1) {
		temp->count = psym[k - 1]->count;
		temp->code[0] = 0;
		temp->left = NULL;
		temp->right = psym[k - 1];
		return temp;
	}
	temp->count = psym[k - 1]->count + psym[k - 2]->count;
	temp->code[0] = 0;
	temp->left = psym[k - 1];
	temp->right = psym[k - 2];

	if (k == 2)
		return temp;
	else
	{
		for (i = 0;i<k;i++)
			if (temp->count>psym[i]->count)
			{
				for (j = k - 1;j>i;j--)
					psym[j] = psym[j - 1];

				psym[i] = temp;
				break;
			}
	}
	return makeTree(psym, k - 1);
}

unsigned long long  compression(FILE *InFile, FILE *Arch) {
	
	caption inf;
	sym *symbols = NULL;
	SymTable Alf[256] = { 0 };
	unsigned long long  charcount; 
	int chh;
	int i;
	sym **psym = NULL; 
	int counter = 0;
	unsigned int simTable[256] = {0};
 
	countSimbls(InFile, simTable);	
	fwrite(simTable, sizeof(unsigned int), 256, Arch);	

	for (int i = 0;i < 256; i++) {
		if (simTable[i] != 0) {
			if (!(counter % 8))
				symbols = (sym*)realloc(symbols, (counter + 8) * sizeof(sym));
			symbols[counter].ch = (unsigned char)i;
			symbols[counter].count = simTable[i];
			symbols[counter].left = NULL;
			symbols[counter].right = NULL;
			counter++;
		}
	}

	qsort(symbols, counter, sizeof(sym), comp);

	psym = (sym**)malloc(counter * sizeof(sym*));
	for (i = 0; i < counter; i++)
		psym[i] = &symbols[i];

	sym *root = makeTree(psym, counter);	
	makeCodes(root, counter);

	
	for (i = 0;i < counter;i++) {
		int m, k;
		m = (unsigned char)symbols[i].ch;
		k = strlen(symbols[i].code);

		Alf[m].size = k;
		strcpy(Alf[m].code, symbols[i].code);
	}
	//pr(symbols, counter);

	rewind(InFile);
	charcount = WriteBits(InFile, Alf, Arch);
	
	free(symbols);
	free(psym);
	return charcount;
}

void makeCodes(sym *root, int num)
{
	if (root == NULL)
		return;


	if (root->left)
	{
		strcpy(root->left->code, root->code);
		strcat(root->left->code, "0");
		makeCodes(root->left, num);
	}
	if (root->right)
	{
		strcpy(root->right->code, root->code);
		strcat(root->right->code, "1");
		makeCodes(root->right, num);
	}
}

unsigned long long  WriteBits(FILE *InFile, SymTable *Alf, FILE *Arch) {
	unsigned int  ch;
	int position = 0;
	unsigned long long  charcount = 0;
	unsigned char buf = 0;

	while ((ch = fgetc(InFile)) != EOF) {
		for (int n = 0; n < Alf[ch].size; n++) {

			buf = buf | (Alf[ch].code[n] - '0') << (7 - position);
			position++;
			charcount++;
			if (position == 8) {
				position = 0;
				fwrite(&buf, sizeof(char), 1, Arch);
				buf = 0;
			}
		}
	}
//	printf("%d",position);
	if (position != 0)
		fwrite(&buf, sizeof(char), 1, Arch);
	return charcount;
}

void decoding(FILE *OutFile, unsigned char *buf, unsigned int *simTable, unsigned long long  charcount) {

	sym *symbols = NULL;
	sym **psym = NULL;
	int counter = 0; 
	int position = 0;
	unsigned char byte = 0;
	unsigned long long  size = 0;
	int i = 0;

	for (int i = 0;i < 256; i++) {
		if (simTable[i] != 0) {
			if (!(counter % 8))
				symbols = (sym*)realloc(symbols, (counter + 8) * sizeof(sym));
			symbols[counter].ch = (unsigned char)i;
			symbols[counter].count = simTable[i];
			symbols[counter].left = NULL;
			symbols[counter].right = NULL;
			counter++;
		}
	}

	qsort(symbols, counter, sizeof(sym), comp);

	psym = (sym**)malloc(counter * sizeof(sym*));
	for (i = 0; i < counter; i++)
		psym[i] = &symbols[i];

	sym *root = makeTree(psym, counter);
	sym *tree = root;

	i = 0;
	byte = buf[i];
	while (size < charcount) {

		bool bit = byte & (1 << (7 - position));

		if (!bit)
			tree = tree->left;
		else
			tree = tree->right;
		if ((tree->left == NULL) && (tree->right == NULL)) {
			fwrite(&tree->ch, sizeof(char), 1, OutFile);
			tree = root;
		}
		position++;
		size++;
		if (position == 8) {
			position = 0;
			i++;
			byte = buf[i];
		}
	}
}