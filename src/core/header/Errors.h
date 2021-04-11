#include "Header.h"

#define ERR_EXPANSION (printf("File is not archive\n"))
#define ERR_INFILE (printf("File not found\n"))
#define ERR_NOTFOUND (printf("Archive file not found\n"))
#define ERR_NOTFOUND_IN (printf("File not found in archive\n"))
#define WARNING_EXISTENCE (printf("File with this name already exist in archive.\nDo you want to replace it? Y/N\n"))
#define ERR_VOIDARG (printf("Use flag h for help\n"))
#define WARNING_EMPTY (printf("Archive is empty\n"))
#define ERR_DELETE {(printf("Can't delete temporary files\n")); \
                        break;} 
#define ERR_CLOSE_FILE {(printf("Can't close file\n")); \
                        break;}
#define HELP (printf("USAGE: ./arch <arg> <arch_name.eue> <file1> <file2>...\nOperation:\na - add files to archive\nx - extract files\nd - delete files from archive\nl - info about files in archive"))