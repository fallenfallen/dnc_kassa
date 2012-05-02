/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2009, Authors: Savitskaya Alexandra, Pavlova Irina(Pavlova_Irina@shtrih-s.ru), Sukhanova Anna (anuta.s13@gmail.com), Savrasov Alexandr.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2009, Авторы: Савицкая Александра, Павлова Ирина(Pavlova_Irina@shtrih-s.ru), Суханова Анна (anuta.s13@gmail.com), Саврасов Александр.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "encoding.h"


unsigned char char_koi8r_to_cp1251(unsigned char InCh)
{
     if((unsigned char)InCh > 0xE0 && (unsigned char)InCh < 0xE3)
        return (unsigned char)InCh - 33;
     if((unsigned char)InCh > 0xE3 && (unsigned char)InCh < 0xE6)
        return (unsigned char)InCh - 32;
     if((unsigned char)InCh > 0xE8 && (unsigned char)InCh < 0xF1)
        return (unsigned char)InCh - 33;
     if((unsigned char)InCh > 0xF1 && (unsigned char)InCh < 0xF6)
        return (unsigned char)InCh - 34;
     if((unsigned char)InCh > 0xC0 && (unsigned char)InCh < 0xC3)
        return (unsigned char)InCh + 31;
     if((unsigned char)InCh > 0xC3 && (unsigned char)InCh < 0xC6)
        return (unsigned char)InCh + 32;
     if((unsigned char)InCh > 0xC8 && (unsigned char)InCh < 0xD1)
        return (unsigned char)InCh + 31;
     if((unsigned char)InCh > 0xD1 && (unsigned char)InCh < 0xD6)
        return (unsigned char)InCh + 30;
     if((unsigned char)InCh == 0xF7)
        return 0xC2;
     if((unsigned char)InCh == 0xE7)
        return 0xC3;
     if((unsigned char)InCh == 0xF6)
        return 0xC6;
     if((unsigned char)InCh == 0xFA)
        return 0xC7;
     if((unsigned char)InCh == 0xE6)
        return 0xD4;
     if((unsigned char)InCh == 0xE8)
        return 0xD5;
     if((unsigned char)InCh == 0xE3)
        return 0xD6;
     if((unsigned char)InCh == 0xFE)
        return 0xD7;
     if((unsigned char)InCh == 0xFB)
        return 0xD8;
     if((unsigned char)InCh == 0xFD)
        return 0xD9;
     if((unsigned char)InCh == 0xFF)
        return 0xDA;
     if((unsigned char)InCh == 0xF9)
        return 0xDB;
     if((unsigned char)InCh == 0xF8)
        return 0xDC;
     if((unsigned char)InCh == 0xFC)
        return 0xDD;
     if((unsigned char)InCh == 0xE0)
        return 0xDE;
     if((unsigned char)InCh == 0xF1)
        return 0xDF;
     if((unsigned char)InCh == 0xD7)
        return 0xE2;
     if((unsigned char)InCh == 0xC7)
        return 0xE3;
     if((unsigned char)InCh == 0xD6)
        return 0xE6;
     if((unsigned char)InCh == 0xDA)
        return 0xE7;
     if((unsigned char)InCh == 0xC6)
        return 0xF4;
     if((unsigned char)InCh == 0xC8)
        return 0xF5;
     if((unsigned char)InCh == 0xC3)
        return 0xF6;
     if((unsigned char)InCh == 0xDE)
        return 0xF7;
     if((unsigned char)InCh == 0xDB)
        return 0xF8;
     if((unsigned char)InCh == 0xDD)
        return 0xF9;
     if((unsigned char)InCh == 0xDF)
        return 0xFA;
     if((unsigned char)InCh == 0xD9)
        return 0xFB;
     if((unsigned char)InCh == 0xD8)
        return 0xFC;
     if((unsigned char)InCh == 0xDC)
        return 0xFD;
     if((unsigned char)InCh == 0xC0)
        return 0xFE;
     if((unsigned char)InCh == 0xD1) 
        return 0xFF;
     if((unsigned char)InCh == 0x4E) 
        return 0xB9;
     return InCh;
}

unsigned char char_cp1251_to_koi8r(unsigned char InCh)
{
     if((unsigned char)InCh > 0xBF && (unsigned char)InCh < 0xC2)
        return (unsigned char)InCh + 33;
     if((unsigned char)InCh > 0xC3 && (unsigned char)InCh < 0xC6)
        return (unsigned char)InCh + 32;
     if((unsigned char)InCh > 0xC7 && (unsigned char)InCh < 0xD0)
        return (unsigned char)InCh + 33;
     if((unsigned char)InCh > 0xCF && (unsigned char)InCh < 0xD4)
        return (unsigned char)InCh + 34;
     if((unsigned char)InCh > 0xDF && (unsigned char)InCh < 0xE2)
        return (unsigned char)InCh - 31;
     if((unsigned char)InCh > 0xE3 && (unsigned char)InCh < 0xE6)
        return (unsigned char)InCh - 32;
     if((unsigned char)InCh > 0xE7 && (unsigned char)InCh < 0xF0)
        return (unsigned char)InCh - 31;
     if((unsigned char)InCh > 0xEF && (unsigned char)InCh < 0xF4)
        return (unsigned char)InCh - 30;

     if((unsigned char)InCh == 0xC2)
        return 0xF7;
     if((unsigned char)InCh == 0xC3)
        return 0xE7;
     if((unsigned char)InCh == 0xC6)
        return 0xF6;
     if((unsigned char)InCh == 0xC7)
        return 0xFA;
     if((unsigned char)InCh == 0xD4)
        return 0xE6;
     if((unsigned char)InCh == 0xD5)
        return 0xE8;
     if((unsigned char)InCh == 0xD6)
        return 0xE3;
     if((unsigned char)InCh == 0xD7)
        return 0xFE;
     if((unsigned char)InCh == 0xD8)
        return 0xFB;
     if((unsigned char)InCh == 0xD9)
        return 0xFD;
     if((unsigned char)InCh == 0xDA)
        return 0xFF;
     if((unsigned char)InCh == 0xDB)
        return 0xF9;
     if((unsigned char)InCh == 0xDC)
        return 0xF8;
     if((unsigned char)InCh == 0xDD)
        return 0xFC;
     if((unsigned char)InCh == 0xDE)
        return 0xE0;
     if((unsigned char)InCh == 0xDF)
        return 0xF1;
     if((unsigned char)InCh == 0xE2)
        return 0xD7;
     if((unsigned char)InCh == 0xE3)
        return 0xC7;
     if((unsigned char)InCh == 0xE6)
        return 0xD6;
     if((unsigned char)InCh == 0xE7)
        return 0xDA;
     if((unsigned char)InCh == 0xF4)
        return 0xC6;
     if((unsigned char)InCh == 0xF5)
        return 0xC8;
     if((unsigned char)InCh == 0xF6)
        return 0xC3;
     if((unsigned char)InCh == 0xF7)
        return 0xDE;
     if((unsigned char)InCh == 0xF8)
        return 0xDB;
     if((unsigned char)InCh == 0xF9)
        return 0xDD;
     if((unsigned char)InCh == 0xFA)
        return 0xDF;
     if((unsigned char)InCh == 0xFB)
        return 0xD9;
     if((unsigned char)InCh == 0xFC)
        return 0xD8;
     if((unsigned char)InCh ==0xFD )
        return 0xDC;
     if((unsigned char)InCh == 0xFE)
        return 0xC0;
     if((unsigned char)InCh == 0xFF)
        return 0xD1;
     if((unsigned char)InCh == 0xB9)
        return 0x4E;
	
     return InCh;
}

unsigned char char_ibm866_to_cp1251(unsigned char InCh)
{
    if (InCh > 0x7F && InCh < 0xB0)
	return InCh + 64;
    if (InCh > 0xDF && InCh < 0xF0)
	return InCh + 16;
    if (InCh == 0xF0)
	return 0xA8;
    if (InCh == 0xF1)
	return 0xB8;
    return InCh;
}

unsigned char char_cp1251_to_ibm866(unsigned char InCh)
{
    if ((unsigned char)InCh > 0xBF && (unsigned char)InCh < 0xF0)
	return (unsigned char)InCh - 64;
    if ((unsigned char)InCh > 0xEF)
	return (unsigned char)InCh - 16;
    if ((unsigned char)InCh == 0x88) // evro
	return 0xF2;
    if ((unsigned char)InCh == 0xA8) // yo little
        return 0xF0;
    if ((unsigned char)InCh == 0xB8) // yo big
	return 0xF1;
    if ((unsigned char)InCh == 0xB9) // Number sign
	return 0x24;
    if ((unsigned char)InCh == 0x24) // Dollar sign
	return 0xFC;
    return (unsigned char)InCh;
}

void str_koi8r_to_cp1251(char * InStr)
{
  int len = strlen(InStr);
  char * buf = (char *) calloc(len + 1, sizeof(char));
  for(int i = 0; i < len; i++)
    *(buf + i) = (char) char_koi8r_to_cp1251((unsigned char) *(InStr + i));
  strcpy(InStr, buf);
  free(buf);
}

void str_cp1251_to_koi8r(char * InStr)
{
  int len = strlen(InStr);
  char * buf = (char *) calloc(len + 1, sizeof(char));
  for(int i = 0; i < len; i++)
    *(buf + i) = (char) char_cp1251_to_koi8r((unsigned char) *(InStr + i));
  strcpy(InStr, buf);
  free(buf);
}

void str_ibm866_to_cp1251(char * InStr)
{
  int len = strlen(InStr);
  char * buf = (char *) calloc(len + 1, sizeof(char));
  for(int i = 0; i < len; i++)
    *(buf + i) = (char) char_ibm866_to_cp1251((unsigned char) *(InStr + i));
  strcpy(InStr, buf);
  free(buf);
}

void str_cp1251_to_ibm866(char * InStr)
{
  int len = strlen(InStr);
  char * buf = (char *) calloc(len + 1, sizeof(char));
  for(int i = 0; i < len; i++)
    *(buf + i) = (char) char_cp1251_to_ibm866((unsigned char) *(InStr + i));
  strcpy(InStr, buf);
  free(buf);
}

