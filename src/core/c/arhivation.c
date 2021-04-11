#include "Header.h"
#include "Errors.h"

void addFileToArch(FILE *Arch, char **argv, int j)
{
	FILE *InFile;
	caption inf;
	unsigned long long SizePos, FlagPos, tmp;

	FlagPos = 0;
	SizePos = 0;
	inf.Fsize = 0;
	inf.ComFlag = 0;

	if ((InFile = (fopen(argv[j], "rb"))) == NULL)
	{
		ERR_INFILE;
		puts(argv[j]);
		// return -1;
		return;
	}
	fseek(Arch, 0, SEEK_END);
	//tmp = ftell(Arch);
	inf.Namelen = strlen(argv[j]);

	strcpy(inf.Name, argv[j]);

	fwrite(&inf.Namelen, sizeof(unsigned short), 1, Arch);
	fwrite(inf.Name, sizeof(char), inf.Namelen + 1, Arch);

	FlagPos = ftell(Arch);
	fwrite(&inf.ComFlag, sizeof(short), 1, Arch);
	SizePos = ftell(Arch);
	fseek(InFile, 0, SEEK_END);
	inf.Fsize = ftell(InFile);
	rewind(InFile);
	fwrite(&inf.Fsize, sizeof(unsigned long long), 1, Arch);

	if (inf.Fsize > 512)
	{
		inf.ComFlag = 1;

		inf.Fsize = compression(InFile, Arch);
		fseek(Arch, FlagPos, SEEK_SET);
		fwrite(&inf.ComFlag, sizeof(short), 1, Arch);


		fseek(Arch, SizePos, SEEK_SET);
		fwrite(&inf.Fsize, sizeof(unsigned long long), 1, Arch);
	}
	else
	{
		char *buf = (char *)malloc(sizeof(char) * inf.Fsize);
		fread(buf, sizeof(char), inf.Fsize, InFile);
		fwrite(buf, sizeof(char), inf.Fsize, Arch);
		free(buf);
	}
	fseek(Arch, 0, SEEK_END);
	//tmp = ftell(Arch);
	fclose(InFile);
}

void removeFile(char **argv)
{
	FILE *NewArch;
	int flag, j;
	char NewName[1000];

	j = 3;
	while (argv[j])
	{
		NewArch = fopen(argv[2], "rb");
		flag = findSameNameF(NewArch, argv, j, NewName);
		switch (flag)
		{
		case 0:
		{
			ERR_NOTFOUND_IN;
			j++;
			break;
		}
		case 1:
		{
			if (fclose(NewArch) != 0)
				printf("err");
			if (remove(argv[2]) != 0)
				printf("err");
			rename(NewName, argv[2]);
			NewArch = fopen(argv[2], "rb");
			j++;
			break;
		}
		}
		fclose(NewArch);
	}
}

void extract(FILE *Arch, char **argv)
{
	FILE *OutFile;
	caption inf;
	int j;
	unsigned long long fullsize, size, bytesize;
	unsigned int symtable[256] = {0};

	j = 3;
	fullsize = 0;
	size = 4;				  
	fseek(Arch, 0, SEEK_END); 
	fullsize += ftell(Arch);  
	fseek(Arch, 4, SEEK_SET);

	while (argv[j])
	{
		while (size < fullsize)
		{
			fread(&inf.Namelen, sizeof(unsigned short), 1, Arch);
			fread(inf.Name, sizeof(char), inf.Namelen + 1, Arch);
			fread(&inf.ComFlag, sizeof(short), 1, Arch);
			fread(&inf.Fsize, sizeof(unsigned long long), 1, Arch);
			bytesize = inf.Fsize;
			if (inf.ComFlag == 1)
			{
				bytesize = inf.Fsize / 8;
				if (inf.Fsize % 8 != 0)
					bytesize++;
				fread(symtable, sizeof(unsigned int), 256, Arch);
			}
			if (strcmp(argv[j], inf.Name) == 0)
			{
				OutFile = fopen(argv[j], "wb");
				inf.buf = (unsigned char *)malloc(sizeof(unsigned char) * bytesize);
				fread(inf.buf, 1, bytesize, Arch);
				if (inf.ComFlag == 1)
				{
					decoding(OutFile, inf.buf, symtable, inf.Fsize);
				}
				else
					fwrite(inf.buf, sizeof(char), bytesize, OutFile);
				free(inf.buf);
				fclose(OutFile);
				break;
			}
			else
			{
				fseek(Arch, bytesize, SEEK_CUR);
				if (inf.ComFlag == 1)
				{
					size += (SEEK_NEXT_TRUE + inf.Namelen + bytesize);
				}
				else
				{
					size += (SEEK_NEXT_FALSE + inf.Namelen + bytesize);
				}
				if (size >= fullsize)
				{
					ERR_NOTFOUND_IN;
					puts(argv[j]);
					break;
				}
			}
		}
		j++;
		fseek(Arch, 4, SEEK_SET); 
		size = 4;				 
	}
}

void addToArch(char **argv)
{

	FILE *NewArch;
	FILE *InFile;
	int flag, j;
	char newname[1000];

	j = 3;

	while (argv[j])
	{
		if ((InFile = (fopen(argv[j], "rb"))) == NULL)
		{
			ERR_INFILE;
			puts(argv[j]);
			j++;
		}
		else
		{
			NewArch = fopen(argv[2], "r+b");
			flag = findSameNameF(NewArch, argv, j, newname);
			switch (flag)
			{
			case 0:
			{
				addFileToArch(NewArch, argv, j);
				j++;
				break;
			}
			case 1:
			{
				if (fclose(NewArch) != 0)
					ERR_CLOSE_FILE
				if (remove(argv[2]) != 0)
					ERR_DELETE
				rename(newname, argv[2]);
				NewArch = fopen(argv[2], "r+b");
				addFileToArch(NewArch, argv, j);
				j++;
				break;
			}
			}
			fclose(NewArch);
		}
	}
}

void getinf(FILE *Arch)
{
	caption inf;
	int num;
	unsigned long long fullsize, size, bytesize;
	unsigned short symtable[256] = {0};
	fullsize = 0;
	size = 4;
	num = 0;

	fseek(Arch, 0, SEEK_END);
	fullsize += ftell(Arch);
	fseek(Arch, 4, SEEK_SET);

	if (size >= fullsize)
	{
		WARNING_EMPTY;
		return;
	}

	while (size < fullsize)
	{
		fread(&inf.Namelen, sizeof(unsigned short), 1, Arch);
		fread(inf.Name, sizeof(char), inf.Namelen + 1, Arch);
		fread(&inf.ComFlag, sizeof(short), 1, Arch);
		fread(&inf.Fsize, sizeof(unsigned long long), 1, Arch);

		num++;
		printf("%d) ", num);
		puts(inf.Name);

		bytesize = inf.Fsize;
		if (inf.ComFlag == 1)
		{
			printf("The file is compressed\n");
			bytesize = inf.Fsize / 8;
			if (inf.Fsize % 8 != 0)
				bytesize++;
		}
		else
		{
			printf("The file is not compressed.\n");
		}
		printf("%llu Bytes \n", bytesize);

		if (inf.ComFlag == 1)
		{
			// move pointer to file size
			size += (SEEK_NEXT_TRUE + inf.Namelen + bytesize);
			fseek(Arch, bytesize + 1024, SEEK_CUR);
		}
		else
		{
			size += (SEEK_NEXT_FALSE + inf.Namelen + bytesize);
			fseek(Arch, bytesize, SEEK_CUR);
		}
	}
}

void extractAll(FILE *Arch)
{
	FILE *OutFile;
	caption inf;
	int j;
	unsigned long long fullsize, size, bytesize;
	unsigned int symtable[256] = {0};

	fullsize = 0;
	size = 4;				 
	fseek(Arch, 0, SEEK_END);
	fullsize += ftell(Arch); 
	fseek(Arch, 4, SEEK_SET);

	while (size < fullsize)
	{
		fread(&inf.Namelen, sizeof(unsigned short), 1, Arch);
		fread(inf.Name, sizeof(char), inf.Namelen + 1, Arch);
		fread(&inf.ComFlag, sizeof(short), 1, Arch);
		fread(&inf.Fsize, sizeof(unsigned long long), 1, Arch);
		bytesize = inf.Fsize;
		if (inf.ComFlag == 1)
		{
			bytesize = inf.Fsize / 8;
			if (inf.Fsize % 8 != 0)
				bytesize++;
			fread(symtable, sizeof(unsigned int), 256, Arch);
		}
		OutFile = fopen(inf.Name, "wb");
		inf.buf = (unsigned char *)malloc(sizeof(unsigned char) * bytesize);
		fread(inf.buf, 1, bytesize, Arch);

		if (inf.ComFlag == 1)
		{
			size += (SEEK_NEXT_TRUE + inf.Namelen + bytesize);
			decoding(OutFile, inf.buf, symtable, inf.Fsize);
		}
		else
		{
			size += (SEEK_NEXT_FALSE + inf.Namelen + bytesize);
			fwrite(inf.buf, sizeof(char), bytesize, OutFile);
		}
		free(inf.buf);
		fclose(OutFile);
	}
}
