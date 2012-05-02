#ifndef ADDON_FUNCTIONS
#define ADDON_FUNCTIONS

using namespace std;
#include <string>

char * itoa(int d);
const char * ftoa(double s, int ndiget);

void itoa(int d, char* str);      

double Round (double Nmb);

void strrs(char* str);   // функция удаляет из строки пробелы и символ перехода на новую строку

int TestPath(const char* path);		//проверка на доступ и на существование файла и его каталога

//-----(+)-----Zuskin-----24/03/2011-----
int str_count_words(char* str); //функция возвращает количество слов, разделенных пробелами, в строке str
char* str_get_word(char* str, int num); //функция возвращает указатель (выделяется память!!!) на слово номер num из строки str

char* str_current_dtime(); // функция возвращает указатель на выделенную область памяти, содержащую строку с текущими датой/временем в формате "dd.mm.yyyy hh:mm:ss"
//---------------------------------------

#endif
