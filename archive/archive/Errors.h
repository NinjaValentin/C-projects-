#include "Header.h"

#define ERR_EXPANSION (printf("���� �� �������� �������."))
#define ERR_INFILE (printf("���� �� ������. \n"))
#define ERR_NOTFOUND (printf("���� ������ �� ������."))
#define ERR_NOTFOUND_IN (printf("���� �� ������ � ������.\n"))
#define WARNING_EXISTENCE (printf("���� � ����� ������ ��� ���������� � ������. ������ ������������ ����? Y/N\n"))
#define ERR_VOIDARG (printf("��������� ��������� ��� ��������� ������� ����������� �������� h."))
#define WARNING_EMPTY (printf("����� ����."))
#define ERR_DELETE {(printf("�� ������� ������� ���������� ����.")); \
                        break;} 
#define ERR_CLOSE_FILE {(printf("�� ������� ������� ����.")); \
                        break;}
#define HELP (printf("��������� ���������.\n������� ������ ���������: ����_�����������_�������� ���_������.eue ����1 ����2 ...\n��������� ��������:\na - ���������� ������ � �����\nx - ���������� ����� �� ������\nd - �������� ������ �� ������\nl - ����� ���������� � ������ ������������ � ������ (��� � ������)"))