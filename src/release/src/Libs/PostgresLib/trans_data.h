#ifndef TRANSACTION
#define TRANSACTION

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>  

extern int num_last_trans;

// число атрибутов - столбцов в таблице БД и их максимальная длина
//-----(*)-----Zuskin-----25/11/2011-----
//const int COUNT_ATTRIBUTE = 25;
const int COUNT_ATTRIBUTE = 26;
//---------------------------------------
const int ATTRIBUTE_LEN_MAX = 40;

const int COUNT_ATTRIBUTE_IN_FORMAT = 24;
const int COUNT_ATTRIBUTE_SHTRIH = 11;
//-----(*)-----Zuskin-----25/11/2011-----
//const int COUNT_ATTRIBUTE_DNC = 16;
const int COUNT_ATTRIBUTE_DNC = 17;
//---------------------------------------

// максимально возможное код типа транзакции 
const int TYPE_TRANS_MAX = 100;

// количество типов транзакций
const int COUNT_TYPE_TRANS = 8;
const int COUNT_TYPE_TRANS_SHTRIH = 11;
const int COUNT_TYPE_TRANS_DNC = 8;




// всякие заморочества в цифрах
/*
  каждая строка отвечает за один тип транзакции
  цифрами указаны какие столбцы таблицы соответсвуют данной позиции в строке
  например 7 строка отвечает транзакции регистрации (11) 
  на 0 позиции дожны быть данные из нулевого столбца
  в 1 из 2
  в 2 из 3 и тд
  номер строки вычисляется из массива type_trans,
  например для регистрации с кодом 11 это будет type_trans[11] = 6 
  массив type_trans заполняется в методе init_type_trans()
  
  0 - номер строки для записи по умолчанию, если вдруг встетиться неизвестный тип транзакции
  
     type_trans[11] = 7;   // регистрация
   type_trans[14] = 1;    // налог
   type_trans[55] = 2;    // закрытие чека
  15,16,17,18,35,36,37,38 = 3;  // скидки / надбавки
  type_trans[41] = 4;   // оплата без ввода суммы клиента
  type_trans[51] = 5;   //внесение
  type_trans[50] = 5;   //выплаты
  */


// Столбцы в запросе 
/* 
0 - n_transaction, 
1 - tr_type, 
2 - tr_date_time, 
3 - n_pc, 
4 - n_kkm, 
5 - seller, 
6 - n_session, 
7 - n_check, 
8 - check_type,
9 - code, 
10 - code_tax_group, 
11 - code_tax_rate, 
12 - payment_type, 
13 - card_code, 
14 - value_1, 
15 - value_2, 
16 - reduction_type, 
17 - reduction_info, 
18 - auto_scheme_id, 
19 - gain, 
20 - ROUND(value_1*value_2,2), 
21 - barcode, 
22 - n_doc, 
23 - value_2 + round_sum, 
24 - round_sum
//-----(+)-----Zuskin-----25/11/2011-----
25 - manager
//---------------------------------------

*/
  
const int mas[COUNT_TYPE_TRANS] [COUNT_ATTRIBUTE_IN_FORMAT] = { {0,2,1,3,7,5,-1,-1,-1,-1,-1,8,6,-1,-1,4,-1,-1,-1,-1,-1,-1,-1}, // по умолчанию
                                                                {0,2,1,3,7,5,9,-1,10,11,14,8,6,-1,-1,4,-1,-1,-1,-1,-1,-1,-1}, // налог
                                                                {0,2,1,3,7,5,13,-1,-1,14,15,8,6,23,24,4,-1,-1,-1,-1,-1,-1,-1}, // закр чека
//-----(*)-----Zuskin-----29/09/2011-----
//                                                              {0,2,1,3,7,5,17,-1,16,19,24,8,6,18,9,4,-1,-1,-1,-1,-1,-1,-1}, // скидки
                                                                {0,2,1,3,7,5,17,-1,16,15,14,8,6,18,9,4,-1,-1,-1,-1,-1,-1,-1}, // скидки
//---------------------------------------
                                                                {0,2,1,3,7,5,-1,12,15,15,14,8,6,-1,-1,4,-1,-1,-1,-1,-1,-1,-1}, // оплата
                                                                {0,2,1,3,7,5,-1,-1,-1,-1,14,8,6,-1,-1,4,-1,-1,-1,-1,-1,-1,-1}, //внесение
                                                                {0,2,1,3,7,5,-1,6,19,14,15,-1,6,-1,-1,4,-1,-1,-1,-1,-1,-1,-1}, // смена -
                                                                {0,2,1,3,7,5,9,-1,14,15,20,8,6,-1,-1,4,-1,21,-1,-1,-1,-1,-1} }; // регистрация  

const int mas_shtrih[COUNT_TYPE_TRANS_SHTRIH] [COUNT_ATTRIBUTE_SHTRIH] = { {0,2,1,4,7,5,-1,-1,-1,-1,-1}, // по умолчанию
									   {0,2,1,4,7,5,9,-1,14,15,20}, // регистрация (возврат)
									   {0,2,1,4,7,5,9,-1,-1,14,14}, // суммовые скидки и надбавки на поз
									   {0,2,1,4,7,5,9,-1,-1,15,14}, // процентные скидки и надбавки на поз	
									   {0,2,1,4,7,5,-1,-1,-1,24,24}, // суммовые скидки и надбавки на чек
									   {0,2,1,4,7,5,-1,-1,-1,19,24}, // процентные скидки и надбавки на чек
									   {0,2,1,4,7,5,9,16,-1,-1,-1}, // детализация скидки
									   {0,2,1,4,7,5,-1,-1,15,12,14}, // оплата
									   {0,2,1,4,7,5,-1,-1,-1,-1,14}, // выплата / внесение
									   {0,2,1,4,7,5,21,-1,14,-1,19}, // X(Z) отчет
									   {0,2,1,4,7,5,-1,-1,-1,-1,15} }; // закр чека

//-----(*)-----Zuskin-----29/09/2011-----
const int mas_dnc[COUNT_TYPE_TRANS_DNC] [COUNT_ATTRIBUTE_DNC] = { {0,2,5,6,7,8,3,4,1,-1,-1,-1,-1,-1,-1,-1,-1},		// по умолчанию
								  {0,2,5,6,7,8,3,4,1,9,14,15,24,-1,17,12,-1},		// Регистрация / сторно
								  {0,2,5,6,7,8,3,4,1,9,-1,14,-1,-1,11,10,-1},		// Налог
								  //-----(*)-----Zuskin-----28/12/2011-----
								  //{0,2,5,6,7,8,3,4,1,16,15,14,-1,17,9,-1,-1},		// Скидки; поле 14 - исходная сумма скидки без учета ограничений по мин. цене
								  {0,2,5,6,7,8,3,4,1,16,15,24,-1,17,9,-1,-1},		// Скидки; поле 24 - итоговая сумма скидки с учетом ограничений по мин. цене 
								  //---------------------------------------
								  {0,2,5,6,7,8,3,4,1,12,14,15,-1,-1,-1,-1,-1},		// Оплата чека
								  {0,2,5,6,7,8,3,4,1,-1,14,15,23,-1,-1,-1,25},		// Закрытие / отмена чека
								  {0,2,5,6,7,8,3,4,1,-1,14,-1,-1,-1,-1,-1,-1},		// Выплата / внемение
								  {0,2,5,6,7,8,3,4,1,-1,14,15,19,-1,-1,-1,-1}, };	// Отчеты
/*const int mas_dnc[COUNT_TYPE_TRANS_DNC] [COUNT_ATTRIBUTE_DNC] = { {0,2,5,6,7,8,3,4,1,-1,-1,-1,-1,-1,-1,-1},		// по умолчанию
								  {0,2,5,6,7,8,3,4,1,9,14,15,24,-1,17,12},		// Регистрация / сторно
								  {0,2,5,6,7,8,3,4,1,9,-1,14,-1,-1,11,10},		// Налог
								  {0,2,5,6,7,8,3,4,1,16,15,14,-1,17,9,-1},		// Скидки
								  {0,2,5,6,7,8,3,4,1,12,14,15,-1,-1,-1,-1},		// Оплата чека
								  {0,2,5,6,7,8,3,4,1,-1,14,15,23,-1,-1,-1},		// Закрытие / отмена чека
								  {0,2,5,6,7,8,3,4,1,-1,14,-1,-1,-1,-1,-1},		// Выплата / внемение
								  {0,2,5,6,7,8,3,4,1,-1,14,15,19,-1,-1,-1}, }; */	// Отчеты 								  
								  
//---------------------------------------
    

class Transaction
{
  public:
   char** data;
   Transaction();
   ~Transaction(); 
   void CreateRecord(char* str);
   void CreateShtrihRecord(char* str);
   void CreateRecordDnc(char* str);   
   void DateToFormat(char* str);
   static void init_type_trans(int n_protocol = 0);
  private:
   void itoa(int d, char * str);
   void add_minus(char * str);
   
};

#endif


