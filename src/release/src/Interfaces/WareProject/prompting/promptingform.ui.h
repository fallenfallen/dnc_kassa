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
    count_action = 9;
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
    PrompTable->setColumnWidth(1,600);
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
	 addPosition(QString("Tab"), QString( "Перемещение по объектам форм"));
	 addPosition(QString("F1"), QString("Вызов подсказки по действующим клавишам"));
	 addPosition(QString("F12"), QString("Выход"));
	 addPosition(QString("Enter"), QString("Открыть папку с товарами / открыть карточку товара для редактирования / подтверждение выполнения действия"));
	 addPosition(QString("Return"), QString("Открыть папку с товарами / открыть карточку товара для редактирования / подтверждение выполнения действия"));
	 addPosition(QString("Delete"), QString("Удаление выделенного штрих-кода"));
	 addPosition(QString("Home"), QString("Перемещение в начало строки"));
	 addPosition(QString("End"), QString("Перемещение в конец строки"));
	 addPosition(QString("BackSpace"), QString( "Удалить последний введенный символ"));
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
	    close();
	    releaseKeyboard();
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
    default:
	{
	  //  puts("Чего-то кто-то пытается ввести!!!");
	    break;
	}
    }
}




