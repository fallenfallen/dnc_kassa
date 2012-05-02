//*******************************************************************************//
//*** ��������� ������ ������ ������ �������***//
//***************************** KOI8R  <--> CP1251 ******************************//
//***************************** CP1251 <--> IBM866 ******************************//
//*******************************************************************************//

#include <string.h>

unsigned char char_koi8r_to_cp1251(unsigned char InCh);
unsigned char char_cp1251_to_koi8r(unsigned char InCh);
unsigned char char_ibm866_to_cp1251(unsigned char InCh);
unsigned char char_cp1251_to_ibm866(unsigned char InCh);

//����!!! ������� � ���� ���� ���� ���
void str_koi8r_to_cp1251(char * InStr);
void str_cp1251_to_koi8r(char * InStr);
void str_ibm866_to_cp1251(char * InStr);
void str_cp1251_to_ibm866(char * InStr);

