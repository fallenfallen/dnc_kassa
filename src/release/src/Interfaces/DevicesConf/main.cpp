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
#include <qapplication.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <qtextcodec.h>
#include <qmessagebox.h>
#include "newperecodirovka.h"
#include "configurations.h"
#include "scannertest.h"

#include "addon_func.h"
#include "const.h"
#include "setup_param.h"
#include "blocktimer.h"
#include "conf_const.h"
#include "dkbinput.h"
#include "bcskbinput.h"
#include "mcrkbinput.h"

using namespace std;

HWSRV  * hw;

KeyboardDevice * kbcode_scaner = NULL;
KeyboardDevice * kbcode_reader = NULL;

SP sp;
BlockTimer *bt;


QFileDialog *f_g;

int mcrklava;
int p;
int klava;
int Scanner;
int AutoScanner;
Configurations * w;
bool autoerror = false;
QString BarCode;
QTime t;
bool begin_timer_flag = true;
bool next_timer_flag = false;


QTextCodec * DefCodec;

    
QString  GetPar(int par_number)
{
    DefCodec = QTextCodec::codecForName(SYSCODEC);
   // puts("in main QString  GetPar(int par_number)");
    QString str,pref1,pref2,suf1,suf2;
    int b_ind = 0;
    int e_ind = 0;
    int kol1,kol2 = 0; 
    int i,k=0;
    bool pref1_fl = false, pref2_fl = false, suf1_fl = false, suf2_fl = false;
    
    QString Err;
     str = BarCode;   
   
     puts(str);
     kol1 = str.contains('[');
     kol2 = str.contains(']');
     if (kol1 != kol2 )
        { 
	  Err = QString("-3");
          return Err;
	} 
     if (( kol1 > 4) || (kol2 > 4))
       {
          Err = QString("-2");
          return Err; 
       }
     
    puts("'[' == ']'");   
    char* s_str;
    char* s_par; 
     s_str = (char*) calloc ((strlen(str))+1,sizeof(char));
     s_par = (char*) calloc (6,sizeof(char));
     strcpy(s_str,str);
     
     if ((kol1 == 1) || (kol2 == 1))
     {
   // определен всего один префикс или суффикс
	 b_ind = str.find( '[', e_ind );
	 e_ind = str.find( ']', e_ind ); 
	 for(i = 0; i<(e_ind-b_ind -1);i++ )
	     *(s_par+i) = *(s_str+b_ind+i+1);
	 if ((unsigned)e_ind < (strlen(str)/2))
	 {
	     pref1_fl = true;
	     pref1 = QString(s_par); 
	     pref2_fl = false;
	     suf1_fl = false;
	     suf2_fl = false;
	 }
	 else
	 {
	     suf1_fl = true;
	     suf1 = QString(s_par); 
	     pref2_fl = false;
	     pref1_fl = false;
	     suf2_fl = false;
	 }
     }
     else
     {
    
	 puts(s_str);
	 while (k < kol1)
	 { 
	     b_ind = str.find( '[', e_ind );
	     e_ind = str.find( ']', e_ind );
	     for(i = 0; i<(e_ind-b_ind -1);i++ )
		 *(s_par+i) = *(s_str+b_ind+i+1);	    
	     if ((unsigned)e_ind < (strlen(str)/2))
	     {
		 pref2_fl = true;
		 pref2 = QString(s_par); 
		 free(s_par);
		 s_par = (char*) calloc (6,sizeof(char));      
		 k++;
		 b_ind = str.find( '[', e_ind+1 );
		 e_ind = str.find( ']', e_ind+1 );
		 for(i = 0; i<(e_ind-b_ind -1);i++ )
		     *(s_par+i) = *(s_str+b_ind+i+1);    
		 if ((unsigned)e_ind < (strlen(str)/2))
		 { 
		     pref1_fl = true;
		     pref1 = pref2;
		     pref2 = QString(s_par);
		     k++;
		 } 
	     }
	     if ((unsigned)e_ind > (strlen(str)/2))
	     {
		 suf1 =   QString(s_par); 
		 suf1_fl = true;
		 free(s_par);
		 s_par = (char*) calloc (6,sizeof(char));	    
		 k++;
		 b_ind = str.find( '[', e_ind+1 );
		 e_ind = str.find( ']', e_ind+1 );
		 if ((b_ind == -1) || (e_ind == -1))
		     break;
		 for(i = 0; i<(e_ind-b_ind - 1);i++ )
		     *(s_par+i) = *(s_str+b_ind+i+1); 
		 suf2 =   QString(s_par); 
		 suf2_fl = true;
		 free(s_par);
		 s_par = (char*) calloc (6,sizeof(char));
		 k++;
	     } 
	 }
	// puts("after while");
     }
     free(s_str);
     free(s_par);   
    // puts("after free");
     switch (par_number)
       {
         case 1: { if (pref1_fl == false)
	                    { Err = QString("0"); return Err; }
	                 return pref1; }
         case 2: { if (pref2_fl == false)
	                    { Err = QString("0"); return Err; }
			 return pref2; }  
         case 3: { if (suf1_fl == false)
	                    { Err = QString("0"); return Err; }
			 return suf1; }     
         case 4: { if (suf2_fl == false)
	                    { Err = QString("0"); return Err; }
			 return suf2; }
       }  
     Err = QString("0");  
     return Err;
 }




void catch_sig(int sig_num)
{
	 cout<<"!!! in confGUI void catch_sig(int sig_num) "<<endl;
	 DefCodec = QTextCodec::codecForName(SYSCODEC);
	 unsigned int i = 0,k = 0;
	 QString pref1,pref2,suf1,suf2;
	// cout<<"catch_sig: "<<sig_num<<endl;
	 if (sig_num == SIGUSR1) 
	 {
		 // printf("sig_num =%d\n",sig_num);
		  sigset(sig_num, catch_sig);
		  if (hw->SigFromBCS() == 1)//приняли от сканера
		  {
				BarCode = "";
				bool barcodestart = false;
				char* buf = (char*)calloc(43,sizeof(char));
				//printf("hw->BCS_GetBarCode() = |%s|\n",hw->BCS_GetBarCode());
				strcpy(buf,hw->BCS_GetBarCode());
				//for (unsigned int h = 0; h<strlen(buf); h++)
				//	 printf("*(buf+%d) = %d\n",h,*(buf+h));
				for (i=0;i<strlen(buf);i++)
				{
					 if (*(buf + i)<0)
					 {
						  QString er_str;
						  QString er_id;
						  DeviceConfErrorText(29,er_str,er_id);
						  InterfaceErrorWin(er_str, er_id);
						  free(buf);
						  return;
					 }
				}
				for (i=0;i<strlen(buf);i++)
				{
					 if (((buf[i]>47) && (buf[i]<58)) || ((buf[i]>64) && (buf[i]<91)) || ((buf[i]>96) && (buf[i]<123)))
					 {
						  BarCode.append(buf[i]);
						  barcodestart = true;
					 }
					 else   
					 {
						  if ((barcodestart) &&(((buf[i+1]>47) && (buf[i+1]<58)) || ((buf[i+1]>64) && (buf[i+1]<91)) || ((buf[i+1]>96) && (buf[i+1]<123)))) //непечатаемые или символы кроме цифр и букв внутри штрих-кода
						  {
								BarCode.append(buf[i]);
								BarCode.append(buf[i+1]);
								i++;
						  }
						  else //префиксы и суффиксы выделяем
						  { 
								if (k<4)
								{
									 if ((((buf[i+1]>47) && (buf[i+1]<58)) || ((buf[i+1]>64) && (buf[i+1]<91)) || ((buf[i+1]>96) && (buf[i+1]<123)))|| 
										  //следующий символ обязательно буква или цифра 
										  (((buf[i+2]>47) && (buf[i+2]<58)) || ((buf[i+2]>64) && (buf[i+2]<91)) || ((buf[i+2]>96) && (buf[i+2]<123)))||
										  //следующий за следующим символ обязательно буква или цифра
										  ((((buf[i-1]>47) && (buf[i-1]<58)) || ((buf[i-1]>64) && (buf[i-1]<91)) || ((buf[i-1]>96) && (buf[i-1]<123))) && barcodestart)||
										  //предыдущий символ обязательно буква или цифра
										  ((((buf[i-2]>47) && (buf[i-2]<58)) || ((buf[i-2]>64) && (buf[i-2]<91)) || ((buf[i-2]>96) && (buf[i-2]<123))) && barcodestart))
									 {
										  BarCode+=" [" + QString::number(buf[i],10) + "] "; 
										  k++;  // количество префисков и суффиксов
									 }
								}
						  }
					 }
				}
				free (buf); 
				w->st_p->BarCodetextLabel->setText(BarCode);
		  }
		  if (hw->SigFromMCR() == 1)
		  {
				//puts("hw->GetDeviceType() == 3");
				//запустить таймер.
				//если между таймерами > 200msec очищать все строки

				if (next_timer_flag == true)
				{
					 int n = t.restart();
					 if (n>200)
					 {
						  w->mt_p->MCRCode1textLabel->setText("");
						  w->mt_p->MCRCode2textLabel->setText("");
						  w->mt_p->MCRCode3textLabel->setText("");
					 }
				}
				if (begin_timer_flag == true)
				{
					 next_timer_flag = true;
					 t.start();
					 begin_timer_flag = false;
				}

				QString first_str = hw->MCR_GetFirstTrack();
				// printf("first_str = %s\n",first_str.ascii());
				QString second_str = hw->MCR_GetSecondTrack();
				//printf("second_str = %s\n",second_str.ascii());
				QString third_str = hw->MCR_GetThirdTrack();
				//printf("third_str = %s\n",third_str.ascii());
	     
				QString pref  = ";";
				QString suf =  "?";
				QString info;    
				if (first_str !="")
				{
					 w->mt_p->MCRCode1textLabel->setText(first_str);	
					 info = pref  +first_str + suf;
				}
				else{
					 w->mt_p->MCRCode1textLabel->clear();	
				}
				pref = "%";
				suf = "?";
				if (second_str !="")
				{
					 w->mt_p->MCRCode2textLabel->setText(second_str);
					 info += pref  + second_str + suf;
				}
				else{
					 w->mt_p->MCRCode2textLabel->clear();	
				}
				pref ="_";
				suf = "?";
				if (third_str !="")
				{
					 w->mt_p->MCRCode3textLabel->setText(third_str);
					 info += pref  + third_str + suf;
				}
				else{
					 w->mt_p->MCRCode3textLabel->clear();	
				}
				w->mt_p->MCRInfo_textLabel->setText(info);  
	
		  }
		  // ELECTRONIC WEIGHTER SIGNAL
		  if (hw->SigFromEW()==1)
		  {
				//puts("Signal from EW!");
				if (w->ew_t->isActiveWindow()) 
				{ 
					 w->ew_t->ShowWeight();
				}
		  }		
	 }
	 if (sig_num == SIGSEGV)       
	 {
		  puts("get SIGSEGV in DeviceConf");
		  sigset(SIGSEGV,SIG_DFL); 
	 }  
}

void quit()
{
	 //  puts("IN QUIT");
	 if (hw->BCS_GetPid() > 0)
		  kill(hw->BCS_GetPid(),SIGTERM); 
	 if (hw->MCR_GetPid() > 0)
		  kill(hw->MCR_GetPid(),SIGTERM); 
	 if (hw->DSP_GetPid() > 0)
		  kill(hw->DSP_GetPid(),SIGTERM); 
}


int main( int argc, char ** argv )
{   
	 hw = new HWSRV(1); 
	 int error = hw->ErrNum;
	 QApplication a (argc, argv);
	 QDesktopWidget * d = a.desktop();
	 int resolution_width = d->width();  
	 int resolution_heigth = d->height();
	 w = new Configurations; 
	 int wid = w->width(); 
	 int heig = w->height();
	 if ((wid>resolution_width)||(heig>resolution_heigth)){
		  w->setWindowState(Qt::WindowFullScreen);
		  w->setMinimumSize(QSize(resolution_width,resolution_heigth));
		  w->setMaximumSize(QSize(resolution_width,resolution_heigth));
		  w->adjustSize();
	 }
	 w -> show();
	 a. processEvents();
	 a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
	 sigset(SIGUSR1,catch_sig);
	 if (error>2){
		  QString er_str;
		  QString er_id;
		  HWSRVErrorText(error,er_str,er_id);
		  InterfaceErrorWin(er_str, er_id);
		  return error;
	 }
	 int ret = a.exec(); 
	 return ret;
}
