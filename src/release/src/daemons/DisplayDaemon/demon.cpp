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
#include "../../../bin/include/hwsrv.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#define SCROLL_WAY "/tmp/stop_scroll.flg"

int main(void)
{ 
  
  int pid;
  pid = fork();
  switch (pid){
    case 0:
    {
      setsid();//создание новой группы процессов
      chdir("/");//делаем демона текущим каталогом
      close(0);//закрываем файловые дескрипторы стандартного ввода-вывода
      close(1);
      close(2);
      HWSRV *hw = new HWSRV(1);
      hw->DSP_ReadConf();
      if (hw->DSP_GetType()==0) //если настроено не использовать дисплей, то выходим 
           {
	      delete hw;
	      return 0;
	    }
      int i = hw->DSP_Start();
      if (i==0)
      {
        hw->DSP_Up(hw->DSP_DemonUpString());
        hw->DSP_Down(hw->DSP_DemonDownString());
	break;
      }
      else
       return -1;
    }
    case -1:
    {
      printf("DisplayDemon Error: unable to fork\n");
      break;
    }
    default:
    {
      //printf("Success: process %d went to background\n", pid);
      break;
    }
  }
return 0;
}
