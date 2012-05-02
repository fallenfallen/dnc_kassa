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

#include "striketabitm.h"
#include <qpainter.h>

//==================================================================================// 
void StrikeOutTableItem::paint(QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected)
{
//puts("StrikeOutTableItem::paint(QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected)");
  QFont f;

  f.setStrikeOut(TRUE);
  f.setFamily("Fixed[Misc]");
  f.setBold(FALSE);
  f.setPointSize(12);
  
  QColorGroup ncg = QColorGroup(cg); // makes non-const copy of the form's colorgroup [cg]
  QColor clr = QColor(210,210,210); 
  ncg.setColor(QColorGroup::Base,clr);
   
  p -> setFont(f);
  QTableItem::paint( p, ncg, cr, selected );
}
