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

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/      



void promptingForm::init()
{
    //puts("void promtingForm::init()");
    grabKeyboard();	
    DefCodec = QTextCodec::codecForName(SYSCODEC);
    //-----(*)-----Zuskin-----16/11/2011-----
    count_action = 43;
    //-------------------------------------------------
    current_action = 0;
    initTable();
    insertValueInTable();
    PrompTable->setCurrentCell(0,0);
    //PrompTable->setNumRows(count_action);
}

void promptingForm::initTable()
{
  //  puts("void promptingForm::initTable()");
    PrompTable->setColumnReadOnly(0, true);
    PrompTable->setColumnReadOnly(1,true);
    PrompTable->setColumnWidth(0,100);
    PrompTable->setColumnWidth(1,550);
    PrompTable->setSelectionMode(QTable::SingleRow);
}

void promptingForm::addPosition(QString button, QString action)
{
    PrompTable->setText(current_action, 0, DefCodec->toUnicode(button)); 
    PrompTable->setText(current_action, 1, DefCodec->toUnicode(action)); 
    current_action++;
}

void promptingForm::insertValueInTable()
{
    // puts("void promptingForm::insertValueInTable()");
    PrompTable->setNumRows(count_action);
    
    addPosition(QString("F1"), QString("Вызов подсказки по действующим клавишам"));
    addPosition(QString("F2"), QString("Регистрация товара по внутреннему коду (из поля ввода)"));
    addPosition(QString("F3"), QString("Регистрация товара по штрих-коду (из поля ввода)"));
    //-----(+)-----Zuskin-----03/08/2011-----
    addPosition(QString("F4"), QString("Регистрация товара по цене (из поля ввода)"));
    //-------------------------------------------------
    addPosition(QString("F5"), QString("Открыть окно типов оплаты"));
    addPosition(QString("F6"), QString( "Визуальный подбор товара/регистрация по наименованию (из поля ввода)"));
    
    addPosition(QString("F7"), QString("Начисление фиксированной скидки/надбавки по магнитной карте (из поля ввода)"));
    addPosition(QString("F8"), QString("Продолжить печать документа"));
    addPosition(QString("F9"), QString("Комбинированная оплата чека (кредитом)"));
    addPosition(QString("F10"), QString("Внесение наличности в кассу"));
    addPosition(QString("F11"), QString("Выплата наличности из кассы"));
    
    addPosition(QString("F12"), QString("Выход"));
    addPosition(QString("Esc"), QString("Очистить поле ввода и количество/массу"));
    addPosition(QString("BackSpace"), QString( "Удалить последний введенный символ"));
    addPosition(QString("Enter"), QString( "Комбинированная оплата по чеку (наличными)"));
    addPosition(QString("Plus"), QString("Оплата чека наличными"));    
    
    addPosition(QString("Ctrl + Plus"), QString("Оплата чека наличными, печать чека без печати позиций"));
    addPosition(QString("* (NUM)"), QString("Перенести содержимое поля ввода в поле количество/масса"));
    addPosition(QString("Insert"), QString("Промежуточный итог по текущему чеку"));
    addPosition(QString("Delete"), QString("Сторнирование выбранной позиции в чеке"));
    addPosition(QString("PageUp"), QString("Продвинуть чековую ленту"));
    
    addPosition(QString("PageDown"), QString("Открыть денежный ящик"));
    addPosition(QString("Ctrl + Home"), QString("Сумма наличных в денежном ящике"));
    addPosition(QString("Alt + Delete"), QString("Отменить чек"));
    addPosition(QString("Ctrl + Delete"), QString("Открыть чека возврата (или возврат по номеру чека)"));
    addPosition(QString("Alt + D"), QString("Начислить суммовую скидку (из поля ввода)"));    
    
    addPosition(QString("Alt + N"), QString("Начислить суммовую надбавку (из поля ввода)"));
    addPosition(QString("Ctrl + P"), QString("Печать документов"));
    addPosition(QString("Ctrl + Q"), QString("Редактировать количество (из поля ввода)"));
    addPosition(QString("Ctrl + R"), QString("Повтор последней продажи"));
    addPosition(QString("Ctrl + C"), QString("Отмена скидки/надбавки"));
    
    addPosition(QString("Ctrl + F"), QString("Начисление фиксированной скидки/надбавки"));
    addPosition(QString("Ctrl + D"), QString("Начисление процентной скидки (из поля ввода)"));
    addPosition(QString("Ctrl + N"), QString("Начисление процентной надбавки (из поля ввода)"));
    addPosition(QString("Ctrl + V"), QString("Печать копии чека по номеру (из поля ввода)"));
    addPosition(QString("Ctrl + S"), QString("Вызов формы [СЕРВИС]"));    
    
    addPosition(QString("Ctrl + B"), QString("Принудительная блокировка/смена пользователя"));
    addPosition(QString("Ctrl + 0"), QString("Ввод <00> в поле ввода"));
    addPosition(QString("Ctrl + M"), QString("Включить/выключить детектор валют"));
    addPosition(QString("Alt + P"), QString("Редактирование цены"));         
    //-----(+)-----Zuskin-----09/08/2011-----
    addPosition(QString("Ctrl + E"), QString("Запрос данных от внешней дисконтной системы"));    
    //-------------------------------------------------
    //-----(+)-----Zuskin-----10/10/2011-----
    addPosition(QString("Ctrl + L"), QString("Отложить текущий чек/восстановить отложенный ранее чек"));    
    //-------------------------------------------------
    //-----(+)-----Zuskin-----16/11/2011-----
    addPosition(QString("Ctrl + Z"), QString("Выбрать менеджера-консультанта"));    
    //-------------------------------------------------
}

void promptingForm::Down(void)
{
 //   puts("void promptingForm::Up(void)");
    int curr = PrompTable->currentRow();
    if (curr<PrompTable->numRows())
	PrompTable->setCurrentCell(curr+1,0);
}

void promptingForm::Up(void)
{
  //  puts("void promptingForm::Down(void)");
    int curr = PrompTable->currentRow();
    if (curr!=0)
	PrompTable->setCurrentCell(curr-1,0);
    
}

void promptingForm::keyPressEvent(QKeyEvent *k)
{
 //   puts("void promptingForm::keyPressEvent(QKeyEvent *k)");
    switch(k->key())
    {
    case Qt::Key_F12:
	{
	    releaseKeyboard();
	    reject();
	    return;
	}
    case Qt::Key_Up:
	{
	    //puts("Qt::Key_Up:");
	    Up();
	    break;
	}
    case Qt::Key_Down:
	{
	    //puts("Qt::Key_Down:");
	    Down();
	    break;
	}
    case Qt::Key_Return :
    case Qt::Key_Enter:	{  current_action = PrompTable->currentRow();	accept();   	    break;  	}
    default:
	{
	  //  puts("Чего-то кто-то пытается ввести!!!");
	    break;
	}
    }
}




