#include "Header.h"
#include "Errors.h"

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");

	FILE *ArchName;
	int j = 3;

	if (argc == 1) {		
		HELP;
		return 0;
	}
	
	/*Нужно прописать хелп*/
	if (argc == 2) {
		if (*argv[1] == 'h')
			HELP;	
		else {
			ERR_VOIDARG;
		}
		return 0;
	}		

	if (argc > 2) {

		switch (*argv[1]) {
		case 'a': {

			if (ChekingArch(argv, 2, strlen(argv[2]) - 1)) {
				ERR_EXPANSION;
				return -1;
			}
			if ((ArchName = fopen(argv[2], "rb")) == NULL) {				
				ArchName = fopen(argv[2], "w+b");
				WriteSig(ArchName);
				while (argv[j]) {
					AddFileToArch(ArchName, argv, j);
					j++;
				}
				fclose(ArchName);
			}
			else {				
				
				if (ChekingSign(argv[2])) {				
					ERR_EXPANSION;
					return -1;
				}	
				fclose(ArchName);				
				AddToArch(argv);								
			}
			break;
		}

		case 'x': {

			if (ChekingArch(argv, 2, strlen(argv[2]) - 1)) {
				ERR_EXPANSION;
				return -1;
			}

			if ((fopen(argv[2], "rb")) == NULL) {
				ERR_NOTFOUND;
				return -1;
			}

			if (ChekingSign(argv[2])) {
				ERR_EXPANSION;
				return -1;
			}
			ArchName = fopen(argv[2], "rb");

			if (argc == 3) {
				ExtractAll(ArchName);
			}
			else
				extract(ArchName, argv);

			fclose(ArchName);
			break;
		}
		case 'l': {

			if (ChekingArch(argv, 2, strlen(argv[2]) - 1)) {
				ERR_EXPANSION;
				return -1;
			}

			if ((fopen(argv[2], "rb")) == NULL) {
				ERR_NOTFOUND;
				return -1;
			}

			if (ChekingSign(argv[2])) {			
				ERR_EXPANSION;
				return -1;
			}
			ArchName = fopen(argv[2], "rb");
			getinf(ArchName);
			fclose(ArchName);
			break;
		}
		case 'd': {

			if (ChekingArch(argv, 2, strlen(argv[2]) - 1)) {
				ERR_EXPANSION;
				return -1;
			}

			if ((ArchName = fopen(argv[2], "rb")) == NULL) {
				ERR_NOTFOUND;
				return -1;
			}

			if (ChekingSign(argv[2])) {			
				ERR_EXPANSION;
				return -1;
			}		
			fclose(ArchName);
			RemoveFile(argv);
			break;
		}
		default: ERR_VOIDARG;
		}
	}
	
	return 0;
}