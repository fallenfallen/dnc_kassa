/*====================================================================================
# «ДЭНСИ:РМК» module of «ДЭНСИ: КАССА» complex is the front-office application for POS-systems based on fiscal registrators.
# © OOO "DNC Trade" 2011, Authors: Savrasov Alexandr aka Zuskin.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:РМК» программного комплекса «ДЭНСИ: КАССА» - рабочее место кассира для POS-систем на базе фискальных регистраторов
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Саврасов Александр aka Zuskin.
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
#include<qevent.h>

extern SP sp;
extern BlockTimer *bt;

void BonusCardForm::init()
{
  
    grabKeyboard();
    if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
     
}

void BonusCardForm::destroy()
{
    delete bonuspv;
}

void BonusCardForm::close()
{	
    releaseKeyboard();
    reject();
}

void BonusCardForm::keyPressEvent(QKeyEvent *k)
{
    
    if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
    switch ( k->key() )  {
    case Qt::Key_F12 :    {	
	    close();
	    break;
	}
   case Qt::Key_Enter :    {
	    releaseKeyboard();
	    accept();
	    break;
	}
    case Qt::Key_Return :    {
	    releaseKeyboard();
	    accept();
	    break;
	}
    case Qt::Key_Escape :    {
	  bonusEdit->setText("");
	  break;
	}
    case Qt::Key_Plus :    {
	  bonusEdit->setText(bonusLabel->text());
	  break;
	}
    case Qt::Key_BackSpace :  {
	  QString curTxt =  bonusEdit->text();
	  if (curTxt.length() > 0) {
	      curTxt = curTxt.left(curTxt.length() - 1);
	  }
	  bonusEdit->setText(curTxt);
	  break;
	}
    default: {
	   QString keytext = k->text(); 
	  bonusEdit->setText(bonusEdit->text()+keytext);  
	}
	
    }
}

void BonusCardForm::CommaTest( const QString & kstr)
{
    int k = kstr.length();  // длина строки
    
    string str = kstr.ascii();
    
    if ( str[k-1] == ',' ) {
	str[k-1] = '.';	
	    bonusEdit->setText(str.c_str());	
    }
}

void BonusCardForm::CheckBonus(const QString & kstr)
{
  if ( kstr.length() > 0 ) {
     QString curTxt = bonusEdit->text();
     int pos = 0;
      if (bonusEdit->validator()->validate(curTxt,pos) != QValidator::Acceptable ) {
	 bonusEdit->setText(curTxt.left(curTxt.length()-1)); 
      }
  }
}


void BonusCardForm::InitFields( double bonus_sum, double max_summ )
{
    QTextCodec * DefCodec = QTextCodec::codecForName(SYSCODEC);
    QString text = "Введите сумму для списания ( <b>max ";
    text += QString::number(max_summ) + QString("</b> ) :");
    redBonusLabel->setText(DefCodec->toUnicode(text));
    bonuspv = new PaymentSumValidator(this);
    bonusEdit->setValidator((const QValidator *)bonuspv);
    bonuspv->SetMaxSum(max_summ);
    bonusLabel->setText(QString::number(bonus_sum));
}
