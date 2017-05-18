#include "Header.h"

#define ERR_EXPANSION (printf("Файл не является архивом."))
#define ERR_INFILE (printf("Файл не найден. \n"))
#define ERR_NOTFOUND (printf("Файл Архива не найден."))
#define ERR_NOTFOUND_IN (printf("Файл не найден в архиве.\n"))
#define WARNING_EXISTENCE (printf("Файл с таким именем уже существует в архиве. Хотите перезаписать файл? Y/N\n"))
#define ERR_VOIDARG (printf("Программа архиватор для получения справки используйте аргумент h."))
#define WARNING_EMPTY (printf("Архив пуст."))
#define ERR_DELETE {(printf("Не удалось удалить врмененный файл.")); \
                        break;} 
#define ERR_CLOSE_FILE {(printf("Не удалось закрыть файл.")); \
                        break;}
#define HELP (printf("Программа архиватор.\nПорядок вызова программы: одна_издоступных_операций имя_архива.eue файл1 файл2 ...\nДоступные операции:\na - добавление файлов в архив\nx - извлечение фалов из архива\nd - удаление файлов из архива\nl - вывод информации о файлах содержащихся в архиве (имя и размер)"))