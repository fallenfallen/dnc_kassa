/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/
#include "addon_func.h"

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <iostream> 
#include <string.h>
#include <sstream>
#include <time.h>

using namespace std;

//===========================================================================//
char * itoa(int d)
{
  int r[20];
  int count = 0;
  char* str = NULL;

  if (d == 0)
  {
    str = (char *) calloc(2, sizeof(char));
    strcpy(str, "0");
    return str;
  }
  while( d > 0 )
  {
    r[count] = d % 10;
	d = (int ) d / 10;
	count++;
  }

  str = (char *) calloc (count + 1,sizeof(char));
  for (int i = 0; i < count; i++)
    *(str + i) = (char) (r[count - i - 1] + 48);
  return str;
}

//===========================================================================//
const char * ftoa(double s, int ndiget)
{

    
    ostringstream oss;
    oss << s;
    
    return oss.str().c_str();
    
}

//===========================================================================//
void itoa(int d, char * str)
{
    int r[8];
    int count = 0;
    
    if (d==0) {   strcat(str, "0");   return;    } 
    
    while (d>0)   {
	r[count] = d %10;
	d = (int ) d/10;
	count++;    }
    
    for (int i=0; i< count;++i)    *(str + i) = (char) (r[count - i - 1] + 48);
    *(str + count) = '\0';
}
//===========================================================================//
double Round (double Nmb)
{
  bool LessZero = false;
  double Res = 0;
  
  if (Nmb < 0)
     {
      LessZero = true;
      Nmb = -Nmb;
     }
  //-----(*)-----Zuskin-----14/06/2011-----
  //if(fmod(Nmb, 1) >= 0.5) 
  if(fmod(Nmb, 1) >= 0.499999999) 
  //---------------------------------------
    Res = ceil(Nmb - 0.000000001);
  else
    Res = floor(Nmb + 0.000000001);
  
  if (Res == 0) return 0;
  if (LessZero) return -Res;
  return Res; 
    
}
/*************************************************************************/
void strrs(char* str) {
   char* tmp = (char*) calloc( strlen(str), sizeof(char) );
   
   int k = 0;
   for (int i = 0; i < strlen(str); i++) {
      if ( ( *(str + i) != ' ' ) && ( *(str + i) != '\n' ) ) {    *(tmp + k) = *(str + i);    k++;    }
   }
   strcpy(str, tmp); 
   free(tmp);
}
/*************************************************************************/
// в качествве входящего параметра принимается полное имя - каталог + файл, 
// пустая строка считается не правильным путем
// имя файла не может быть пустым ( "/home/shonda/exchange/" - не правильно )
// 0 - файл существует и доступен
// 1 - существует каталог и доступен, файла нет = NULL !!!
// 2 - файл существует, но не доступен на запись
// 3 - каталог существует, но не доступен на запись
// -1 - неверный путь

int TestPath(const char* path)
{
    if ( path == NULL ) return -1;
    if ( strlen(path) == 0 ) return -1;
    
    if ( access(path, 02) == 0 ) return 0;
    if ( access(path, 00) == 0 && access(path, 02) != 0 ) return 2;
    
    char * dir_path;
    char path_not_const[strlen(path) + 1];
    strcpy(path_not_const, path);
    
    char* ptr = strrchr(path_not_const,'/');
    if (ptr == NULL)    {
	// пустое имя каталога, // путь правильный
	dir_path = (char*) calloc( 2, sizeof(char) );
	strcpy(dir_path, "");
    }
    
    if (ptr != NULL) {  
	if ( strlen(ptr) < 2 ) return -1;		// имя файла пустое
	int n =  strlen(path) - strlen(ptr) + 1 ;
	dir_path = (char*) calloc(n+1, sizeof(char));
	strncpy( dir_path, path, n );	
    } 
    
    int ret = -1;
    
    if ( access(dir_path, 00) == 0 && access(path, 02) != 0 ) ret = 3;
    if ( access(dir_path, 02) == 0 ) ret = 1;
    
    free(dir_path);
    return ret;    
    
}
/*******************************************************************/

//-----(+)-----Zuskin-----24/03/2011-----
/*************************************************************************/
int str_count_words(char* str) {
    int res = 0;
    bool prev_spc = true;
    for (int i=0; i<strlen(str); i++) {
	if (*(str + i) == 32) {
	    prev_spc = true;	    
	}
	else { // not space
	    if (prev_spc == true) {
		res++;
	    }
	    prev_spc = false;
	}    
    }
    return res;
}

char* str_get_word(char* str, int num) {
    
    int i = 0;
    int index = 0;
    int start_pos = 0;
    int end_pos = 0;
    bool prev_spc = true;
    char * res = NULL;
    for (i=0; i<strlen(str); i++) {
	if (*(str + i) == 32) {
	    if ((prev_spc == false) && (index == num)) {
	    end_pos = i;
	    break;
	    }
	    prev_spc = true;
	}
	else {
	    if (prev_spc == true) {
		index++;
		if (index == num) {
		    start_pos = i;		
		}
	    }
	    prev_spc = false;
	}
    }
    
    if (index == num) {
	if ( end_pos == 0 ) {
	    end_pos = i;
	}
	res = (char *) calloc (end_pos - start_pos + 1, sizeof(char));
	for (i = 0; i< (end_pos - start_pos); i++) {
	    *(res + i) = *(str + start_pos + i);
	}
    }
        
    return res;
}

/*************************************************************************/

char * str_current_dtime() {
 const struct tm* ttime;
 time_t ctm;
 time(&ctm);
 ttime = localtime(&ctm);
 char* tstr = (char*) calloc (21, sizeof(char));
 strftime(tstr, 20, "%d.%m.%Y %H:%M:%S", ttime);
 return tstr; 
}

/*************************************************************************/
