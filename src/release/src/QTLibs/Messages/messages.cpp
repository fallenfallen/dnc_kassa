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

#include "messages.h"
#include "const.h"

#include <qtextcodec.h>
#include "messageform.h"


void InterfaceErrorWin(QString er_str, QString er_id)
{
  
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  messageForm *mf = new messageForm();
  mf->setIcon( 1 );
  mf->setCaption(DefCodec->toUnicode("Ошибка"));
  mf->SetText(er_id + "\n" + er_str);
  mf->exec();
  delete(mf);
}

void InterfaceMessageWin(QString er_str, QString er_id)
{
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  messageForm *mf = new messageForm();
  mf->setIcon( 2 );
  mf->setCaption(DefCodec->toUnicode("Сообщение"));
  mf->SetText(er_id + "\n" + er_str);
  mf->exec();
  delete(mf);
}

void VisualsearchErrorText(int er, QString & er_str, QString & er_id)
{
    QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
    switch(er)  {
    case 1 :
	er_id = DefCodec->toUnicode("Ошибка 1 :");
	er_str = DefCodec->toUnicode("Этот товар закончился\n");   
	break;
    case 2 :
	er_id = DefCodec->toUnicode("Ошибка 2 :");
	er_str = DefCodec->toUnicode("Нет требуемого количества товара\n");
	break;
    case 3 :
	er_id = DefCodec->toUnicode("Ошибка 3 :");
	er_str = DefCodec->toUnicode("Товар закончился \n");
	break;
    case 4 :
	er_id = DefCodec->toUnicode("Ошибка 4:");
	er_str = DefCodec->toUnicode("Ошибка запроса к бд\n");
	break;	
    case 5 :
	er_id = DefCodec->toUnicode("Ошибка 5:");
	er_str = DefCodec->toUnicode("Для данного товара запрещено редактирования количества\n");
	break;		
    default :   {}
    }
}


void LoadUnloadErrorText(int er, QString & er_str, QString & er_id)
{
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(er)
  {
    case 1 :
      er_id = DefCodec->toUnicode("Ошибка 1 :");
      er_str = DefCodec->toUnicode("Файл config не может быть создан, путь к файлу не существует или отсутсвуют права на его редактирование\n");   
      break;
  }
}

void SetupErrorText(int er, QString & er_str, QString & er_id) 
{
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(er)
  {
    case 1 :
      er_id = DefCodec->toUnicode("Ошибка 1 :");
      er_str = DefCodec->toUnicode("Не удалось загрузить список схем\n");   
      break;
    case 2 :
      er_id = DefCodec->toUnicode("Ошибка 2 :");
      er_str = DefCodec->toUnicode("Не удалось считать параметры схемы\n");
      break;
    case 3 :
      er_id = DefCodec->toUnicode("Ошибка 3 :");
      er_str = DefCodec->toUnicode("Множитель для шаблона штрих-кода не может быть пустым\n");
      break;
    case 4 :
      er_id = DefCodec->toUnicode("Ошибка 4 :");
      er_str = DefCodec->toUnicode("Неверный префикс\n");
      break;
    case 5 :
      er_id = DefCodec->toUnicode("Ошибка 5 :");
      er_str = DefCodec->toUnicode("Штрих-код неверной длины\n");
      break;
    case 6 :
      er_id = DefCodec->toUnicode("Ошибка 6 :");
      er_str = DefCodec->toUnicode("Неверный шаблон штрих-кода\n");
      break;
    case 7 :
      er_id = DefCodec->toUnicode("Ошибка 7 :");
      er_str = DefCodec->toUnicode("Схема не сохранена\n");
      break;
    case 8 :
      er_id = DefCodec->toUnicode("Ошибка 8 :");
      er_str = DefCodec->toUnicode("Не удалось сохранить параметры схемы\n"); 
      break; 
    case 9 :
      er_id = DefCodec->toUnicode("Ошибка 9 :");
      er_str = DefCodec->toUnicode("Некоторые штрих-коды уже используются. Задайте другие значения или оставьте поля пустыми\n"); 
      break;
    case 10 :
      er_id = DefCodec->toUnicode("Ошибка 10 :");
      er_str = DefCodec->toUnicode("Некоторые коды магнитных карт уже используются. Задайте другие значения или оставьте поля пустыми\n"); 
      break;
    case 11 :
      er_id = DefCodec->toUnicode("Ошибка 11 :");
      er_str = DefCodec->toUnicode("Не удалось сохранить параметры фиксированных скидок\n"); 
      break;
    case 12 :
      er_id = DefCodec->toUnicode("Ошибка 12 :");
      er_str = DefCodec->toUnicode("Не удалось сохранить шаблоны штрих-кодов\n"); 
      break;
    case 13 :
      er_id = DefCodec->toUnicode("Ошибка 13 :");
      er_str = DefCodec->toUnicode("Не удалось проверить имя схемы\n"); 
      break;
    case 14 :
      er_id = DefCodec->toUnicode("Ошибка 14 :");
      er_str = DefCodec->toUnicode("Имя схемы уже занято\n"); 
      break;
   case 15 :
      er_id = DefCodec->toUnicode("Ошибка 15  :");
      er_str = DefCodec->toUnicode("Не удалось переименовать схему\n"); 
      break;
   case 16 :
      er_id = DefCodec->toUnicode("Ошибка 16 :");
      er_str = DefCodec->toUnicode("Не удалось сохранить параметры в бд\n"); 
      break;
    case 17 :
      er_id = DefCodec->toUnicode("Ошибка 17 :");
      er_str = DefCodec->toUnicode("Имя схемы пусто\n"); 
      break;      
    case 18 :
      er_id = DefCodec->toUnicode("Ошибка 18 :");
      er_str = DefCodec->toUnicode("Не удалось добавить новую схему\n"); 
      break;        
   case 19 :
      er_id = DefCodec->toUnicode("Ошибка 19 :");
      er_str = DefCodec->toUnicode("Вы не можете удалить схему по умолчанию\n"); 
      break;        
   case 20 :
      er_id = DefCodec->toUnicode("Ошибка 20 :");
      er_str = DefCodec->toUnicode("Не удалось удалить схему\n"); 
      break;       
   case 21 :
      er_id = DefCodec->toUnicode("Ошибка 21 :");
      er_str = DefCodec->toUnicode("Не удалось загрузить параметры по умолчанию\n"); 
      break;       
   case 22 :
      er_id = DefCodec->toUnicode("Ошибка 22 :");
      er_str = DefCodec->toUnicode("Не удалось загрузить параметры схемы\n"); 
      break;       
   case 23 :
      er_id = DefCodec->toUnicode("Ошибка 23 :");
      er_str = DefCodec->toUnicode("Не удалось назначить схему по умолчанию\n"); 
      break;
   case 24 :
      er_id = DefCodec->toUnicode("Ошибка 24 :");
      er_str = DefCodec->toUnicode("Не удалось загрузить параметры новой схемы \n"); 
      break;       
   case 25 :
      er_id = DefCodec->toUnicode("Ошибка 25 :");
      er_str = DefCodec->toUnicode("Скидка/надбавка не может иметь нулевое значение\n"); 
      break;
   case 26 :
      er_id = DefCodec->toUnicode("Ошибка 26 :");
      er_str = DefCodec->toUnicode("Должен остаться хотя бы один тип  авторизации\n"); 
      break;       
   case 27 :
      er_id = DefCodec->toUnicode("Ошибка 27 :");
      er_str = DefCodec->toUnicode("Нет связи с фискальным регистратором!!! Включение обучающего режима невозможно \n"); 
      break;
   case 28 :
      er_id = DefCodec->toUnicode("Ошибка 28 :");
      er_str = DefCodec->toUnicode("Смена открыта, включение обучающего режима невозможно\n"); 
      break;     
   case 29 :
      er_id = DefCodec->toUnicode("Ошибка 29 :");
      er_str = DefCodec->toUnicode("Не удалось сохранить список печатаемых документов\n"); 
      break;     
   case 30 :
      er_id = DefCodec->toUnicode("Ошибка 30 :");
      er_str = DefCodec->toUnicode("Не удалось загрузить список печатаемых документов\n"); 
      break;   
   case 31 :
      er_id = DefCodec->toUnicode("Ошибка 31 :");
      er_str = DefCodec->toUnicode("Номинал максимальной купюры не может быть меньше единицы\n"); 
      break;     
  }
}

void AccessRightsMessageText(int er, QString & er_str, QString & er_id)
{
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(er)
  {
    /*---------EditUser-----------------*/
    case 1 :
      er_id = DefCodec->toUnicode("Ошибка 1 :");
      er_str = DefCodec->toUnicode("Не введено имя пользователя");   
      break;
    case 2 :
      er_id = DefCodec->toUnicode("Ошибка 2 :");
      er_str = DefCodec->toUnicode("Не введен профиль пользователя\n");
      break;
    case 3 :
      er_id = DefCodec->toUnicode("Ошибка 3 :");
      er_str = DefCodec->toUnicode("Не удалось добавить пользователя :о\n");
      break;
    case 4 :
      er_id = DefCodec->toUnicode("Ошибка 4 :");
      er_str = DefCodec->toUnicode("Не удалось изменить данные пользователя :о\n");
      break;
    case 5 :
      er_id = DefCodec->toUnicode("Ошибка 5 :");
      er_str = DefCodec->toUnicode("Неверно введен пароль пользователя\n");
      break;
    case 6 :
      er_id = DefCodec->toUnicode("Ошибка 6 :");
      er_str = DefCodec->toUnicode("Данный штрих-код уже используется в системе\n");
      break;
    case 7 :
      er_id = DefCodec->toUnicode("Ошибка 7 :");
      er_str = DefCodec->toUnicode("Данный код магнитной карты уже используется\n");
      break;
    /*-----------ProfileEdit---------------*/
    case 8 :
      er_id = DefCodec->toUnicode("Ошибка 8 :");
      er_str = DefCodec->toUnicode("Не удалось добавить профиль\n"); 
      break; 
    case 9 :
      er_id = DefCodec->toUnicode("Ошибка 9 :");
      er_str = DefCodec->toUnicode("Не удалось изменить профиль\n"); 
      break;
    case 10 :
      er_id = DefCodec->toUnicode("Ошибка 10 :");
      er_str = DefCodec->toUnicode("Неизвестное наименование профиля\n"); 
      break;
    /*-----------MainForm---------------*/ 
    case 11 :
      er_id = DefCodec->toUnicode("Ошибка 11 :");
      er_str = DefCodec->toUnicode("Невозможно удалить или изменить профиль Администратора\n"); 
      break;
    case 12 :
      er_id = DefCodec->toUnicode("Ошибка 12 :");
      er_str = DefCodec->toUnicode("Невозможно удалить пользователя Администратор\n"); 
      break;
    case 13 :
      er_id = DefCodec->toUnicode("Ошибка 13 :");
      er_str = DefCodec->toUnicode("Не удалось удалить профиль\n"); 
      break;
    case 14 :
      er_id = DefCodec->toUnicode("Ошибка 14 :");
      er_str = DefCodec->toUnicode("Существует пользователь с выбранным профилем\nудалить профиль невозможно\n"); 
      break;
   case 15 :
      er_id = DefCodec->toUnicode("Ошибка 15 :");
      er_str = DefCodec->toUnicode("Не выбран объект редактирования\n"); 
      break;
   case 16 :
      er_id = DefCodec->toUnicode("Ошибка 16 :");
      er_str = DefCodec->toUnicode("Не выбран объект для удаления\n"); 
      break;
   case 17 :
      er_id = DefCodec->toUnicode("Ошибка 17 :");
      er_str = DefCodec->toUnicode("Не удалось удалить пользователя\n"); 
      break;
  case 18 :
      er_id = DefCodec->toUnicode("Ошибка 18 :");
      er_str = DefCodec->toUnicode("Не удалось загрузить данные о профилях\n"); 
      break;
   case 19 :
      er_id = DefCodec->toUnicode("Ошибка 19 :");
      er_str = DefCodec->toUnicode("Не удалось загрузить данные о пользователях\n"); 
      break;
  }
}

void DeviceConfErrorText(int er, QString & er_str, QString & er_id)
{
QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(er)
  {
  case 1 :
      er_id = DefCodec->toUnicode("Ошибка 1 :");
      er_str = DefCodec->toUnicode("Схема с указанным именем уже существует!\n");
      break;
    case 2 :
      er_id = DefCodec->toUnicode("Ошибка 2 :");
      er_str = DefCodec->toUnicode("Нет доступа к конфигурационному файлу\n");
      break;
    case 3 :
      er_id = DefCodec->toUnicode("Ошибка 3 :");
      er_str = DefCodec->toUnicode("Нет доступа к файлу /tmp/hw.temp\n");
      break; 
    case 4 :
      er_id = DefCodec->toUnicode("Ошибка 4 :");
      er_str = DefCodec->toUnicode("Ошибка в настройках сканера!\nПорт уже используется другим оборудованием\n");
      break;
    case 5 :
      er_id = DefCodec->toUnicode("Ошибка 5 :");
      er_str = DefCodec->toUnicode("Ошибка в настройках ридера!\nПорт уже используется другим оборудованием\n");
      break;
    case 6 :
      er_id = DefCodec->toUnicode("Ошибка 6 :");
      er_str = DefCodec->toUnicode("Ошибка в настройках фискального регистратора/принтера чеков!\nПорт уже используется другим оборудованием\n");  
      break;
   case 7 :
      er_id = DefCodec->toUnicode("Ошибка 7 :");
      er_str = DefCodec->toUnicode("Должен быть указан как минимум один суффикс,\n отличный от нуля\n");
      break;
    case 8 :
      er_id = DefCodec->toUnicode("Ошибка 8 :");
      er_str = DefCodec->toUnicode("Должен быть указан как минимум один префикс,\n отличный от нуля\n");
      break;
    case 9 :
      er_id = DefCodec->toUnicode("Ошибка 9 :");
      er_str = DefCodec->toUnicode("Этот порт уже используется другим оборудованием\n");
      break;
    case 10 :
      er_id = DefCodec->toUnicode("Ошибка 10 :");
      er_str = DefCodec->toUnicode("Отсутствует файл с перекодировками\n");
      break;
    case 11 :
      er_id = DefCodec->toUnicode("Ошибка 11 :");
      er_str = DefCodec->toUnicode("Настройки дисплея покупателя не верны\n");
      break;
    case 12 :
      er_id = DefCodec->toUnicode("Ошибка 12 :");
      er_str = DefCodec->toUnicode("Не задано название схемы\n");  
      break;
    case 13 :
      er_id = DefCodec->toUnicode("Ошибка 13 :");
      er_str = DefCodec->toUnicode("Название схемы должно быть уникальным\n");
      break;
    case 14 :
      er_id = DefCodec->toUnicode("Ошибка 14 :");
      er_str = DefCodec->toUnicode("Невозможно открыть указанный порт\n");
      break; 
    case 15 :
      er_id = DefCodec->toUnicode("Ошибка 15 :");
      er_str = DefCodec->toUnicode("Неверная длина введенного штрих-кода\n");
      break;
    case 16 :
      er_id = DefCodec->toUnicode("Ошибка 16 :");
      er_str = DefCodec->toUnicode("Штрих-код не введен\n");
      break;
    case 17 :
      er_id = DefCodec->toUnicode("Ошибка 17 :");
      er_str = DefCodec->toUnicode("Ридер не смог считать информацию с карты\n");  
      break;   
    case 18 :
      er_id = DefCodec->toUnicode("Ошибка 18 :");
      er_str = DefCodec->toUnicode("Не совпадают префиксы и суффиксы разных дорожек карты!\nПерепрограммируйте ридер\n");
      break;  
    case 19 :
      er_id = DefCodec->toUnicode("Ошибка 19 :");
      er_str = DefCodec->toUnicode("Количество полученных префиксов не равно двум!\nПерепрограммируйте ридер\n");
      break;
    case 20 :
      er_id = DefCodec->toUnicode("Ошибка 20 :");
      er_str = DefCodec->toUnicode("Префиксы не определены!\nПерепрограммируйте ридер\n");  
      break;
    case 21 :
      er_id = DefCodec->toUnicode("Ошибка 21 :");
      er_str = DefCodec->toUnicode("Количество полученных суффиксов не равно двум!\nПерепрограммируйте ридер\n");
      break;
    case 22 :
      er_id = DefCodec->toUnicode("Ошибка 22 :");
      er_str = DefCodec->toUnicode("Суффиксы не определены!\nПерепрограммируйте ридер\n");
      break;
    case 23 :
      er_id = DefCodec->toUnicode("Ошибка 23 :");
      er_str = DefCodec->toUnicode("Необходимо заполнить перекодировку для всех букв\n");
      break;
    case 24 :
      er_id = DefCodec->toUnicode("Ошибка 24 :");
      er_str = DefCodec->toUnicode("Первый символ названия пекрекодировки\n не должен начинться со служебного символа\n");
      break;
    case 25 :
      er_id = DefCodec->toUnicode("Ошибка 25 :");
      er_str = DefCodec->toUnicode("Название перекодировки должно содержать\n хотя бы один символ\n");
      break;
    case 26 :
      er_id = DefCodec->toUnicode("Ошибка 26 :");
      er_str = DefCodec->toUnicode("Схема не может быть сохранена, так как два вида оборудования настроены на один порт\n");
      break;
    case 27 :
      er_id = DefCodec->toUnicode("Ошибка 27 :");
      er_str = DefCodec->toUnicode("Принудительная прокрутка не может быть установлена для верхней строки, так как ее длина больше 20 символов\n");
      break;
    case 28 :
      er_id = DefCodec->toUnicode("Ошибка 28 :");
      er_str = DefCodec->toUnicode("Принудительная прокрутка не может быть установлена для нижней строки, так как ее длина больше 20 символов\n");
      break;
    case 29 :
      er_id = DefCodec->toUnicode("Ошибка 29 :");
      er_str = DefCodec->toUnicode("Сканер не смог считать штрих-код!\n");
      break;	
    case 30 :
      er_id = DefCodec->toUnicode("Ошибка 30 :");
      er_str = DefCodec->toUnicode("Введенный штрих-код не совпадает с полученным!\n");
      break;
    case 31:
      er_id = DefCodec->toUnicode("Ошибка 31 :");
      er_str = DefCodec->toUnicode("Файл с перекодировками не доступен на запись!\n");
      break;
    case 32:
      er_id = DefCodec->toUnicode("Ошибка 32 :");
      er_str = DefCodec->toUnicode("Файл с перекодировками не доступен на чтение!\n");
      break;
    case 33:
      er_id = DefCodec->toUnicode("Ошибка 33 :");
      er_str = DefCodec->toUnicode("Не возможно остановить бегущие строки в связи с  попыткой процесса остановиться(в файле записан символ остановки(#))\n");
      break;
    case 34:
      er_id = DefCodec->toUnicode("Ошибка 34 :");
      er_str = DefCodec->toUnicode("Не возможно остановить бегущие строки в связи с отсутствием прав доступа на запись и создание на файл остановки бегущих строк дисплея!\n");
      break;
    case 35:
      er_id = DefCodec->toUnicode("Ошибка 35 :");
      er_str = DefCodec->toUnicode("Не возможно остановить бегущие строки в связи с отсутствием прав доступа на запись на файл остановки бегущих строк дисплея!\n");
      break;
    case 36:
      er_id = DefCodec->toUnicode("Ошибка 36 :");
      er_str = DefCodec->toUnicode("Ошибка в настройках дисплея!\nПорт уже используется другим оборудованием\n\n");
      break;
    case 37:
      er_id = DefCodec->toUnicode("Ошибка 37 :");
      er_str = DefCodec->toUnicode("Ошибка в настройках платежной системы!\nПорт уже используется другим оборудованием\n\n");
      break;
    case 38:
      er_id = DefCodec->toUnicode("Ошибка 38 :");
      er_str = DefCodec->toUnicode("Ошибка в настройках весов!\nПорт уже используется другим оборудованием\n\n");
      break;
  default:
      er_id = DefCodec->toUnicode("Ошибка -10 :");
      er_str = DefCodec->toUnicode("Неизвестная ошибка!\n");
      break;

  }
}


void DeviceConfMessageText(int mes, QString & mes_str, QString & mes_id)
{
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(mes)
  {
    case 1 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Введите наименование схемы\n");
      break;
    case 2 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Данная схема не может быть удалена\n");
      break;
    case 3 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Схема успешно удалена\n");
      break;
    case 4 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Невозможна работа с конфигурационным файлом!\nНажмите [OK] для завершения работы\n");
      break;
    case 5 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Для работы с настроечным интерфейсом\n необходимо ввести имя схемы.\nДанная схема будет использоваться по умоланию\n");
      break;
    case 6 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Конфигурационный файл пуст\n");
      break;
    case 7 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Для работы с настроечным интерфейсом\n необходимо ввести имя схемы\n");
      break;
    case 8 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Данная схема будет использоваться по умолчанию\n");
      break;
    case 9 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Не введена информация в строки\n для вывода на дисплей\n");
      break;
    case 10 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Префиксы и суффиксы определены неверно\n");
      break;
    case 11 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Префиксы и суффиксы не были определены\n");
      break;
    case 12 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Файл содержит последнюю перекодировку\n");
      break;
    case 13 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Перекодировка с таким именем уже существует\n");
      break;
      case 14 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Схема успешно сохранена\n");
      break; 
   case 15 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Клавиатура настроена на русскую перекодировку!\n");
      break;
   case 16 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Префиксы и суффиксы определены верно\n");
      break;   
  case 17 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Это самая первая схема! Она будет создана и сохранена с именем по умолчанию!\n");
      break;
   case 18 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Неверно установлен флаг прокрутки строки. Он будет снят, так как длина строки больше 20 символов\n");
      break;
  }
}



void HWSRVErrorText(int er, QString & er_str, QString & er_id){
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(er){
    case 1:
      er_id = DefCodec->toUnicode("Ошибка 1 :");
      er_str = DefCodec->toUnicode("Не удалось открыть конфигурационный файл\n");
      break;
    case 2:
      er_id = DefCodec->toUnicode("Ошибка 2 :");
      er_str = DefCodec->toUnicode("Не удалось прочитать параметр из конфигурационного файла\n");
      break;
    case 4:
      er_id = DefCodec->toUnicode("Ошибка 4 :");
      er_str = DefCodec->toUnicode("Пришел неверный код символа от сканера\n");
      break;
    case 5:
      er_id = DefCodec->toUnicode("Ошибка 5 :");
      er_str = DefCodec->toUnicode("Не удалось назначить сетевой адрес сокету\n");
      break;
    case 6:
      er_id = DefCodec->toUnicode("Ошибка 6 :");
      er_str = DefCodec->toUnicode("Не удалось открыть новый сокет\n");
      break;
   case 7:
      er_id = DefCodec->toUnicode("Ошибка 7 :");
      er_str = DefCodec->toUnicode("Не удалось установить параметры сокета\n");
      break;
  }
}




void AuthorizationErrorText(int er, QString & er_str, QString & er_id)
{
QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(er)
  {
    case 0 :
      er_id = DefCodec->toUnicode("Сообщение :");
      er_str = DefCodec->toUnicode("Вы авторизовались как ") + er_str + "\n";
      break;
    case 1 :
      er_id = DefCodec->toUnicode("Ошибка 1 :");
      er_str = DefCodec->toUnicode("Ошибка аутентификации\n");
      break;
    case 2 :
      er_id = DefCodec->toUnicode("Ошибка 2 :");
      er_str = DefCodec->toUnicode("Не удалось загрузить права доступа для пользователя\n");
      break;
    case 3 :
      er_id = DefCodec->toUnicode("Ошибка 3 :");
      er_str = DefCodec->toUnicode("Ошибка выполнения запроса к БД\n");
      break; 
    case 4 :
      er_id = DefCodec->toUnicode("Ошибка 4 :");
      er_str = DefCodec->toUnicode("Неверный пароль\n");
      break;  
    case 5 :
      er_id = DefCodec->toUnicode("Ошибка 5 :");
      er_str = DefCodec->toUnicode("Нет пользователя с данным штрих-кодом\n");
      break;
    case 6 :
      er_id = DefCodec->toUnicode("Ошибка 6 :");
      er_str = DefCodec->toUnicode("Вы не зарегистрированы в системе\n");  
      break;   
    case 7 :
      er_id = DefCodec->toUnicode("Ошибка 7 :");
      er_str = DefCodec->toUnicode("Авторизация по штрих-коду запрещена в настройках\n");  
      break;      
    case 8 :
      er_id = DefCodec->toUnicode("Ошибка 8 :");
      er_str = DefCodec->toUnicode("Авторизация магнитной картой запрещена в настройках\n");  
      break;      
    case 9 :
      er_id = DefCodec->toUnicode("Ошибка 9 :");
      er_str = DefCodec->toUnicode("Авторизация по паролю запрещена в настройках. Используйте другие способы авторизации\n");  
      break;
  }
}

void RegistrationMessageText(int mes, QString & mes_str, QString & mes_id)
{
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(mes)
  {
    case 1 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Необходимо синхронизировать время ККМ и ПК");
      break;
    case 2 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Рассинхронизация времени ККМ и ПК больше допустимого диапазона.\nПриложение будет закрыто");
      break;
    case 3 : 
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Время смены закончилось.\nПриложение будет закрыто");
      break;
    case 4 :
      mes_id = DefCodec->toUnicode("Сообщение :");
      mes_str = DefCodec->toUnicode("Оплата наличными запрещена правами доступа");	
      break;
    case 5 :
      mes_id = DefCodec->toUnicode("Сообщение :");
      mes_str = DefCodec->toUnicode("Сумма клиента больше чем сумма чека и максимально возможная купюра");	
      break;     
    case 6 :
      mes_id = DefCodec->toUnicode("Сообщение :");
      mes_str = DefCodec->toUnicode("Оплата кредитом не доступна в обучающем режиме");	
      break;      
    case 7 :
      mes_id = DefCodec->toUnicode("Сообщение :");
      mes_str = DefCodec->toUnicode("Платежная система не подключена");	
      break;     
  }
}



void RegistrationErrorText(int er, QString & er_str, QString & er_id)
{
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);  
  switch(er)
  {
  case 1 : 
      er_id = DefCodec->toUnicode("Ошибка 1 :");
      er_str = DefCodec->toUnicode("Продажа данного товара запрещена в карточке товара");
      break;
  case 2 : 
      er_id = DefCodec->toUnicode("Ошибка 2 :");
      er_str = DefCodec->toUnicode("Возврат данного товара запрещен в карточке товара");
      break;
  case 3 : 
      er_id = DefCodec->toUnicode("Ошибка 3 :");
      er_str = DefCodec->toUnicode("Неверный штрих-код товара");
      break;
  case 4 : 
      er_id = DefCodec->toUnicode("Ошибка 4 :");
      er_str = DefCodec->toUnicode("Неверный код товара");
      break;
  case 5 :
      er_id = DefCodec->toUnicode("Ошибка 5 :");
      er_str = DefCodec->toUnicode("Нет требуемого количества товара");
      break;
  case 6 : 
      er_id = DefCodec->toUnicode("Ошибка 6 :");
      er_str = DefCodec->toUnicode("Этот чек не является продажей - возврат невозможен");
      break;
  case 7 : 
      er_id = DefCodec->toUnicode("Ошибка 7 :");	  
      er_str = DefCodec->toUnicode("Сервер БД не отвечает");
      break;
  case 8 :
      er_id = DefCodec->toUnicode("Ошибка 8 :");
      er_str = DefCodec->toUnicode("Попытка продать штучный товар как весовой!!!");
      break;
  case 9 :
      er_id = DefCodec->toUnicode("Ошибка 9 :");
      er_str = DefCodec->toUnicode("Некорректный код товара\nКод должен содержать только цифры");
      break;
  case 10 :
      er_id = DefCodec->toUnicode("Ошибка 10 :");
      er_str = DefCodec->toUnicode("Превышение максимальной суммы на позиции\n Попробуйте разбить сумму на два чека");
      break;      
  case 11 :
      er_id = DefCodec->toUnicode("Ошибка 11 :");
      er_str = DefCodec->toUnicode("Превышение максимальной суммы чека\n Зарегистрируйте оставшиеся позиции отдельным чеком");
      break;     
  case 12 :
      er_id = DefCodec->toUnicode("Ошибка 12 :");
      er_str = DefCodec->toUnicode("Отсутствует объект редактирования");
      break;
  case 13 :
      er_id = DefCodec->toUnicode("Ошибка 13 :");
      er_str = DefCodec->toUnicode("Чек закрыт - редактирование невозможно");
      break;     
  case 14 : 
      er_id = DefCodec->toUnicode("Ошибка 14 :");
      er_str = DefCodec->toUnicode("Чек не открыт");
      break;
  case 15 : 
      er_id = DefCodec->toUnicode("Ошибка 15 :");
      er_str = DefCodec->toUnicode("Отмена закрытого чека невозможна");
      break;
  case 16 :
      er_id = DefCodec->toUnicode("Ошибка 16 :");
      er_str = DefCodec->toUnicode("Неверно введена цена");
      break;
  case 17 :
      er_id = DefCodec->toUnicode("Ошибка 17 :");
      er_str = DefCodec->toUnicode("Повторное сторнирование невозможно");
      break;
  case 18 :
      er_id = DefCodec->toUnicode("Ошибка 18 :");
      er_str = DefCodec->toUnicode("Чек закрыт - операция невозможна");
      break;  
  case 19:
      er_id = DefCodec->toUnicode("Ошибка 19 :");
      er_str = DefCodec->toUnicode("Не смогли записать/считать значение регистра");
      break;  
  case 21 :
      er_id = DefCodec->toUnicode("Ошибка 21 :");
      er_str = DefCodec->toUnicode("Транзакция не прошла");
      break;
  case 22 :
      er_id = DefCodec->toUnicode("Ошибка 22 :");
      er_str = DefCodec->toUnicode("Редактирование сторнированной позиции запрещено");
      break;
  case 23 :
      er_id = DefCodec->toUnicode("Ошибка 23 :");
      er_str = DefCodec->toUnicode("Неверно введен номер чека");
      break;
  case 24 :
      er_id = DefCodec->toUnicode("Ошибка 24 :");
      er_str = DefCodec->toUnicode("Чек открыт - операция невозможна");
      break;
  case 25 :
      er_id = DefCodec->toUnicode("Ошибка 25 :");
      er_str = DefCodec->toUnicode("Введена неверная сумма");
      break;
  case 26 :
      er_id = DefCodec->toUnicode("Ошибка 26 :");
      er_str = DefCodec->toUnicode("Операция внесения/выплаты невозможна");
      break;
  case 27 :
      er_id = DefCodec->toUnicode("Ошибка 27 :");
      er_str = DefCodec->toUnicode("Нет такого чека в базе данных. Или чек не является ни оплатой ни возратом");
      break;
  case 28 :
      er_id = DefCodec->toUnicode("Ошибка 28 :");
      er_str = DefCodec->toUnicode("Невозможен повтор продажи для весового товара");
      break;
  case 29 : 
      er_id = DefCodec->toUnicode("Ошибка 29 :");
      er_str = DefCodec->toUnicode("Сумма клиента меньше итоговой суммы");
      break;
  case 30 : 
      er_id = DefCodec->toUnicode("Ошибка 30 :");
      er_str = DefCodec->toUnicode("Отсутствует объект сторнирования");
      break;      
  case 31 : 
      er_id = DefCodec->toUnicode("Ошибка 31 :");
      er_str = DefCodec->toUnicode("Чек закрыт - сторнирование невозможно");
      break;
  case 32 :
      er_id = DefCodec->toUnicode("Ошибка 32 :");
      er_str = DefCodec->toUnicode("Промитог в просмотре копии чека запрещен");
      break;
  case 33 :
      er_id = DefCodec->toUnicode("Ошибка 33 :");
      er_str = DefCodec->toUnicode("Визуальный поиск в просмотре копии чека запрещен");
      break;
  case 34 :
      er_id = DefCodec->toUnicode("Ошибка 34 :");
      er_str = DefCodec->toUnicode("Сторнирование в просмотре копии чека запрещено");
      break;
  case 35 :
      er_id = DefCodec->toUnicode("Ошибка 35 :");
      er_str = DefCodec->toUnicode("Повтор продажи в просмотре копии чека запрещен");
      break;
  case 36 :
      er_id = DefCodec->toUnicode("Ошибка 36 :");
      er_str = DefCodec->toUnicode("Эта операция в просмотре копии чека запрещена");
      break;
  case 37 :
      er_id = DefCodec->toUnicode("Ошибка 37 :");
      er_str = DefCodec->toUnicode("Редактирование в просмотре копии чека запрещено");
      break;
  case 38 :
      er_id = DefCodec->toUnicode("Ошибка 38 :");
      er_str = DefCodec->toUnicode("Ошибка при синхронизации баз");
      break;
  case 39 :
      er_id = DefCodec->toUnicode("Предупреждение 39 :");
      er_str = DefCodec->toUnicode("Транзакция внесения/выплаты не будет внесена в БД транзакций");
      break;
  case 40 :
      er_id = DefCodec->toUnicode("Предупреждение 40 :");
      er_str = DefCodec->toUnicode("Транзакция регистрации/возврата не будет внесена в БД транзакций");
      break;
  case 41 :
      er_id = DefCodec->toUnicode("Ошибка 41 :");
      er_str = DefCodec->toUnicode("Введено количество для неизвестного товара");
      break;
  case 42 :
      er_id = DefCodec->toUnicode("Ошибка 42 :");
      er_str = DefCodec->toUnicode("Неверно задана скидка");
      break;
  case 43 :
      er_id = DefCodec->toUnicode("Ошибка 43 :");
      er_str = DefCodec->toUnicode("Не выбрана позиция в чеке для скидки");
      break;
  case 44 :
      er_id = DefCodec->toUnicode("Ошибка 44 :");
      er_str = DefCodec->toUnicode("Не задана скидка");
      break;
  case 45 :
      er_id = DefCodec->toUnicode("Ошибка 45 :");
      er_str = DefCodec->toUnicode("Операция невозможна - начислена скидка на чек\n Возможные операции: \"Оплата\", \"Отменить скидку\", \"Отменить чек\"");
      break;
  case 46 :
      er_id = DefCodec->toUnicode("Ошибка 46 :");
      er_str = DefCodec->toUnicode("Сумма скидки превышает сумму указанной позиции, начисление скидки отменено!");
      break;
  case 47 :
      er_id = DefCodec->toUnicode("Ошибка 47 :");
      er_str = DefCodec->toUnicode("Сумма скидки превышает сумму чека с учетом всех скидок, начисление скидки отменено!");
      break;
  case 48 :
      er_id = DefCodec->toUnicode("Ошибка 48 :");
      er_str = DefCodec->toUnicode("Все позиции сторнированы - закрытие чека невозможно");
      break;
  case 49 :
      er_id = DefCodec->toUnicode("Ошибка 49 :");
      er_str = DefCodec->toUnicode("Сумма в ящике нулевая - возврат невозможен");
      break;
  case 50 :
      er_id = DefCodec->toUnicode("Ошибка 50 :");
      er_str = DefCodec->toUnicode("Накопление меньше суммы возврата - возврат невозможен");
      break;
  case 51 :
      er_id = DefCodec->toUnicode("Ошибка 51 :");
      er_str = DefCodec->toUnicode("Ошибка редактирования количества\nвведены разные величины в поле ввода и поле 'количество/масса'");
      break;
  case 52 :
      er_id = DefCodec->toUnicode("Ошибка 52 :");
      er_str = DefCodec->toUnicode("Вызов меню типов оплаты запрещен при возврате,\nвыплата возможна только наличными");
      break;
    case 53:
      er_id = DefCodec->toUnicode("Ошибка 53 :");
      er_str = DefCodec->toUnicode("Введенная цена ниже минимальной, указанной в карточке товара");
      break;
    case 54/*1*/ : 
      er_id = DefCodec->toUnicode("Ошибка 54 :");
      er_str = DefCodec->toUnicode("Некорректный штрих код товара");
      break;
    case 55/*2*/ : 
      er_id = DefCodec->toUnicode("Ошибка 55 :");
      er_str = DefCodec->toUnicode("Неверно введено количество товара");
      break;
    case 56/*9*/ : 
      er_id = DefCodec->toUnicode("Ошибка 56 :");
      er_str = DefCodec->toUnicode("Некорректный код товара\n Код должен содержать только цифры");
      break;
    case 57 :
      er_id = DefCodec->toUnicode("Ошибка 57 :");
      er_str = DefCodec->toUnicode("Нет позиций для возврата");
      break;
    case 66 :
      er_id = DefCodec->toUnicode("Ошибка 66 :");
      er_str = DefCodec->toUnicode("Ошибка при запросе состояния ККМ");
      break;
  case 67 :
      er_id = DefCodec->toUnicode("Ошибка 67 :");
      er_str = DefCodec->toUnicode("Нет скидок на позицию или чек, которые можно отменить");
      break;
  case 68 :
      er_id = DefCodec->toUnicode("Ошибка 68 :");
      er_str = DefCodec->toUnicode("Нет объектов для начисления скидок");
      break;
  case 69 :
      er_id = DefCodec->toUnicode("Ошибка 69 :");
      er_str = DefCodec->toUnicode("Не отвечает база данных или список допустимых фиксированных скидок пуст");
      break;
    case 70 :
      er_id = DefCodec->toUnicode("Ошибка 70 :");
      er_str = DefCodec->toUnicode("Не отвечает база данных - невозможно отменить автоматическую скидку");
      break;
    case 71 :
      er_id = DefCodec->toUnicode("Ошибка 71 :");
      er_str = DefCodec->toUnicode("Смена открыта, синхронизация времени невозможна\nЗакройте смену");
      break;
    case 72 :
      er_id = DefCodec->toUnicode("Ошибка 72 :");
      er_str = DefCodec->toUnicode("Операция запрещена правами доступа");
      break;
    case 73 :
      er_id = DefCodec->toUnicode("Ошибка 73 :");
      er_str = DefCodec->toUnicode("Для продолжения работы необходима синхронизация времени кассы и ПК");
      break;
    case 74 :
      er_id = DefCodec->toUnicode("Ошибка 74 :");
      er_str = DefCodec->toUnicode("Время открытой смены закончилось. Необходимо закрыть смену");
      break;
    case 75 : // FATAL ERROR;
      er_id = DefCodec->toUnicode("Ошибка 75 :");
      er_str = DefCodec->toUnicode("Нет связи с БД. Не удалось получить время начала смены");
      break;
    case 76 :
      er_id = DefCodec->toUnicode("Ошибка 76 :");
      er_str = DefCodec->toUnicode("В БД нет информации об открытой смене. Для продолжения работы необходимо закрыть смену");
      break;
    case 77 :
      er_id = DefCodec->toUnicode("Ошибка 77 :");
      er_str = DefCodec->toUnicode("Не удалось получить права доступа"); 
      break;
    case 78 :
      er_id = DefCodec->toUnicode("Ошибка 78 :");
      er_str = DefCodec->toUnicode("Не удалось получить настройки интерфейса. Работа будет продолженна с настройками по умолчанию");
      break;
    case 79 :
      er_id = DefCodec->toUnicode("Ошибка 79 :");
      er_str = DefCodec->toUnicode("Не удалось получить все настройки интерфейса. Для отсутствующих настроек будут использованны значения по умолчанию");
      break;
    case 80 :
      er_id = DefCodec->toUnicode("Ошибка 80 :");
      er_str = DefCodec->toUnicode("Нет связи с базой данных");
      break;
    case 81 :
      er_id = DefCodec->toUnicode("Ошибка 81 :");
      er_str = DefCodec->toUnicode("Сумма скидки больше начиcлений по чеку");
      break;
    case 82 :
      er_id = DefCodec->toUnicode("Ошибка 82 :");
      er_str = DefCodec->toUnicode("Цена на товар с учетом скидки меньше минимальной цены в карточке товара");
      break;
    case 83 :
      er_id = DefCodec->toUnicode("Ошибка 83 :");
      er_str = DefCodec->toUnicode("Сумма скидки больше максимальной скидки на позицию");
      break;
    case 84 :
      er_id = DefCodec->toUnicode("Ошибка 84 :");
      er_str = DefCodec->toUnicode("Сумма скидки больше максимальной скидки на чек");
      break;
    case 85 :
      er_id = DefCodec->toUnicode("Ошибка 85 :");
      er_str = DefCodec->toUnicode("Сумма надбавки больше максимальной надбавки на позицию");
      break;
    case 86 :
      er_id = DefCodec->toUnicode("Ошибка 86 :");
      er_str = DefCodec->toUnicode("Сумма надбавки больше максимальной надбавки на чек");
      break;
    case 87 :
      er_id = DefCodec->toUnicode("Ошибка 87 :");
      er_str = DefCodec->toUnicode("Сумма чека больше максимальной");	
      break;
    case 88 :
      er_id = DefCodec->toUnicode("Ошибка 88 :");
      er_str = DefCodec->toUnicode("Невозможно получить актуальные данные по чеку");	
      break;
    case 89 :
      er_id = DefCodec->toUnicode("Ошибка 89 :");
      er_str = DefCodec->toUnicode("Невозможно получить данные по чеку");
      break;
    case 90 :
      er_id = DefCodec->toUnicode("Ошибка 90 :");
      er_str = DefCodec->toUnicode("Чек не является продажей. Печать копии не возможна");
      break;
    case 91 :
      er_id = DefCodec->toUnicode("Ошибка 91 :");
      er_str = DefCodec->toUnicode("Начисление фиксированной скидки по магнитной карте доступно только в состоянии промитога");
      break; 
    case 92 :
      er_id = DefCodec->toUnicode("Ошибка 92 :");
      er_str = DefCodec->toUnicode("В БД отсутствует информация о фиксированной скидке с таким кодом дисконтной карты");
      break; 
    case 93 :
      er_id = DefCodec->toUnicode("Ошибка 93 :");
      er_str = DefCodec->toUnicode("Продажа товаров запрещена в настройках контроля регистраций");
      break; 
    case 94 :
      er_id = DefCodec->toUnicode("Ошибка 94 :");
      er_str = DefCodec->toUnicode("Возрат товаров запрещен в настройках контроля регистраций");
      break; 
    case 95 :
      er_id = DefCodec->toUnicode("Ошибка 95 :");
      er_str = DefCodec->toUnicode("Для продаваемого товара истек срок годности!!!");
      break; 
    case 96 :
      er_id = DefCodec->toUnicode("Ошибка 96 :");
      er_str = DefCodec->toUnicode("Продажа весового товара запрещена в настройках контроля регистраций!!!");
      break; 
    case 97 :
      er_id = DefCodec->toUnicode("Ошибка 97 :");
      er_str = DefCodec->toUnicode("Редактирование цены запрещено в настройках контроля регистраций!!!");
      break; 
    case 98 :
      er_id = DefCodec->toUnicode("Ошибка 98 :");
      er_str = DefCodec->toUnicode("Для данного товара запрещен ввод количества вручную!!!");
      break; 
    case 99 :
      er_id = DefCodec->toUnicode("Ошибка 99 :");
      er_str = DefCodec->toUnicode("Ошибка запроса к бд");       
      break;       
  case 100 : 
      er_id = DefCodec->toUnicode("Ошибка 100 :");
      er_str = DefCodec->toUnicode("Сумма сдачи не должна превышать значение максимальной денежной купюры");       
      break;
  case 101 :
      er_id = DefCodec->toUnicode("Ошибка 101 :");
      er_str = DefCodec->toUnicode("В базе данных отсутствует информация о фиксированной скидке с таким штрих кодом"); 
      break;	             
  case 102 :
      er_id = DefCodec->toUnicode("Ошибка 102 :");
      er_str = DefCodec->toUnicode("Продажа дробного количества товара запрещена в настройках программы");       
      break;
  case 103 :
      er_id = DefCodec->toUnicode("Ошибка 103 :");
      er_str = DefCodec->toUnicode("Редактирование цены запрещено в карточке товара");       
      break;	                   
  case 104 :
      er_id = DefCodec->toUnicode("Ошибка 104 :");
      er_str = DefCodec->toUnicode("Не удалось получить информацию для отчета");       
      break;      
  case 105:   
      er_id = DefCodec->toUnicode("Ошибка 105 : ");
      er_str = DefCodec->toUnicode("Отсутствует информация о шаблонах документов");    
      break;  
  case 106:   
      er_id = DefCodec->toUnicode("Ошибка 106 : ");
      er_str = DefCodec->toUnicode("У некоторых позиций сумма скидки превышает максимально допустимую. Скидка отменена");    
      break;  	      
  case 107:   
      er_id = DefCodec->toUnicode("Ошибка 107 : ");
      er_str = DefCodec->toUnicode("Цена товара меньше его минимальной цены");    
      break;        
  case 108:   
      er_id = DefCodec->toUnicode("Ошибка 108 : ");
      er_str = DefCodec->toUnicode("Продажа данного товара ограничена по времени");    
      break;          
  case 109:   
      er_id = DefCodec->toUnicode("Ошибка 109 : ");
      er_str = DefCodec->toUnicode("Для данного типа ФР запрещена комбинированная оплата чеков");    
      break;          
  case 110:   
      er_id = DefCodec->toUnicode("Ошибка 110 : ");
      er_str = DefCodec->toUnicode("Недостаточно наличности в данной секции");    
      break;          
  case 111:   
      er_id = DefCodec->toUnicode("Ошибка 111 : ");
      er_str = DefCodec->toUnicode("В одной из секций недостаточно наличности для возврата");    
      break;                
  case 112:   
      er_id = DefCodec->toUnicode("Ошибка 112 : ");
      er_str = DefCodec->toUnicode("Оплата чеков кредитом в режиме работы с несколькими секциями заблокирована");    
      break; 
  //-----(+)-----Zuskin-----03/08/2011-----  
  case 113:   
      er_id = DefCodec->toUnicode("Ошибка 113 : ");
      er_str = DefCodec->toUnicode("Товар с указанной ценой не найден");    
      break;        
  case 114:   
      er_id = DefCodec->toUnicode("Ошибка 114 : ");
      er_str = DefCodec->toUnicode("Внешняя дисконтная система не настроена");    
      break;        
  //---------------------------------------
  //-----(+)-----Zuskin-----13/10/2011-----
  case 115:   
      er_id = DefCodec->toUnicode("Ошибка 115 : ");
      er_str = DefCodec->toUnicode("Не введено количество/вес товара!");    
      break;      
  //---------------------------------------
  //-----(+)-----Zuskin-----24/10/2011-----
  case 116:   
      er_id = DefCodec->toUnicode("Ошибка 116 : ");
      er_str = DefCodec->toUnicode("Нет ни одного отложенного чека!");    
      break;  
  case 117:   
      er_id = DefCodec->toUnicode("Ошибка 117 : ");
      er_str = DefCodec->toUnicode("В БД нет отложенного чека с таким номером!");    
      break;  
  //---------------------------------------
  //-----(+)-----Zuskin-----27/10/2011-----
  case 118:   
      er_id = DefCodec->toUnicode("Ошибка 118 : ");
      er_str = DefCodec->toUnicode("Невозможно отложить чек без позиций!");    
      break;  
  case 119:   
      er_id = DefCodec->toUnicode("Ошибка 119 : ");
      er_str = DefCodec->toUnicode("Невозможно отложить чек: все позиции сторнированы!");    
      break;                        
  //---------------------------------------
  //-----(+)-----Zuskin-----18/11/2011----- 
  case 120:   
      er_id = DefCodec->toUnicode("Ошибка 120 : ");
      er_str = DefCodec->toUnicode("Список менеджеров-консультантов пуст!");    
      break;  
  //---------------------------------------
  
  }  
}

void kkm_Error(QString er_id, QString er_str)
{
    QTextCodec * HwCodec = QTextCodec::codecForName(HWCODEC);
    InterfaceErrorWin(HwCodec->toUnicode(er_str), HwCodec->toUnicode(er_id));
}	 


void WareReferenceErrorText(int er, QString & er_str, QString & er_id)
{
QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(er)
  {
  case 1 :      {
      er_id = DefCodec->toUnicode("Ошибка 1 :");
      er_str = DefCodec->toUnicode("Не введено наименование автоматической скидки!\n");      break;      }
  case 2 :      {
      er_id = DefCodec->toUnicode("Ошибка 2 :");
      er_str = DefCodec->toUnicode("Ошибка запроса к базе данных!\n");      break;      }
  case 3 :      {
      er_id = DefCodec->toUnicode("Ошибка 3 :");
      er_str = DefCodec->toUnicode("Такое наименование автоматической скидки уже существует в базе!\n");      break;      }
  case 4 :      {
      er_id = DefCodec->toUnicode("Ошибка 4 :");
      er_str = DefCodec->toUnicode("Не введено наименование автоматической скидки для печати на документах!\n");      break;      }
  case 5 :      {
      er_id = DefCodec->toUnicode("Ошибка 5 :");
      er_str = DefCodec->toUnicode("Не введено значение автоматической скидки!\n");      break;      }
  case 6 :      {
      er_id = DefCodec->toUnicode("Ошибка 6 :");
      er_str = DefCodec->toUnicode("Значение процентной скидки/надбавки не может превышать 100%!\n");      break;      }
  case 7 :      {
      er_id = DefCodec->toUnicode("Ошибка 7 :");
      er_str = DefCodec->toUnicode("Не введено наименование схемы автоматических скидок!\n");      break;      }
  case 8 :       {
      er_id = DefCodec->toUnicode("Ошибка 8 :");
      er_str = DefCodec->toUnicode("Не введено наименование схемы автоматических скидок для печати на документах!\n");      break;      }
  case 9 :       {
      er_id = DefCodec->toUnicode("Ошибка 9 :");
      er_str = DefCodec->toUnicode("Не введено наименование налоговой группы!\n");      break;      }
  case 10 :      {
      er_id = DefCodec->toUnicode("Ошибка 10 :");
      er_str = DefCodec->toUnicode("Не введено наименование налоговой группы для печати на документах!\n");      break;      }
  case 11 :      {
      er_id = DefCodec->toUnicode("Ошибка 11 :");
      er_str = DefCodec->toUnicode("Не введено наименование налоговой ставки!\n");      break;      }
  case 12 :      {
      er_id = DefCodec->toUnicode("Ошибка 12 :");
      er_str = DefCodec->toUnicode("Такое наименование налоговой ставки уже существует в базе\n");      break;      }
  case 13 :      {
      er_id = DefCodec->toUnicode("Ошибка 13 :");
      er_str = DefCodec->toUnicode("Не введено наименование налоговой ставки для печати на документах!\n");      break;      }
  case 14 :      {
      er_id = DefCodec->toUnicode("Ошибка 14 :");
      er_str = DefCodec->toUnicode("Не введено значение ставки!\n");      break;      }
  case 15 :      {
      er_id = DefCodec->toUnicode("Ошибка 15 :");
      er_str = DefCodec->toUnicode("Значение ставки не может превышать 100%!\n");
      break;      }
  case 16 :      {
      er_id = DefCodec->toUnicode("Ошибка 16 :");
      er_str = DefCodec->toUnicode("Не введено название товара!\n");      break;      }
  case 17 :      {
      er_id = DefCodec->toUnicode("Ошибка 17 :");
      er_str = DefCodec->toUnicode("Запрещено создавать товары с одинаковыми названиями!\n");      break;      }
  case 18 :      {
      er_id = DefCodec->toUnicode("Ошибка 18 :");
      er_str = DefCodec->toUnicode("Не введена цена товара!\n");      break;      }
  case 19:      {
      er_id = DefCodec->toUnicode("Ошибка 19 :");
      er_str = DefCodec->toUnicode("Цена товара не может равняться 0!\n");      break;      }
  case 20 :      {
      er_id = DefCodec->toUnicode("Ошибка 20 :");
      er_str = DefCodec->toUnicode("Длина штрих-кода не может превышать 20 символов!\n");      break;      }
  case 21 :      {
      er_id = DefCodec->toUnicode("Ошибка 21 :");
      er_str = DefCodec->toUnicode("Минимальная цена товара не может быть больше его цены!\n");      break;      }
  case 22 :      {
      er_id = DefCodec->toUnicode("Ошибка 22 :");
      er_str = DefCodec->toUnicode("Минимальное количество товара на продажу не может быть больше его количества!\n");      break;      }  
  case 23 :      {
      er_id = DefCodec->toUnicode("Ошибка 23 :");
      er_str = DefCodec->toUnicode("Такой штрих-код уже существует в базе!\n");      break;      } 
  case 24 :      {
      er_id = DefCodec->toUnicode("Ошибка 24 :");
      er_str = DefCodec->toUnicode("Фатальная ошибка запроса к БД!\n");      break;      } 
  case 25 :      {
      er_id = DefCodec->toUnicode("Ошибка 25 :");
      er_str = DefCodec->toUnicode("Для удаления товара его необходимо выбрать!\n");      break;      } 
  case 26 :      {
      er_id = DefCodec->toUnicode("Ошибка 26 :");
      er_str = DefCodec->toUnicode("Для редактирования товара его необходимо выбрать!\n");      break;      } 
  case 27 :      {
      er_id = DefCodec->toUnicode("Ошибка 27 :");
      er_str = DefCodec->toUnicode("В базе нет ни одной группы схем автоматических скидок!\n");      break;      }  
  case 30 :      {
      er_id = DefCodec->toUnicode("Ошибка 30 :");
      er_str = DefCodec->toUnicode("Необходимо выделить скидку для ее редактирования!\n");      break;      } 
  case 31 :      {
      er_id = DefCodec->toUnicode("Ошибка 31:");
      er_str = DefCodec->toUnicode("В данной группе нет ни одной автоматической скидки!\n");      break;      } 
  case 32 :      {
      er_id = DefCodec->toUnicode("Ошибка 32 :");
      er_str = DefCodec->toUnicode("Необходимо выделить скидку для удаления!\n");      break;      } 
  case 33 :      {
      er_id = DefCodec->toUnicode("Ошибка 33 :");
      er_str = DefCodec->toUnicode("Такое название автоматической схемы уже существует в базе!\n");      break;      } 
  case 34 :      {
      er_id = DefCodec->toUnicode("Ошибка 34 :");
      er_str = DefCodec->toUnicode("Необходимо выделить группу товаров для создания в ней новой группы!\n");      break;      } 
  case 35 :      {
      er_id = DefCodec->toUnicode("Ошибка 35 :");
      er_str = DefCodec->toUnicode("Необходимо выделить группу товаров для ее удаления!\n");      break;      } 
  case 36 :      {
      er_id = DefCodec->toUnicode("Ошибка 36 :");
      er_str = DefCodec->toUnicode("Такое название налоговой группы уже существует в базе!\n");      break;      } 
  case 37 :      {
      er_id = DefCodec->toUnicode("Ошибка 37 :");
      er_str = DefCodec->toUnicode("Нельзя создать ставку вне группы!\n");      break;      } 
  case 38 :      {
      er_id = DefCodec->toUnicode("Ошибка 38 :");
      er_str = DefCodec->toUnicode("В данной группе нет ни одной налоговой ставки!\n");      break;      } 
  case 39 :      {
      er_id = DefCodec->toUnicode("Ошибка 39 :");
      er_str = DefCodec->toUnicode("Необходимо выделить ставку для ее редактирования!\n");      break;      } 
  case 40 :      {
      er_id = DefCodec->toUnicode("Ошибка 40 :");
      er_str = DefCodec->toUnicode("Название схемы должно содержать хотя бы один символ!\n");      break;      }
  case 41 :      {
      er_id = DefCodec->toUnicode("Ошибка 41 :");
      er_str = DefCodec->toUnicode("Необходимо выделить ставку для ее удаления!\n");      break;      }      
  case 42 :      {
      er_id = DefCodec->toUnicode("Ошибка 42 :");
      er_str = DefCodec->toUnicode("Название новой группы товаров для печати на документах не задано!\nГруппа товаров не может быть создана!");      break;      }  
  case 43 :      {
      er_id = DefCodec->toUnicode("Ошибка 43 :");
      er_str = DefCodec->toUnicode("Значение коэффициента товара слишком велико!");      break;      }    
  case 44 :      {
      er_id = DefCodec->toUnicode("Ошибка 44 :");
      er_str = DefCodec->toUnicode("Штрих-код содержит недопустимые символы!");      break;      }      
  case 45 :      {
      er_id = DefCodec->toUnicode("Ошибка 45 :");
      er_str = DefCodec->toUnicode("Для копирования товара его необходимо выбрать!\n");      break;      }  
  case 46 :      {
      er_id = DefCodec->toUnicode("Ошибка 46 :");
      er_str = DefCodec->toUnicode("Такое название группы товаров уже существует в базе!\n");      break;      }
  case 47 :      {
      er_id = DefCodec->toUnicode("Ошибка 47 :");
      er_str = DefCodec->toUnicode("Название родительской группы не должно совпадать с названием группы\n");      break;      }
  case 48 :      {
      er_id = DefCodec->toUnicode("Ошибка 48 :");
      er_str = DefCodec->toUnicode("Коэффициент штрих-кода для штучного товара не может быть дробным\n");      break;      }
  case 49 :      {
      er_id = DefCodec->toUnicode("Ошибка 49 :");
      er_str = DefCodec->toUnicode("В базе нет таблицы Flags\n");      break;      }    
  case 50 :      {
      er_id = DefCodec->toUnicode("Ошибка 50 :");
      er_str = DefCodec->toUnicode("Коэффициент штрих-кода содержит недопустимые символы\n");      break;      } 
  case 51 :      {
      er_id = DefCodec->toUnicode("Ошибка 51 :");
      er_str = DefCodec->toUnicode("Коэффициент штрих-кода не может превышать число 9999999.999\n");      break;      }         
   case 52 :      {
      er_id = DefCodec->toUnicode("Ошибка 52 :");
      er_str = DefCodec->toUnicode("Такой штрих-код уже задан для этого товара!\n");      break;      }  
   case 53 :      {
      er_id = DefCodec->toUnicode("Ошибка 53 :");
      er_str = DefCodec->toUnicode("Ответ сервера непонятен!\n");      break;      } 
   case 54 :      {
	   er_id = DefCodec->toUnicode("Ошибка 54 :");
	   er_str = DefCodec->toUnicode("Непредвиденные проблемы с сервером!\n");break;}
   case 55 :	  {
	   er_id = DefCodec->toUnicode("Ошибка 55 :");
	   er_str = DefCodec->toUnicode("Не удалось получить настройки интерфейса. Работа будет продолженна с настройками по умолчанию");break;}
   case 56:    {
	   er_id = DefCodec->toUnicode("Ошибка 56 :");
	   er_str = DefCodec->toUnicode("Не удалось получить все настройки интерфейса. Для отсутствующих настроек будут использованны значения по умолчанию");break;}
   case 57:    {
	   er_id = DefCodec->toUnicode("Ошибка 57 :");
	   er_str = DefCodec->toUnicode("Не найден идентификатор налоговой группы/ставки");break;} 
   case 58:    {
	   er_id = DefCodec->toUnicode("Ошибка 58 :");
	   er_str = DefCodec->toUnicode("Не найден порядок начисления налоговой ставки в группе");break;} 
   case 59:    {
	   er_id = DefCodec->toUnicode("Ошибка 59 :");
	   er_str = DefCodec->toUnicode("Для этого товара уже задан такой штрих-код");break;} 	     
   case 60:    {
	   er_id = DefCodec->toUnicode("Ошибка 60 :");
	   er_str = DefCodec->toUnicode("Данный внутренний код уже используется"); break;} 	    	    
   case 61:    {
	   er_id = DefCodec->toUnicode("Ошибка 61 :");
	   er_str = DefCodec->toUnicode("Внутренний код товара не может быть пустым");  break;}  	 
	     
   default:{
	er_id = DefCodec->toUnicode("Ошибка -10 :");
	er_str = DefCodec->toUnicode("Ошибка -10 :\nНеизвестная ошибка\n");break;
     }
  } 
}


void WareReferenceMessageText(int mes, QString & mes_str, QString & mes_id)
{
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(mes)  {
    case 1 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Автоматическая скидка успешно добавлена!\n");      break;    }
    case 2 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Товар успешно добавлен!\n");      break;    }
    case 3 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Автоматическая скидка успешно отредактирована!\n");      break;    }
    case 4 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Схема автоматических скидок успешно отредактирована!\n");      break;    }
    case 5 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Налоговая группа успешно отредактирована!\n");      break;    }
    case 6 :    {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Товар успешно отредактирован!\n");      break;    }
    case 7 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Группа автоматических скидок успешно создана!\n");      break;    }	
    case 8 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Схема успешно удалена!\n");      break;    }	
    case 9 :    {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Налоговая группа успешно создана!\n");      break;    }
    case 10 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Налоговая группа успешно удалена!\n");      break;    }
    case 11 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Налоговая ставка успешно удалена!\n");      break;    }	
    case 12 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Нельзя переместить ставку на уровень выше!\n");      break;    }
     case 13 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Нельзя переместить ставку на уровень ниже!\n");      break;    }
     case 14 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Налоговая ставка успешно создана!\n");      break;    }	
     case 15 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Налоговая ставка успешно отредактирована!\n");      break;    }
     case 16 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Группа товаров успешно добавлена!\n");      break;    }
     case 17 :     {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Группа товаров успешно отредактирована!\n");      break;    
  }	
	 default:{
	 mes_id = DefCodec->toUnicode("Ошибка -10 :");
	 mes_str = DefCodec->toUnicode("Ошибка -10 :\nНеизвестная ошибка\n");break;
     } 
  }
}


/*
void HandleUnloadErrorText(int er, QString & er_str, QString & er_id)
{
QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(er)
  {
  case 1 :
      {
      er_id = DefCodec->toUnicode("Ошибка 1 :");
      er_str = DefCodec->toUnicode("Дата и время окончания не может превышать дату и время начала выгрузки отчета!\n");
      break;
      }
   case 2 :
      {
      er_id = DefCodec->toUnicode("Ошибка 2 :");
      er_str = DefCodec->toUnicode("Дата и время начала/окончания превышает текущую дату и время!\n");
      break;
      }
  }
}

void HandleUnloadMessageText(int mes, QString & mes_str, QString & mes_id)
{
  QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
  switch(mes)
  {
    case 1 : 
    {
      mes_id = DefCodec->toUnicode("Сообщение:");
      mes_str = DefCodec->toUnicode("Отчет сформирован!\n");
      break;
    }
    }
}

*/







