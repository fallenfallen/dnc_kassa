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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <syslog.h>
#include <iostream> 
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "conf_const.h"
using namespace std;

/*************************************************************/
int GetIntParam1(char * PName, int * Res, long POS, FILE * fp)
{
	// printf("GetIntParam1(%s, %d, %d)", PName, Res, POS);
	 int c;
	 int bi=0;
	 int MaxLen = strlen(PName);
	 bool Ind1 = false, Ind2 = false;
	 char *Buf;
	 char *Val;
	 Buf = (char *) calloc(MaxLen+1,sizeof(char));  
	 Val = (char *) calloc(11,sizeof(char));
	 fseek(fp,POS,SEEK_SET);
	 while ((c = fgetc(fp)) != EOF)
	 {
		  if (Ind1 == false)
		  {
				if (c==0x5B)
					 Ind1 = true; 
		  }
		  else
		  {
				if (Ind2 == false)
				{
	    
					 if (c == 0x5D) // c == ' ] ' 
					 {
						  if (bi==0)
						  {
								Ind1=false;
						  }
						  else
						  {
								*(Buf+bi)=0;
								if (!strcmp(PName, Buf))
								{
									 bi=0;
									 Ind2=true;
								}
								else
								{
									 Ind1=false;
									 bi=0;
								}     
						  }
					 }
					 else
					 {
						  if  (((c > 0x40) && (c < 0x5B))|| ((c > 47) && (c < 58)))
						  {
		   
								if (bi>MaxLen-1)
								{
									 bi=0;
									 Ind1 = false;
								}
								else
								{      
									 *(Buf+bi) = c;
									 bi++;
								}
						  }
					 }   
				}
				else //Ind2 == true !!!	  
				{
					 if (c==0x5B) // c == " [ "
					 {
						  if (bi==0)
						  {
								free(Val);
								free(Buf);
								return -2;  
						  }
						  else
						  {
		 
								if (bi<10)
									 *(Val+bi) = 0;
								*Res = 0;
								for (bi=strlen(Val)-1;bi>-1;bi--)
									 *Res = *Res + (*(Val+bi)-48)* ( (int) pow(10, strlen(Val)-bi-1) ) ; 
								free(Val);
								free(Buf);
								return 1;
						  }	 
					 }
					 else
					 {
						  if ( (c>47) && (c<58) ) 
						  {
								if (bi>10)
								{
									 free(Val);
									 free(Buf);
									 return -2;
								}
								*(Val+bi) = c;
								bi++;
						  }
					 }   
				}
		  }
	 }
	 if ((Ind1 == true) && (Ind2 == true) && (bi > 0))
	 {
		  *(Val+bi) = 0;
		  *Res = 0;
		  for (bi=strlen(Val)-1;bi>-1;bi--)
				*Res = *Res + (*(Val+bi)-48)* ( (int) pow(10, strlen(Val) - bi - 1) ) ; 
		  free(Val);
		  free(Buf); 
		  return 1;
	 }
	 free(Buf);
	 free(Val);  
	 return 0; // PARAMETER NOT FOUND !!!
}

/*************************************************************/
FILE * OpenFile(const char * FName)
{
	FILE * fp;
	fp = fopen(FName,"r");
	if (fp == NULL){
		 return 0;
	}
	return fp;
}

/*************************************************************/

int CloseFile(FILE * fp)
{
	 int res = 0;
	 res = fclose(fp);
	 return res;
}

/*************************************************************/

int GetSchemeParam(int SchemeNum, char * PName, int * Res, FILE * fp)
{
	 int c;
	 unsigned int bi = 0;
	 int kol = 0;
	 bool Ind1 = false, Ind2 = false;
	 unsigned int MaxLen = strlen(PName);
	 char * Buf = (char *) calloc(MaxLen+1,sizeof(char));
	 fseek(fp,0,SEEK_SET);
	 while ((c = fgetc(fp)) != EOF)
	 {
		  if (Ind1 == false)
		  {
				if (c==0x5B) 
					 Ind1 = true; 
		  }
		  else
		  {
				if (Ind2 == false)
				{
					 if (c == 0x5D) // c == ']'
					 {
						  if (bi==0)
						  {
								Ind1=false;
						  }
						  else
						  {
								*(Buf+bi)=0; 
								if (!strcmp("SCHEME", Buf))
								{
									 bi=0;
									 Ind2=true;
									 kol++;
									 if (kol != SchemeNum)
									 {
										  Ind1 = false;
										  Ind2 = false;
									 }
									 else
									 { 
										  free(Buf); 
										  long  POS = ftell(fp);
										  return GetIntParam1(PName, Res, POS, fp); 
									 } 
								}
								else
								{
									 Ind1=false;
									 bi=0;
								}     
						  }
					 }
					 else
					 {
						  if  ( (c > 0x40) && (c< 0x5B) )
						  {
		   
								if (bi>MaxLen-1)
								{
									 bi=0;
									 Ind1 = false;
								}
								else
								{      
									 *(Buf+bi) = c;
									 bi++;
								}
						  }
					 }  
				}
		  }
	 }   
	 free(Buf);
	 return 0; // PARAMETER NOT FOUND
}


/*************************************************************/

//выбирает из строки все символы внутри ^
int getStrParam(char * str,char * tmp_str)
{
	 bool flag = false;
	 int pos = 0;
	 for (unsigned int i  = 0;i<strlen(str);i++){
		  if (flag == false){
				if (*(str+i) == 94){
					 flag = true;
					 pos = i;
				}
		  }
		  else
		  {
				if (*(str+i) == 94) break;
				*(tmp_str+ i - pos-1) = *(str+i);
		  }
	 }  
	 return 0;
}

/*************************************************************/

int GetStrParam(char * PName, char * Res, unsigned int MaxResLen,long POS, FILE * fp)
{
	 //printf("int GetStrParam(%s, %s, %d, %d, %d)\n",PName, Res, MaxResLen,(int)POS, fp);
	 unsigned int bi=0;
	 int c;
	 unsigned int MaxLen = strlen(PName);
	 bool Ind1 = false, Ind2 = false, Ind3 = false;
	 char * Buf = (char *) calloc(MaxLen+1, sizeof(char));  
	 char * Val = (char *) calloc(MaxResLen+1, sizeof(char));
	 fseek(fp,POS,SEEK_SET);
	 while ((c = fgetc(fp)) != EOF)
	 {
		  if (Ind1 == false)
		  {
				if (c == 0x5B) // c == '['
					 Ind1 = true;   
		  }
		  else
		  {
				if (Ind2 == false)
				{
					 if (c == 0x5D) // c == ']'
					 {
						  if (bi==0)
						  {
								Ind1=false;
						  }
						  else
						  {
								*(Buf+bi)=0;
								if (strcmp(PName, Buf)==0)
								{
									 bi=0;
									 Ind2=true;
								}
								else
								{
									 Ind1=false;
									 bi=0;
								}     
						  }
					 }
					 else
					 {
						  if ((((c>47) && (c<58)) || (c == 32) || (c == 34)) ||
								(((c>191) && (c<256)) || (c == 163) || (c == 168) || (c == 184) || (c == 179) || (c == 32) || (c == 34)) ||
								(((c>64) && (c<91))|| ((c>96) && (c<124)) || (c == 32) || (c == 34)) ||
								(((c>34) && (c<48))|| ((c>58) && (c<63)) || (c == 65) || (((c>90) && (c<94)) && ((c>94) || (c<97))) || ((c>122) && (c<127)) || (c==33) || (c == 34))
								)
								{
								if (bi>MaxLen-1)
								{
									 bi=0;
									 Ind1 = false;
								}
								else
								{
									 //printf("*(Buf+%d) = %c\n",bi,*(Buf+bi));
									 *(Buf+bi) = c;
									 bi++;
								}
						  }
					 }
				}
				else //Ind2 == true !!!	  
				{
					 if (c==0x5B)// c == '['
					 {
						  if (bi==0)
						  {
								free(Val);
								free(Buf); 
								return -2;
						  }
						  else
						  {
		 
								if (Ind3==true) 
								{
									 *(Val+bi) = 0;
									 for (bi=0;bi<strlen(Val);bi++)
									 {
										  *(Res+bi)  = *(Val+bi);
									 }
									 free(Val);
									 free(Buf);  
									 return 1;
								}
								else
								{
									 free(Val);
									 free(Buf); 
									 return -2;
								}
						  }	 
					 }
					 else // we've got a symbol, that is not '['
					 {
						  if (Ind3 == true)
						  {
								if (
										  (((c>47) && (c<58)) ||
											(c == 32) || (c == 34)) ||

										  (((c>191) && (c<256)) || 
											(c == 163) || 
											(c == 168) || 
											(c == 184) || 
											(c == 179) || 
											(c == 32) || 
											(c == 34)) ||

										  (((c>64) && (c<91)) || 
											((c>96) && (c<124)) || 
											(c == 32) || (c == 34)) || 
										  
										  (((c>34) && (c<48))|| 
											((c>58) && (c<63)) || 
											(c == 65) || 
											(((c>90) && (c<94)) && ((c>94) || (c<97))) || 
											((c>122) && (c<127)) || 
											(c==33) || 
											(c == 34))
										  )
								{
									 if (bi>MaxResLen-1)
									 {
										  free(Val);
										  free(Buf);
										  return -2;
									 }
									 *(Val+bi) = c;
									 bi++;
								}
								if ( c == 0x5E ) // if we've got '^' (wich means the end of the value)
								{
									 if (bi == 0)
									 {
										  free(Val);
										  free(Buf); 
										  return -2;
									 }
									 else
									 {
										  if (bi<MaxResLen)
												*(Val+bi) = 0;
										  for (bi=0; bi < strlen(Val)+1;bi++){
												*(Res+bi)  = *(Val+bi);
										  }
										  free(Val);
										  free(Buf); 
										  return 1;
									 }
								}
						  }
						  else //Ind3 == false
						  {
								if (c == 0x5E) // if we've got '^' (wich means the beginning of the value)
								{
									 Ind3 = true;
								}
						  }
					 }
				}
		  }
	 }
	 if ((Ind1 == true) && (Ind2 == true) && (Ind3 == true) && (bi > 0))
	 {
		  *(Val+bi) = 0;
		  for (bi=0; bi < strlen(Val); bi++)
				*(Res+bi)  = *(Val+bi);
		  free(Val);
		  free(Buf); 
		  return 1;
	 }
	 free(Buf);
	 free(Val); 
	 return 0; // PARAMETER NOT FOUND !!!
}

/*************************************************************/

int GetStrSchemeParam(int SchemeNum,  char * PName,char * Res, FILE * fp)
{
	 //printf("int GetStrSchemeParam(%d,  %s, %s, %d)\n",SchemeNum, PName,Res, fp);
	 int c;
	 unsigned int  bi = 0;
	 int kol = 0;
	 bool Ind1 = false, Ind2 = false;
	 char *Buf;
	 char *Val;
	 char *way;
	 unsigned int MaxLen = 550;
	 Buf = (char *) calloc(MaxLen+1,sizeof(char));  
	 Val = (char *) calloc(11,sizeof(char));
	 way = (char*) calloc (100,sizeof(char));
	 strcpy(way,CONF_WAY);
	 fseek(fp,0,0);
	 while ((c = fgetc(fp)) != EOF)
	 {
		   //printf("!|%s|\n",fgets(Buf,MaxLen+1,fp));
		  if (Ind1 == false)
		  {
				if (c==0x5B) 
					 Ind1 = true;   
		  }
		  else
				if (Ind2 == false)
				{
				if (c == 0x5D) // c == ']'
				{
					 if (bi==0)
					 {
						  Ind1=false;
					 }
					 else
					 {
						  *(Buf+bi)=0;
		  
						  if (!strcmp("SCHEME", Buf))
						  {
								bi=0;
								Ind2=true;
								kol++;
								if (kol != SchemeNum)
								{
									 Ind1 = false;
									 Ind2 = false;
								}
								else
								{ 
									 free (Buf);
									 free(Val);
									 long  POS = ftell(fp);
									 return GetStrParam(PName, Res, MaxLen, POS, fp); 
								}
						  }
						  else
						  {
								Ind1=false;
								bi=0;
						  }     
					 }
				}
				else
				{
					 if (
								(((c>47) && (c<58)) || (c == 32) || (c == 34)) ||
								(((c>191) && (c<256)) || (c == 163) || (c == 168) || (c == 184) || (c == 179) || (c == 32) || (c == 34)) ||
								(((c>64) && (c<91))|| ((c>96) && (c<124)) || (c == 32) || (c == 34)) ||
								(((c>34) && (c<48))|| ((c>58) && (c<63)) || (c == 65) || (((c>90) && (c<94)) && ((c>94) || (c<97))) || ((c>122) && (c<127)) || (c==33) || (c == 34))
								) 
					 {
						  if (bi>MaxLen-1)
						  {
								bi=0;
								Ind1 = false;
						  }
						  else
						  {      
								*(Buf+bi) = c;
								bi++;
						  }
					 }
				}   
		  }
	 } 
	 free(Buf);
	 free(Val);
	 return 0;
}

/*************************************************************/

int GetIntParam(char * PName,  int * Res)
{ 
  FILE *fp;
  int c;
  int res = 0;
  int bi=0;
  int MaxLen = strlen(PName);
  bool Ind1 = false, Ind2 = false;
  char * Buf = (char *) calloc(MaxLen+1,sizeof(char));  
  char * Val = (char *) calloc(11,sizeof(char));
  fp = fopen(CONF_WAY,"r");
  //printf("---- const.cpp 5 open ( %s ) ------\n", CONF_WAY);	
 // printf("fp 5 const.cpp = %d\n",fp);
  if (fp == NULL)
  {
	free(Buf);
	free(Val);
	return -1;
  } 
  while ((c = fgetc(fp)) != EOF)
  {
    if (Ind1 == false)
       {
        if (c==0x5B)
	   Ind1 = true; 
       }
    else
       if (Ind2 == false)
    	    {
	    
	    if (c == 0x5D) // c == ' ] ' 
	       {
	       if (bi==0)
	          {
		  Ind1=false;
		  }
	       else
	          {
		  *(Buf+bi)=0;
		  if (!strcmp(PName, Buf))
		     {
		     bi=0;
		     Ind2=true;
		     }
		  else
		     {
		     Ind1=false;
		     bi=0;
   		     }     
		  }
	       }
	    else
	       {
	       if  (( (c > 0x40) && (c< 0x5B) )|| ((c > 47) && (c < 58)))
	           {
		   
		   if (bi>MaxLen-1)
		      {
		      bi=0;
		      Ind1 = false;
		      }
		   else
		      {      
		      *(Buf+bi) = c;
		      bi++;
		      }
		   }
	       }   
	    }
	else //Ind2 == true !!!	  
	   {
	   if (c==0x5B) // c == " [ "
	      {
	      if (bi==0)
	         {
		 free(Val);
		 free(Buf);
		 res = fclose(fp);
		//printf ("-------const.cpp 18 close (%d)------\n", fp);
		// printf("fclose(fp = %d) 5 const.cpp = %d\n",fp,res);
		 return -2;  
		 }
	      else
	         {
		 
		 if (bi<10)
         		 *(Val+bi) = 0;
		 *Res = 0;
		 for (bi=strlen(Val)-1;bi>-1;bi--)
		      *Res = *Res + (*(Val+bi)-48)* ( (int) pow(10, strlen(Val)-bi-1) ) ; 
		 free(Val);
		 free(Buf);
		 res = fclose(fp); 
		 //printf("-------const.cpp 19 close (%d)------\n", fp);
		// printf("fclose(fp = %d) 5 const.cpp = %d\n",fp,res);
		 return 1;
		 }	 
	      }
	   else
	      {
	      if ( (c>47) && (c<58) ) 
	         {
		 if (bi>10)
		    {
		    free(Val);
		    free(Buf);
		    res = fclose (fp); 
		//	 printf("-------const.cpp 20 close (%d)------\n", fp);	
		//   printf("fclose(fp = %d) 5 const.cpp = %d\n",fp,res);
		    return -2;
		    }
		 *(Val+bi) = c;
		 bi++;
		 }
	      
	      }      
	   
	   }
  }
  
if ((Ind1 == true) && (Ind2 == true) && (bi > 0))
   {
   *(Val+bi) = 0;
   *Res = 0;
   for (bi=strlen(Val)-1;bi>-1;bi--)
       *Res = *Res + (*(Val+bi)-48)* ( (int) pow(10, strlen(Val) - bi - 1) ) ; 
   free(Val);
   free(Buf); 
   res = fclose(fp);
	//printf("-------const.cpp 21 close (%d)------\n", fp);
  // printf("fclose(fp = %d) 5 const.cpp = %d\n",fp,res);
   return 1;
   }
free(Buf);
free(Val);  
res = fclose (fp); 
//printf("-------const.cpp 22 close (%d)------\n", fp);
//printf("fclose(fp = %d) 5 const.cpp = %d\n",fp,res);
return 0; // PARAMETER NOT FOUND !!!
}


/*************************************************************/

int GetCodesParam(char * PName, char * Res,unsigned int MaxResLen, int PNum)
{
 FILE *fp;
 int c;
 int res = 0;
 unsigned int bi=0;
 int i=1;
 unsigned int MaxLen = strlen(PName);
 bool Ind1 = false, Ind2 = false, Ind3 = false;
 char * Buf = (char *) calloc(MaxLen+1, sizeof(char)); 
 char * Val = (char *) calloc(MaxResLen+1, sizeof(char));
 fp = fopen(CODES_WAY,"r");
// printf("---- const.cpp 6 open ( %s ) ------\n", CODES_WAY);
 //printf("fp 6 const.cpp = %d\n",fp);
 if (fp == NULL)
 {
	  free(Buf);
	  free(Val);
	  return -1;
 } 
 while ((c = fgetc(fp)) != EOF)
 {
	  if (Ind1 == false)
	  {
			if (c==0x5B) 
			{
				 Ind1 = true; 
			}
	  }
	  else
			if (Ind2 == false)
			{
			if (c == 0x5D) // c == ']'
			{
				 if (bi==0)
				 {
					  Ind1=false;
				 }
				 else
				 {
					  *(Buf+bi)=0;
					  if (!strcmp(PName, Buf))
					  {
							bi=0;
							Ind2=true;
					  }
					  else
					  {
							Ind1=false;
							bi=0;
					  }     
				 }
			}
			else
			{
				 if  ( (c > 0x40) && (c< 0x5B) )
				 {
		   
					  if (bi>MaxLen-1)
					  {
							bi=0;
							Ind1 = false;
					  }
					  else
					  {      
							*(Buf+bi) = c;
							bi++;
					  }
				 }
			}   
	  }
	  else //Ind2 == true !!!	  
	  {
			if (c==0x5B)
			{
				 if (bi==0)
				 {
					  free(Val);
					  free(Buf);
					  res = fclose(fp);
				//	  printf("-------const.cpp 23 close (%d)------\n", fp);
				//	  printf("fclose(fp = %d) 6 const.cpp = %d\n",fp,res);
					  return -2;
				 }
				 else
				 {
					  if (Ind3==true) 
					  {
							*(Val+bi) = 0;
							for (bi=0;bi<strlen(Val);bi++)
								 *(Res+bi)  = *(Val+bi);
							free(Val);
							free(Buf); 
							res = fclose(fp);
					//		printf("-------const.cpp 24 close (%d)------\n", fp);
					//		printf("fclose(fp = %d) 6 const.cpp = %d\n",fp,res);
							return 1;
					  }
					  else
					  {
							free(Val);
							free(Buf);
							res = fclose(fp);
						//	printf("-------const.cpp 25 close (%d)------\n", fp);
						//	printf("fclose(fp = %d) 6 const.cpp = %d\n",fp,res);
							return -2;
					  }
				 }	 
			}
			else // we've got a symbol, that is not '['
			{
				 if (Ind3 == true)
				 {
	      
					  if ( c == 0x22 ) // if we've got '"' (wich means the end of the value)
					  {
							if (bi == 0)
							{
								 free(Val);
								 free(Buf);
								 res = fclose(fp);
							//	 printf("-------const.cpp 26 close (%d)------\n", fp);
							//	 printf("fclose(fp = %d) 6 const.cpp = %d\n",fp,res);
								 return -2;
							}
							else
							{
								 if (bi<MaxResLen)
									  *(Val+bi) = 0;
								 for (bi=0; bi < strlen(Val)+1;bi++)
									  *(Res+bi)  = *(Val+bi);
								 if (i==PNum)
								 {
									  free(Val);
									  free(Buf);
									  res = fclose(fp);
							//		  printf("-------const.cpp 27 close (%d)------\n", fp);
							//		  printf("fclose(fp = %d) 6 const.cpp = %d\n",fp,res);
									  return 1;
								 }
								 else
								 {
									  bi=0;
									  Ind3=false;
									  Ind2=false;
									  Ind1=false;
									  i++;
								 } 
							}
					  }
					  else
					  {
							if (bi>MaxResLen-1)
							{
								 free(Val);
								 free(Buf);
								 res = fclose(fp);
							//	 printf("-------const.cpp 28 close (%d)------\n", fp);
							//	 printf("fclose(fp = %d) 6 const.cpp = %d\n",fp,res);
								 return -2;
							}
							*(Val+bi) = c;
							bi++;
					  }
				 }
				 else //Ind3 == false
				 {
					  if (c == 0x22) // if we've got '"' (wich means the beginning of the value)
					  {
							Ind3 = true;
					  }
				 }
			}
	  }
 }
 if ((Ind1 == true) && (Ind2 == true) && (Ind3 == true) && (bi > 0))
 {
	  *(Val+bi) = 0;
	  for (bi=0; bi < strlen(Val); bi++)
			*(Res+bi)  = *(Val+bi);
	  free(Val);
	  free(Buf);
	  res = fclose(fp);
	//  printf("-------const.cpp 29 close (%d)------\n", fp);
	//  printf("fclose(fp = %d) 6 const.cpp = %d\n",fp,res);
	  return 1;
 }
 free(Buf);
 free(Val);
 res = fclose(fp);
 //printf("-------const.cpp 30 close (%d)------\n", fp);
// printf("fclose(fp = %d) 6 const.cpp = %d\n",fp,res);
 return 0; // PARAMETER NOT FOUND !!!
}

/*************************************************************/
int FileAcces(const char * file_path)
{
	 char * file_way = (char *)calloc(strlen(file_path)+1,sizeof(char));
	 strcpy(file_way,file_path);	
	 FILE * fp = 0;  
	 int res = 0;
	 int f = (S_IRWXU|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	 if ( access(file_way,F_OK) == -1 )  {
		  fp = fopen(file_way,"a+");
	//	  printf("---- const.cpp 7 open ( %s ) ------\n", file_way);
	//	  printf("fp 7 const.cpp = %d\n",fp);
		  if ( fp != 0 )		{
				chmod(file_way,f);
				res = fclose(fp);
		//		printf("-------const.cpp 31 close (%d)------\n", fp);
		//		printf("fclose(fp = %d) 7 const.cpp = %d\n",fp,res);
		  }		else		{
				free(file_way);
				return -10;
		  }	
	 }
	 
	 if ((access(file_way,0))==-1)  {
		  free(file_way);
		  return -13;
	 }
	 
	 if ((access(file_way,2))==-1)  {
		  free(file_way);
		  return -14;
	 }
	 
	 if ((access(file_way,4))==-1)  {
		  free(file_way);
		  return -11;
	 }
	 
	 if ((access(file_way,6))==-1)  {
		  free(file_way);
		  return -15;
	 }
	 
	 fp = fopen(file_way,"r+");
	 //printf("---- const.cpp 8 open ( %s ) ------\n", file_way);
	 //printf("fp 8 const.cpp = %d\n",fp);
	 
	 if (fp == NULL)  {  free(file_way);	  return -12;	 }
	 fclose(fp);
	// res = fclose(fp);
	 //printf("-------const.cpp 32 close (%d)------\n", fp);
	// printf("fclose(fp = %d) 8 const.cpp = %d\n",fp,res);
	 free(file_way);
	 return 0;
}

/*************************************************************/

int  CreateListComPort (void)
{
    if (system("ls /dev | grep ttyS > /tmp/ports.list") == -1)
    {
	perror("/tmp/ports.list");
	return -1;
    }
    if (system("ls /dev | grep ttyUSB >> /tmp/ports.list") == -1)
    {
	perror("/tmp/ports.list");
	return -1;
    }   
    if (system("ls /dev | grep ttyACM >> /tmp/ports.list") == -1)
    {
	perror("/tmp/ports.list");
	return -1;
    }
    if (FileAcces("/tmp/ports.list") != 0)
    {
      return -1;
    }
    return 1;
}

/*************************************************************/

int GetStrParamForCodes(int SchemeNum, char * PName, char * Res, int MaxResLen)
{
//puts("int GetStrParamForCodes(int SchemeNum, char * PName, char * Res, int MaxResLen)");
FILE *fp;
int c,bi=0;
int MaxLen = strlen(PName);
bool Ind1 = false, Ind2 = false, Ind3 = false, Ind4 = false;
char *Buf;
char *Val;
int kol = 0;
Buf = (char *) calloc(MaxLen+1, sizeof(char));  
Val = (char *) calloc(MaxResLen+2, sizeof(char));
if ((fp = fopen(CODES_WAY,"r")) == NULL)
  {
    free(Buf);
    free(Val);
    return -1;
  } 
fseek(fp,0,0);
while ((c = fgetc(fp)) != EOF)
  {
    if (Ind1 == false)
       {
        if (c==0x5B) 
	   Ind1 = true;   
       }
    else
       if (Ind2 == false)
    	    {
	    if (c == 0x5D) // c == ']'
	       {
	       if (bi==0)
	          {
		  Ind1=false;
		  }
	       else
	          {
    		  *(Buf+bi)=0;
		  if (!strcmp(PName, Buf))
		     {
		     bi=0;
		     Ind2=true;
		     } 
		  else
		     {
		     Ind1=false;
		     bi=0;
   		     }     
		  }
	       }
	    else
	       {
		   if (bi>MaxLen-1)
		      {
		      bi=0;
		      Ind1 = false;
		      }
		   else
		      {   
		      *(Buf+bi) = c;
		      bi++;
		      }
	       }   
	    }
	else //Ind2 == true !!!	  
	   {
	   if ((c==0x5B)||(c == EOF))
	      {
	      if (bi==0)
	         {
		 free(Val);
		 free(Buf);
		 fclose(fp);
//puts("7");
		 return -2;
		 }
	      else
	         { 
		 if (Ind3==true) 
		 { 
		 strcpy(Res, Val);
		 strcpy(Val,"");
		 
		 if (kol != (SchemeNum-1))
		 {
		     bi=0;
		     Ind3=false;
		     Ind2=false;
		     Ind1=false;
		     Ind4=false;
		     kol++;
		 }
		 else
		 {
		 free(Val);
		 free(Buf);
		 fclose(fp);
		 return 1; 
		 }
		 }
		 else
		 {
		 
		 free(Val);
		 free(Buf);
		 fclose(fp);
//puts("6");
		 return -2;
		 }
		 }	 
	      }
	   else // we've got a symbol, that is not '['
	      {
	      if (Ind3 == true)
	      {
	      if ( (c>47) && (c<58) ) 
	         {
		 if (bi>MaxResLen-1)
		    {
		    free(Val);
		    free(Buf);
		    fclose(fp);
//puts("5");
		    return -2;
		    }
		 char* r = (char*)calloc(2,sizeof(char));
		 *r = c;
		 strcat(Val,r);
		 bi++;
		 free(r);
		 }
	      if ( c == 0x22 ) // if we've got '"' (wich means the end of the value)
	         {
		 if (bi == 0)
		    {
		    free(Val);
		    free(Buf);
		    fclose(fp);
//puts("4");
		    return -2;
		    }
		 else
		    {
		    if (Ind4==true)
		    {
		    strcpy(Res, Val);
		    strcpy(Val,"");
	            free(Val);
		    free(Buf);
		    fclose(fp);
//puts("3");
		    return 1;
		    }
		    }
		 }
	      }
	      else //Ind3 == false
	       {
	       if (c == 0x22) // if we've got '"' (wich means the beginning of the value)
	          {
		  Ind3 = true;
		  }
	       }

	      }
	   }
  }
if ((Ind1 == true) && (Ind2 == true) && (Ind3 == true) && (bi > 0))
   { 
   strcpy(Res, Val);
   strcpy(Val,"");
   free(Val);
   free(Buf);
   fclose(fp);
   return 1;
   }
free(Buf);
free(Val);
//puts("1");
return 0; // PARAMETER NOT FOUND !!!
}

/*************************************************************/

char int_to_char(int n) 
{
  char ch = 0;
  
  if (0<n<9) 
  {
  
  switch(n)
  {
  case 0: {ch = '0'; /*ch[0] = 48;*/ break;}
  case 1: {ch = '1'; /*ch[0] = 49;*/ break;}
  case 2: {ch = '2'; /*ch[0] = 50; */ break;}
  case 3: {ch = '3'; /*ch[0] = 51;*/ break;}
  case 4: {ch = '4'; /*ch[0] = 52;*/ break;}
  case 5: {ch = '5'; /*ch[0] = 53;*/ break;}
  case 6: {ch = '6'; /*ch[0] = 54; */ break;} 
  case 7: {ch = '7'; /*ch[0] = 55; */ break;}
  case 8: {ch = '8'; /*ch[0] = 56; */ break;}
  case 9: {ch = '9'; /*ch[0] = 57; */ break;}
  }
  return ch;
}

return 0;

}

char* int_to_str(int n)
{

  //printf("int_to_str(%d)\n",n);
  char* buf = (char*) calloc (11,sizeof(char));
  if (n == 0){
   strcpy(buf,"0");
   return buf;
  }
  double x,y,z;
  char* s = (char*) calloc (11,sizeof(char));
  int i = 0;
  int k = 0;
  x = n;
  while (x != 0)
    {
       x = x/10;
       y = modf(x,&z);
      //printf("x=[%f],y=[%f],z=[%f],y*10=[%f]\n",x,y,z,y*10);
       *(s + i) = int_to_char(int(y*10 + 0.01));
       i++;
       x = x-y;
       if (x == 0)
       break;
    }
  k = strlen(s);
  for (i = 0; i < k; i++)
    {
      *(buf+i) = *(s+(k-1) -i);
    }
  return buf;
}
