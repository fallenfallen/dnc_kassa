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

#include "dedicationcombobox.h"
#include <qpainter.h>

DedicationComboBox::DedicationComboBox(QWidget * parent, const char * name ):QComboBox(parent,name)
{
	 QFont f;
	 f.setBold(false);
	 setFont(f);
}


void DedicationComboBox::focusInEvent ( QFocusEvent * foc)
{
   // puts("DedicationComboBox::focusInEvent");
    QFont f;
    f.setBold(true);
    setFont(f);
}

void DedicationComboBox::focusOutEvent ( QFocusEvent * foc)
{
  //  puts("DedicationComboBox::focusOutEvent");
    QFont f;
    f.setBold(false);
    setFont(f);
}
