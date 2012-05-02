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
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
extern SP sp;
extern BlockTimer *bt;

#include <vector>
#include <map>
#include <algorithm>

//#include "function_objects_stl.h"
using namespace std;

QTextCodec* DefCodec = QTextCodec::codecForName(SYSCODEC);

//redSum_map m_sum;
//discount_map m_discount; 
//discount_map m_input_discount; // будем хранить тут исходный набор скидок

//checkBox_vector v_chbox;
//textLabel_vector v_tlabel;
//textLabel_vector v_mlabel;

//focus_map m_foc;

//map<int,int> foc_to_ind;


//less_d less_discount;


//===============================================================//
void canceldiscountForm::ClearStlObjects()
{
    m_discount.clear(); 
    m_input_discount.clear();
    
    v_chbox.clear();
    v_tlabel.clear();
    //v_mlabel.clear();
    
    m_foc.clear();
    foc_to_ind.clear();
}

//===============================================================//
void canceldiscountForm::init()
{
    //puts("canceldiscountForm::init()");
    
    // DefCodec = QTextCodec::codecForName(SYSCODEC);
    
    //Пока автоматическая скидка не используется, скроем эти поля
    //autoCheckBox->hide();
    //autoTextLabel->hide();
    //markautoPixmapLabel->hide();
    
    // Пока карты не используются, скроем эти поля
    //-----(-)-----Zuskin-----23/12/2011-----
    //cardCheckBox->hide();
    //cardTextLabel->hide();
    //------------------------------------------------
    //markcardPixmapLabel->hide();
    
    // Алгоритм начисления скидок и надбавок "Объединение" или "Суммирование"
    if ( sp.result_discount != 0 )   {
	markautoPixmapLabel->hide();
	markfixPixmapLabel->hide();
	markhandPixmapLabel->hide();
	markcardPixmapLabel->hide();
    }
    
    
    for ( int i = 0; i < 4; i++ )  {
	choose[i] = false;
	was_checked[i] = false;
    }
    
    v_chbox.push_back(autoCheckBox);
    v_chbox.push_back(fixCheckBox);
    v_chbox.push_back(handCheckBox);
    v_chbox.push_back(cardCheckBox);
    
    v_tlabel.push_back(autoTextLabel);
    v_tlabel.push_back(fixTextLabel);
    v_tlabel.push_back(handTextLabel);
    v_tlabel.push_back(cardTextLabel);
    
    v_mlabel.push_back(markautoPixmapLabel);
    v_mlabel.push_back(markfixPixmapLabel);
    v_mlabel.push_back(markhandPixmapLabel);
    v_mlabel.push_back(markcardPixmapLabel);
    
    grabKeyboard();
    adjustSize();
    if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
    
    discount_num = 0;
    
}

//===============================================================//
void canceldiscountForm::destroy()
{
    ClearStlObjects();
}

//===============================================================//
void canceldiscountForm::Markout(int index, const QColor color)
{
    v_chbox.at(index)->setPaletteForegroundColor(color);
    v_tlabel.at(index)->setPaletteForegroundColor(color);
}

//===============================================================//
void canceldiscountForm::MarkoutMaxDiscount(int index)
{
    //puts("void canceldiscountForm::MarkoutMaxDiscount(int index)");
    //QColor color(65, 90, 255);
    //Markout(index, color);
    
    //v_mlabel.at(index)->setEnabled(true);  
    v_mlabel.at(index)->show();
}

//===============================================================//
void canceldiscountForm::MarkoutMaxCharge(int index)
{
    //puts("void canceldiscountForm::MarkoutMaxCharge(int index)");
    //QColor color(255, 46, 77);
    //Markout(index, color);
    
    //v_mlabel.at(index)->setEnabled(true);
    v_mlabel.at(index)->show();
}

//===============================================================//
void canceldiscountForm::MarkoutMinMax()
{
    //redSum_map::iterator max_discount_it = max_element(m_sum.begin(), m_sum.end(), less_sum);
    // redSum_map::iterator max_charge_it = min_element(m_sum.begin(), m_sum.end(), less_sum);
    //puts("void canceldiscountForm::MarkoutMinMax()");
    //QColor color(0,0,0);
    
    for ( int i = 0; i < 4; i++ )  {
	//Markout(i, color);
	//v_mlabel.at(i)->setEnabled(false);
	v_mlabel.at(i)->hide();
    }
    
    
    discount_map::iterator max_discount_it = max_element(m_discount.begin(), m_discount.end(), less_discount);
    discount_map::iterator max_charge_it = min_element(m_discount.begin(), m_discount.end(), less_discount);
    
    //puts("=====MAX======");
    //cout << (*max_discount_it).first << " ==> " << (*max_discount_it).second.dsum << endl;
    //puts("=============");
    
    //puts("=====MIN======");
    //cout << (*max_charge_it).first << " ==> " << (*max_charge_it).second.dsum << endl;
    //puts("=============");
    
    
    if ( (*max_charge_it).second.dsum != 0 )    MarkoutMaxCharge((*max_charge_it).first); 
    if ( (*max_discount_it).second.dsum != 0 )    MarkoutMaxDiscount((*max_discount_it).first);
}

//===============================================================//
int  canceldiscountForm::GetMaybeDiscounts(Receipt *rec, int i)
{
    //puts("canceldiscountForm::GetMaybeDiscounts(Receipt *rec, int i = %d)\n");
    //int ret = 0;
    pos_index = i;
    receipt = rec;
    
    // Скидки на позицию
    if ( i != -1 )   {
	area_flag = false;
	// нет скидок
	if ( rec->GetPosElemRef(i)->RecalculationDiscounts() == 0 ) return -1; 
	
	// заполняем вектор сумм скидок/надбавок
	for ( int k = 0; k < 4; k++ )       {
	    m_discount.insert(pair<int, Discount>(k, *(rec->GetPosElemRef(i)->GetDiscountElemRef(k))));
	    // m_sum[k] = rec->GetPosElemRef(i)->GetDiscountElemRef(k)->dsum;
	    choose[k] = rec->GetPosElemRef(i)->GetDiscountElemRef(k)->df;
	}
    } else   {
	// Скидки на чек
	area_flag = true;
	if ( rec->dsum == 0 ) return -2;
	// заполняем вектор сумм скидок/надбавок
	for ( int k = 0; k < 4; k ++ )      {
	    m_discount[k] = *(rec->GetDiscountElemRef(k));
	    choose[k] = rec->GetDiscountElemRef(k)->df;
	}
    }
    m_input_discount.insert(m_discount.begin(), m_discount.end());
    discount_num = InitDiscountsCheckBoxs();
    if ( discount_num !=  0 ) InitFocus();
    // //printf("return discount_num = %d \n");
    return discount_num;
}

//===============================================================//
void canceldiscountForm::SetTextCharge(int i) 
{
    if ( i == 2 ) { v_chbox.at(i)->setText(DefCodec->toUnicode("Отменить произвольную надбавку")); return; }
    if ( i == 1 ) { v_chbox.at(i)->setText(DefCodec->toUnicode("Отменить фиксированную надбавку")); return; }
    if ( i == 0 ) { v_chbox.at(i)->setText(DefCodec->toUnicode("Отменить автоматическую надбавку")); return;}
    
}
//===============================================================//
int canceldiscountForm::InitDiscountsCheckBoxs()
{
    //puts("int canceldiscountForm::InitDiscountsCheckBoxs()");
    QString s;
    int num = 0;
    for ( int k = 0; k < 4; k++ )  {
	if ( choose[k] )    {
	    v_chbox.at(k)->setEnabled(true);
	    v_tlabel.at(k)->setEnabled(true);
	    //v_tlabel.at(k)->setText(s.setNum(m_sum[k], 'f', 2));
	    if ( m_discount[k].dsum < 0 )  SetTextCharge(k);
	    v_tlabel.at(k)->setText(s.setNum(m_discount[k].dsum, 'f', 2));
	    num++;
	}
    }
    
    // алгоритм начисления скидок/надбавок - "максимальная", выделим действующие скидку и надбавку 
    if ( sp.result_discount == 0 )     MarkoutMinMax();
    if ( area_flag ) allTextLabel->setText(s.setNum(receipt->RecalculationDiscounts(), 'f', 2));
    else allTextLabel->setText(s.setNum(receipt->GetPosElemRef(pos_index)->RecalculationDiscounts(), 'f', 2));

    if ( num > 1 )  {
	allCheckBox->setEnabled(true);
	allTextLabel->setEnabled(true);
    }
    //printf("return num = %d \n", num);
    return num;
}

//===============================================================//
void canceldiscountForm::InitFocus()
{
    //puts("void canceldiscountForm::InitFocus()");
    int ind = -1;
    foc = -1;
    if ( discount_num > 1 )  {
	foc = 0;
	m_foc[0] = allCheckBox;
    }
    
    for ( discount_map::iterator it = m_discount.begin(); it != m_discount.end(); it++ )  {
	ind++;
	if ( it->second.df )    {
	    foc ++;
	    foc_to_ind[foc] = ind; 
	    //cout << "foc = " << foc << endl; 
	    m_foc.insert(make_pair(foc, v_chbox.at(it->first)));
	}
    }
    
    QFont f;
    f.setBold(true);
    
    m_foc.begin()->second->setFont(f);
    foc = 0; 
}

//===============================================================//
void canceldiscountForm::SetFocusUp()
{
    //puts("canceldiscountForm::SetFocusUp()");
    //cout << "m_foc.size() = " << m_foc.size() << endl;
    int last_el = m_foc.size() - 1;
    if ( last_el != 0 )   {
	QFont f, g;
	f.setBold(true);
	
	m_foc[foc]->setFont(g);

	if ( foc == 0 ) foc = last_el;
	else foc--;
	
	m_foc[foc]->setFont(f);
    }
}

//===============================================================//
void canceldiscountForm::SetFocusDown()
{
    //puts("canceldiscountForm::SetFocusDown()");
    int last_el = m_foc.size() - 1;
    if ( last_el != 0 )  {
	QFont f, g;
	f.setBold(true);
	
	m_foc[foc]->setFont(g);
	
	if ( foc == last_el ) foc = 0;
	else foc++;
	
	m_foc[foc]->setFont(f);
    }
}

//===============================================================//
void canceldiscountForm::CancelOneReduction(int i, bool restore_flag)
{    
    printf("canceldiscountForm::CancelOneReduction(%d, bool restore_flag) \n", i);
    
    QString s;
    double sum = 0;
    double percent = 0;
    bool flag = false;
    if ( restore_flag )  {
	sum = m_input_discount[i].dsum;
	percent = m_input_discount[i].percent;
	flag = m_input_discount[i].df;
    }
    
    m_discount[i].dsum = sum;
    m_discount[i].percent = percent;
    m_discount[i].df = flag;
    
    if ( area_flag )  {
	receipt->GetDiscountElemRef(i)->dsum = sum;
	receipt->GetDiscountElemRef(i)->percent = percent;
	receipt->GetDiscountElemRef(i)->df = flag;
    }  else  {
	receipt->GetPosElemRef(pos_index)->GetDiscountElemRef(i)->dsum = sum;
	receipt->GetPosElemRef(pos_index)->GetDiscountElemRef(i)->percent = percent;
	receipt->GetPosElemRef(pos_index)->GetDiscountElemRef(i)->df = flag;
    }
    
    if ( area_flag ) {
	receipt->transaction_discount_sum = receipt->RecalculationDiscounts(); 
	printf("transaction_discount_sum = %f \n", receipt->transaction_discount_sum);
	receipt->RecalcTransactionDiscount();	
    } else {
	receipt->RecalculationDiscounts(); 
    }
    
    
}

//===============================================================//
void canceldiscountForm::PositionChecked()
{
    
    QString s;
    
    //m_foc[foc]->setChecked(! m_foc[foc]->isChecked()); // Смена состояния checkbox-а
    
    bool restore_flag;
    
    if ( m_foc[foc]->isChecked() ) restore_flag = false;  // отменить скидку
    else  restore_flag = true;  // восстановит скидку
    
    printf("PositionChecked() foc = %d \n", foc);
    //printf("foc_to_ind.size() = %d \n", (int)foc_to_ind.size());
    
    // отмена всех скидок
    if ( (foc == 0) && (discount_num != 1) )  {
	for ( unsigned int i = 1; i <= foc_to_ind.size(); i++ )    {
	    if ( !restore_flag ) {
//-----(*)-----Zuskin-----23/12/2011-----
	/*v_chbox.at(foc + i)->setChecked(false);
	    }      else {
		v_chbox.at(foc + i)->setChecked(was_checked[foc_to_ind[foc + i]]);
	    }	
	    
	    if ( !was_checked[foc_to_ind[foc + i]] )  CancelOneReduction(foc_to_ind[foc + i], restore_flag);
	    if ( area_flag )   v_tlabel.at(foc + i)->setText(s.setNum(receipt->GetDiscountElemRef(i)->dsum, 'f', 2));
	    else v_tlabel.at(foc + i)->setText(s.setNum(receipt->GetPosElemRef(pos_index)->GetDiscountElemRef(i)->dsum, 'f', 2));
	    
	    v_tlabel.at(foc + i)->setEnabled(restore_flag);
	    v_chbox.at(foc + i)->setEnabled(restore_flag);     */
	v_chbox.at(foc_to_ind[foc + i])->setChecked(false);
	    }      else {
		v_chbox.at(foc_to_ind[foc + i])->setChecked(was_checked[foc_to_ind[foc + i]]);
	    }	
	    
	    if ( !was_checked[foc_to_ind[foc + i]] )  CancelOneReduction(foc_to_ind[foc + i], restore_flag);
	    if ( area_flag )   v_tlabel.at(foc_to_ind[foc + i])->setText(s.setNum(receipt->GetDiscountElemRef(foc_to_ind[foc + i])->dsum, 'f', 2));
	    else v_tlabel.at(foc_to_ind[foc + i])->setText(s.setNum(receipt->GetPosElemRef(pos_index)->GetDiscountElemRef(foc_to_ind[foc + i])->dsum, 'f', 2));
	    
	    v_tlabel.at(foc_to_ind[foc + i])->setEnabled(restore_flag);
	    v_chbox.at(foc_to_ind[foc + i])->setEnabled(restore_flag);    
//--------------------------------------------------

	} 
    }  else   {
	// отмена одной из скидок
	CancelOneReduction(foc_to_ind[foc], restore_flag);
	was_checked[foc_to_ind[foc]] = !restore_flag;
//-----(*)-----Zuskin-----23/12/2011-----	
//	if ( area_flag )      v_tlabel.at(foc)->setText(s.setNum(receipt->GetDiscountElemRef(foc_to_ind[foc])->dsum, 'f', 2));
//	else      v_tlabel.at(foc)->setText(s.setNum(receipt->GetPosElemRef(pos_index)->GetDiscountElemRef(foc_to_ind[foc])->dsum, 'f', 2));
	if ( area_flag )      v_tlabel.at(foc_to_ind[foc])->setText(s.setNum(receipt->GetDiscountElemRef(foc_to_ind[foc])->dsum, 'f', 2));
	else      v_tlabel.at(foc_to_ind[foc])->setText(s.setNum(receipt->GetPosElemRef(pos_index)->GetDiscountElemRef(foc_to_ind[foc])->dsum, 'f', 2));
//-------------------------------------------------	
    }	  
    
    //for(int i = 0; i < 4; i ++)
    //  cout<< "was_checked[" << i << "]="  << was_checked[i] << endl;
    
    //  bool all_enable = (choose[0] && was_checked[0]) || (choose[1] && was_checked[1]) || (choose[2] && was_checked[2]) || (choose[3] && was_checked[3]);
    //cout << "all_enable = " << all_enable << endl;
    //  allTextLabel->setEnabled(!all_enable);
    //  allCheckBox->setEnabled(!all_enable);
    
    
    // алгоритм начисления скидок/надбавок - "максимальная", выделим действующие скидку и надбавку 
    if ( sp.result_discount == 0 ) 	MarkoutMinMax();
    
    if ( area_flag )    allTextLabel->setText(s.setNum(receipt->RecalculationDiscounts(), 'f', 2));
    else    allTextLabel->setText(s.setNum(receipt->GetPosElemRef(pos_index)->RecalculationDiscounts(), 'f', 2));  
}

//===============================================================//
void canceldiscountForm::close()
{	
    for ( int i = 0; i < 4; i++ )      {
	choose[i] = false;
	CancelOneReduction(i, true);
    }
    releaseKeyboard();
    reject();
    
}
//===============================================================//
void canceldiscountForm::keyPressEvent(QKeyEvent *k)
{
    
    if ( sp.autoblock_flag ) bt->action_time = QTime::currentTime();
    switch ( k->key() )  {
    case Qt::Key_F12 :    {	
	    close();
	    break;
	}
    case Qt::Key_Up :    {
	    SetFocusUp();
	    break;
	}
    case Qt::Key_Down :    {
	    SetFocusDown();
	    break;
	}
    case Qt::Key_Enter :    {
	    releaseKeyboard();
	    accept();
	    //CancelReductions();
	    break;
	}
    case Qt::Key_Return :    {
	    releaseKeyboard();
	    accept();
	    //CancelReductions();
	    break;
	}
    case Qt::Key_Asterisk :    {
	    m_foc[foc]->setChecked(! m_foc[foc]->isChecked()); // Смена состояния checkbox-а
	    PositionChecked();
	    break;
	}
    case Qt::Key_Space :    {
	    m_foc[foc]->setChecked(! m_foc[foc]->isChecked()); // Смена состояния checkbox-а
	    PositionChecked();
	    break;
	}	
    }
}

//===============================================================//
void canceldiscountForm::autoCheckBox_clicked()
{
    //puts("void canceldiscountForm::autoCheckBox_clicked()");
}
//===============================================================//
void canceldiscountForm::fixCheckBox_clicked()
{
    //puts("void canceldiscountForm::fixCheckBox_clicked()");
    QFont f, g;
    f.setBold(true);   
    //printf("foc = [%d]\n",foc);
    m_foc[foc]->setFont(g);
    ////puts("after setFont1");
    foc = getIndex(fixCheckBox);
    m_foc[foc]->setFont(f);
    ////puts("BEFORE PositionChecked()");
    PositionChecked();
}
//===============================================================//
void canceldiscountForm::handCheckBox_clicked()
{
    ////puts("void canceldiscountForm::handCheckBox_clicked()");
    QFont f, g;
    f.setBold(true);   
    m_foc[foc]->setFont(g);
    foc = getIndex(handCheckBox);
    m_foc[foc]->setFont(f);
    PositionChecked(); 
}
//===============================================================//
void canceldiscountForm::allCheckBox_clicked()
{
    ////puts("void canceldiscountForm::allCheckBox_clicked()");
    QFont f, g;
    f.setBold(true);   
    m_foc[foc]->setFont(g);
    foc = getIndex(allCheckBox);
    m_foc[foc]->setFont(f);
    PositionChecked(); 
    
}
//===============================================================//

int canceldiscountForm::getIndex( QCheckBox* w )
{
   for (int i=0; i< m_foc.size(); i++)
    {
     if (m_foc[i] == w) return i; 
    }
   return 0;
}
//===============================================================//
