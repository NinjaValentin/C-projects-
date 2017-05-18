#include "Header.h"
#include "Errors.h"

//Функция разбиения строки для определения расширения файла
int ChekingArch(char **argv, int i, int j)
{	
	char temp[4];
	int sig, k = 0;
	char ff[] = "eue";

	for ( j; j > 0 && k < 4; j-- )
	{
		if (argv[i][j] == '.') {
			temp[k] = '\0';
			break;
		}
		temp[k] = argv[i][j];
		k++;
	}

	if (strcmp(temp, ff)) 	
			return -1;
	
	
	return 0;
}

//Проверка сигнатуры
int ChekingSign(char *argv) {
	FILE *Arch;
	int sig;

	Arch = fopen(argv, "rb");

	fread(&sig,sizeof(int),1, Arch);	
	fclose(Arch);
	
	if (sig != SIGNAURE)
		return -2;
	return 0;
}

//Запись сигнатуры
void WriteSig(FILE *Arch) {
	int sig;
	
	sig = SIGNAURE;

	fwrite(&sig, sizeof(int), 1, Arch);
}

//Обрезание врмененого имени
void cutname(char *newname) {
	int i,m;
	char tmp[255];
	m=i=strlen(newname);
	int k = 0;
	do {
		if (newname[m] == '\\')
			break;
		k++; m--;

	} while (tmp[i + 1] != '\\');
	
	do{
		if (newname[i] == '\\') {
			
			break;
		}
		tmp[k-1] = newname[i];
		i--;
		k--;
	} while (tmp[i + 1] != '\\');
	strcpy(newname, tmp);	
	
}

//Поиск файлов с одинаковым именем в архиве
int FindSameNameF(FILE *Arch, char **argv, int j, char *newname) {
	unsigned long long fullsize, size, tmp;
	caption inf;
	unsigned long long  byteSize;
	unsigned int simTable[256] = { 0 };

	fullsize = 0;
	size = 4; //счетчик для измерения текущего объема файла

	fseek(Arch, 0, SEEK_END);        // устанавливаем позицию в конец файла
	fullsize += ftell(Arch);         // получаем размер в байтах
	fseek(Arch, 4, SEEK_SET);
	while (size < fullsize) {
		tmp=ftell(Arch);
		fread(&inf.Namelen, sizeof(unsigned short), 1, Arch);
		fread(inf.Name, sizeof(char), inf.Namelen + 1, Arch);
		fread(&inf.ComFlag, sizeof(short), 1, Arch);
		fread(&inf.Fsize, sizeof(unsigned long long), 1, Arch);

		byteSize = inf.Fsize;

		//если файл сжат
		if (inf.ComFlag == 1) {
			byteSize = inf.Fsize / 8;
			if (inf.Fsize % 8 != 0)
				byteSize++;

			fread(simTable, sizeof(unsigned int), 256, Arch);
		}

		if (strcmp(argv[j], inf.Name) == 0) //если именя совпали
		{
			FILE *temparch;

			//выделили новое имя
			tmpnam(newname);
			cutname(newname);
			//создали временный архив
			temparch = fopen(newname, "w+b");
			WriteSig(temparch);

			//указатель снова в начало архива				
			fseek(Arch, 4, SEEK_SET);
			size = 4;

			while (size < fullsize) {


				fread(&inf.Namelen, sizeof(unsigned short), 1, Arch);
				fread(inf.Name, sizeof(char), inf.Namelen + 1, Arch);
				fread(&inf.ComFlag, sizeof(short), 1, Arch);
				fread(&inf.Fsize, sizeof(unsigned long long), 1, Arch);
				byteSize = inf.Fsize;
				if (inf.ComFlag == 1) {
					byteSize = inf.Fsize / 8;
					if (inf.Fsize % 8 != 0)
						byteSize++;
					fread(simTable, sizeof(unsigned int), 256, Arch);
				}
				//записываем все не совпадающие файлы во временный архив
				if (strcmp(argv[j], inf.Name)) {

					fwrite(&inf.Namelen, sizeof(unsigned short), 1, temparch);
					fwrite(inf.Name, sizeof(char), inf.Namelen + 1, temparch);
					fwrite(&inf.ComFlag, sizeof(short), 1, temparch);
					fwrite(&inf.Fsize, sizeof(unsigned long long), 1, temparch);

					inf.buf = (char*)malloc(sizeof(char)*byteSize);
					fread(inf.buf, 1, byteSize, Arch);
					if (inf.ComFlag == 1)
						fwrite(simTable, sizeof(unsigned int), 256, temparch);

					fwrite(inf.buf, 1, byteSize, temparch);
					free(inf.buf);


					if (inf.ComFlag == 1)
						size += (SEEK_NEXT_TRUE + inf.Namelen + byteSize);
					else
						size += (SEEK_NEXT_FALSE + inf.Namelen + byteSize);
				}
				else {
					fseek(Arch, byteSize, SEEK_CUR);   //сдвигаемся на размер файла
					if (inf.ComFlag == 1)
						size += (SEEK_NEXT_TRUE + inf.Namelen + byteSize);
					else
						size += (SEEK_NEXT_FALSE + inf.Namelen + byteSize);
				}
			}
			fclose(temparch);
			return 1;
		}
		else {
			fseek(Arch, byteSize, SEEK_CUR);   //сдвигаемся на размер файла
			if (inf.ComFlag == 1)
				size += (SEEK_NEXT_TRUE + inf.Namelen + byteSize);
			else
				size += (SEEK_NEXT_FALSE + inf.Namelen + byteSize);
		}
	}
	return 0;
}

//вывод информации о сжатии для отладки
void pr(sym *symbols, int u) {
	for (int i = 0;i < u; i++)
		printf("символ %c - %d код %s\n", (unsigned char)symbols[i].ch, symbols[i].count, symbols[i].code);
	printf("%d количество структур \n ", u);
}

//Побочная функция для qsort
int comp(const void* a, const void* b)
{
	const sym* k = (const sym*)a;
	const sym* m = (const sym*)b;
	return ((m->count) - (k->count));
}
