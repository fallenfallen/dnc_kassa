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

#include <qfile.h>
#include <qdatetime.h>
#include <qtextcodec.h>
#include "printreceipt.h"
#include "printform.h"
#include <math.h>      
#include <vector>

extern QTextCodec * DefCodec;

/**********************************************************************/
void PrintReceiptDocument(Receipt * receipt, const QString & document_path)
{     
    
    QString lines;
    
    QFile file( document_path );
    if ( file.open( IO_ReadOnly ) ) {
	QTextStream stream( &file );
	
	while ( !stream.atEnd() ) lines += stream.readLine() + "\n";
	
	RemoveComment(lines);
	
	lines.replace( DATE_TAG, GetDate(QDate::currentDate()), false );
	lines.replace( TOTALSUM_TAG, GetSum(receipt->GetPositionSum()), false );
	lines.replace( DISCOUNTSUM_TAG, GetSum(receipt->GetAllDiscountSum()), false );
	lines.replace( CHECKSUM_TAG, GetSum(receipt->GetResultSum()), false );
	lines.replace( CHECKSUM_INWORDS_TAG, ConvertToWords(receipt->GetResultSum()), false );
	lines.replace( CHECKTYPE_TAG, GetCheckType(receipt->GetCheckType()), false );
	lines.replace( CHECKNUMBER_TAG, QString::number(receipt->GetCheckNumber()), false );
	lines.replace( CHECKDATE_TAG, GetReceiptDate(receipt), false );
	lines.replace( CASHIER_TAG, GetCashier(receipt), false );
	
	InsertCheckTable(lines, receipt);
    }
     
    //cout << lines << endl;
    
    file.close();
    
    PrintForm pf;
    
    pf.LoadString(lines);
    
    pf.exec();
    
}
/**********************************************************************/
// удаляет комментарии внутри html текста
void RemoveComment(QString& html_str)
{
    int positionb = 0;
    int positiona = 0; 
    
    positiona = html_str.find(HTML_COMMENT_START, positiona, false);
    while  ( positiona != -1 ) {
	positionb = html_str.find(HTML_COMMENT_END, positiona + HTML_COMMENT_START.length(), false);
	if (positionb != -1)  html_str.remove(positiona, positionb - positiona + HTML_COMMENT_END.length());
	else html_str.remove(positiona, html_str.length() - 1); 
	positiona = html_str.find(HTML_COMMENT_START, positiona, false);
    }
}
/**********************************************************************/
void InsertCheckTable(QString & html_str, Receipt * receipt) 
{
    int positionb = 0;
    int positiona = 0; 
    QString table_tags;
    positiona = html_str.find(CHECKTABLE_TAG, positiona, false);
    while  ( positiona != -1 ) {
	positionb = html_str.find(END_CHECKTABLE_TAG, positiona + CHECKTABLE_TAG.length(), false);
	if (positionb != -1)  	{
	    table_tags = html_str.mid(positiona, positionb - positiona + END_CHECKTABLE_TAG.length());
	    html_str.remove(positiona, positionb - positiona + END_CHECKTABLE_TAG.length());
	    html_str.insert(positiona, CreateCheckTable(table_tags, receipt));
	} else return;
	
	positiona = html_str.find(CHECKTABLE_TAG, positiona, false);
    }
}
/**********************************************************************/
QString CreateCheckTable(QString tags, Receipt * receipt)
{
    vector <TableColumn> columns;
    
    int position = 0;
    position = tags.find(CHECK_COLUMN_TAG, position, false);
    while ( position != -1 ) {
	int position_end = tags.find(">", position + 1, false);
	
	TableColumn tc;
	tc.FromTag(tags.mid(position, position_end - position + 1 ));
	
	columns.push_back(tc);
	
	position = tags.find(CHECK_COLUMN_TAG, position_end + 1, false);
    }	
    
    return GetReceiptTable(receipt, columns);
    
}
/**********************************************************************/
// i - номер текущей позиции
QString GetReceiptInfo(Receipt* receipt, QString value, int i)
{
    if ( value.isEmpty() ) return "";
    if ( value == "number" ) return QString::number(i + 1);
    if ( value == "name" ) return DefCodec->toUnicode(receipt->GetPosElemRef(i)->GetLongtitle());
    if ( value == "code" ) return DefCodec->toUnicode(receipt->GetPosElemRef(i)->GetCode());
    if ( value == "barcode" ) return DefCodec->toUnicode(receipt->GetPosElemRef(i)->GetBarcode());
    if ( value == "count" ) return QString::number(receipt->GetPosElemRef(i)->quantity, 'f', 3) ;
    if ( value == "price" ) return QString::number(receipt->GetPosElemRef(i)->price, 'f', 2);
    if ( value == "sum" ) return QString::number(receipt->GetPosElemRef(i)->sum, 'f', 2);
    if ( value == "discount_sum" ) return QString::number(receipt->GetPosElemRef(i)->dsum, 'f', 2);
    if ( value == "tax_group" ) return DefCodec->toUnicode(receipt->GetTaxElemRef(i)->GetName());
    if ( value == "tax_sum" ) {
	receipt->CalculationPositionTaxBase(i);
	double taxessum = 0;    
	for ( int j = 0; j < receipt->GetTaxElemRef(i)->num_rates; j++ )   
	    taxessum += receipt->GetTaxElemRef(i)->GetTaxRatesElemRef(j)->taxsum;
	return QString::number(taxessum, 'f', 2);
    }
    if ( value == "result_sum" ) return QString::number(receipt->GetPosElemRef(i)->sum - receipt->GetPosElemRef(i)->dsum, 'f', 2);
    
    return value;
    
}
/**********************************************************************/
QString GetReceiptTable(Receipt * receipt, vector <TableColumn> columns)
{    
    QString receipt_table;
    
    receipt_table += "<TABLE ALIGN = \"center\" WIDTH=\"100%\" cellspacing = \"0\" border = \"1\">\n" ; 
    
    // строка заголовков
    receipt_table += "<TR ALIGN = \"center\" >\n"; 
    for ( vector<TableColumn>::iterator it = columns.begin(); it != columns.end(); it++ ) {			
	receipt_table += "<TD WIDTH = \"";
	receipt_table += it->GetWidth();
	receipt_table +=  "\"> <P ALIGN=\"CENTER\" > <B>";
	receipt_table += it->GetCaption() ;
	receipt_table += "</B></P></TD> \n";
	//cout << "width = " << it->GetWidth() << endl;
	//cout << "caption = " << it->GetCaption() << endl;
    }
    receipt_table += "</TR>\n";
    
    // строки позиций из чека
    //-----(+)-----Zuskin-----08/11/2011-----
    int curpos = 0;
    //---------------------------------------
    for (int i = 0; i < receipt->record_num; i++ ) {
	if (receipt->GetPosElemRef(i)->enable_flag) {	
	    receipt_table += "<TR >\n"; 
	    //-----(+)-----Zuskin-----08/11/2011-----
	    curpos++;
	    //---------------------------------------
	    for ( vector<TableColumn>::iterator it = columns.begin(); it != columns.end(); it++ ) {
		receipt_table += "<TD WIDTH = \"";
		receipt_table += it->GetWidth();		
		receipt_table += "\" > <P ALIGN = \"";
		receipt_table += it->GetAlign();
		receipt_table += "\" >";
		//-----(*)-----Zuskin-----08/11/2011-----
		//receipt_table += GetReceiptInfo(receipt, it->GetValue(), i);
		if (it->GetValue() == QString("number")) {
		    receipt_table += QString::number(curpos);
		}
		else {
		    receipt_table += GetReceiptInfo(receipt, it->GetValue(), i);
		}
		//---------------------------------------
		
		receipt_table += "</P> </TD> \n";		
	    }
	    
	    receipt_table += "</TR>\n";
	}    
    }
    
    receipt_table += "</TABLE>\n";
    
    return receipt_table;
}

/**********************************************************************/
QString GetOne(int value)
{
    switch (value) {
    case 1: return QString("один ");
    case 2: return QString("два ");
    case 3: return QString("три ");
    case 4: return QString("четыре ");
    case 5: return QString("пять ");
    case 6: return QString("шесть ");
    case 7: return QString("семь ");
    case 8: return QString("восемь ");
    case 9: return QString("девять ");
    case 10: return QString("десять ");
    case 11: return QString("одиннадцать ");
    case 12: return QString("двенадцать ");
    case 13: return QString("тринадцать ");
    case 14: return QString("четырнадцать ");
    case 15: return QString("пятнадцать ");
    case 16: return QString("шестнадцать ");
    case 17: return QString("семнадцать ");
    case 18: return QString("восемнадцать ");
    case 19: return QString("девятнадцать ");
    }	
    return QString("");
}
/**********************************************************************/
QString GetTen(int value)
{
    switch (value) {
    case 1: return QString("");
    case 2: return QString("двадцать ");
    case 3: return QString("тридцать ");
    case 4: return QString("сорок ");
    case 5: return QString("пятьдесят ");
    case 6: return QString("шестьдесят ");
    case 7: return QString("семьдесят ");
    case 8: return QString("восемьдесят ");
    case 9: return QString("девяносто ");
    }	
    return QString("");    
}
/**********************************************************************/
QString GetHundred(int value)
{    
    switch (value) {
    case 1: return QString("сто ");
    case 2: return QString("двести ");
    case 3: return QString("триста ");
    case 4: return QString("четыреста ");
    case 5: return QString("пятьсот ");
    case 6: return QString("шестьсот ");
    case 7: return QString("семьсот ");
    case 8: return QString("восемьсот ");
    case 9: return QString("девятьсот ");
    }	
    
    return QString("");
}
/**********************************************************************/
//  position (int) -2 < position < 12
// value (int)  0 < value < 19
QString GetWord(int position, int value)
{       
    if ( position == -2 ) {
	if ( value == 1 ) return "одна копейка "; 
	if ( value == 2 ) return "две копейки ";  
	if ( value == 3 ) return "три копейки "; 
	if ( value == 4 ) return "четыре копейки "; 
	return GetOne(value) + "копеек ";
    }
    
    if ( position == 3 ) {
	if ( value == 1 ) return "одна тысяча ";
	if ( value == 2 ) return "две тысячи "; 
	if ( value == 3 ) return "три тысячи "; 
	if ( value == 4 ) return "четыре тысячи "; 
	return GetOne(value) + "тысяч "; 
    }
    
    if ( position % 3 == 1 || position % 3 == -1 ) return GetTen(value);
    
    if ( position % 3 == 2 ) return GetHundred(value);
    
    QString result;
    if ( position == 0 ) {
	result += GetOne(value);
	if ( value == 1 ) { result += "рубль "; return result;  }
	if ( (value == 2) || (value == 3) || (value == 4) ) { result += "рубля ";  return result;  }
	result += "рублей "; return result; 
    }
    
    if ( position == 6 ) { result +=  GetOne(value) + "миллион"; }
    if ( position == 9 ) { result +=  GetOne(value) + "миллиард"; }
    /*тут можно добавлять бо'льшие числа */
    
    if ( position % 3 == 0 ) {
	if ( value == 1 ) result += " " ;
	if ( (value == 2)  || (value == 3) || (value == 4) ) result += "а ";
	if ( value > 4 ) result += "ов ";
	return result;
    }    
    
    return "";
}

/**********************************************************************/	
QString ConvertToWords(double sum)
{    
    sum = sum  + 0.0001;    // + 0,0001 делается для того чтобы сумма не получалась на единицу меньше
    
    QString result = "";        
    int i = 0;    
    int ones_sum = 0;
    double divisor = 1;    
    double sum0 = sum;
    while ( sum0 > 10 ) {
	divisor = divisor*10;
	sum0 = sum0 / 10;
    }
    
    if ( sum < 1 ) result += "ноль ";
    
    int position = QString::number(divisor, 'f', 0).length() - 1;
    
    
    while ( divisor > 0.009 ) {	
	
	if ( position == -1 ) {
	    if ( sum < 0.01 ) {	 result += "ноль копеек";  break;  }
	}
	
	i = (int)(sum/divisor) ;
	
	ones_sum = ones_sum + i;
	
	sum = sum - i*divisor;
	divisor = divisor/10;
	
	//printf("abs(position % 3) = %d i = %d ones_sum = %d sum = %f divisor = %f \n", abs(position % 3), i, ones_sum, sum, divisor);
	
	if ( (abs(position % 3) == 1) && (i == 1) ) ones_sum = 10;
	else {	    
	    result += GetWord(position, ones_sum);
	    ones_sum = 0;
	}
	
	position--;
    }
    
    //cout  << result << endl;
    
    result = DefCodec->toUnicode(result);
    
    result = result.mid(0,1).upper() + result.mid(1, result.length()-1);
    
    return result;

}
/**********************************************************************/	
QString GetSum(double sum)
{
    QString s = QString::number(sum, 'f', 2) ;
    return s;
}
/**********************************************************************/	
QString GetDate(QDate d)
{    
    //QDate d = QDate::currentDate();
    
    QString result = "";
    result += d.toString("dd ");
    switch ( d.month() ) {
    case 1 : result += DefCodec->toUnicode("Января "); break;
    case 2 : result += DefCodec->toUnicode("Февраля "); 	break;
    case 3 : result += DefCodec->toUnicode("Марта "); break;
    case 4 : result += DefCodec->toUnicode("Апреля ");	break;
    case 5 : result += DefCodec->toUnicode("Мая "); break;
    case 6 : result += DefCodec->toUnicode("Июня ");	break;
    case 7 : result += DefCodec->toUnicode("Июля ");  break;
    case 8 : result += DefCodec->toUnicode("Августа ");  break;
    case 9 : result += DefCodec->toUnicode("Сентября ");  break;
    case 10 : result += DefCodec->toUnicode("октября ");  break;
    case 11 : result += DefCodec->toUnicode("Ноября ");	break;
    case 12 : result += DefCodec->toUnicode("Декабря ");	break;
    }
    
    result += d.toString("yyyy") + DefCodec->toUnicode(" г.");

    return result;
}
/**********************************************************************/	
QString GetCheckType(int rt)
{
    switch ( rt ) {
    case CHECK_SELL : return DefCodec->toUnicode("Продажа "); 
    case CHECK_RETURN : return DefCodec->toUnicode("Возврат "); 
    case CHECK_COPY : return DefCodec->toUnicode("Копия чека "); 	
    default : return "";	
    }
}

/**********************************************************************/
QString GetReceiptDate(Receipt * r)
{
    QDate d;
    if (r->GetCheckType() == CHECK_COPY) d = QDate::fromString(QString(r->getDate()), Qt::ISODate);
    else d = QDate::currentDate();
    
    return GetDate(d);
}
/**********************************************************************/
QString GetCashier(Receipt * r) 
{
    return DefCodec->toUnicode(r->getCashier());
}

/*=========================TableColumn class=============================*/
TableColumn::TableColumn()
{
    caption = QString("");
    value = QString("");
    width = QString("");
}
/**********************************************************************/
void TableColumn::FromTag(QString tag)
{
    //cout << "tag = " << tag << endl;
    tag.remove(0 , CHECK_COLUMN_TAG.length());
    tag.remove(tag.length() - 1, 1);
    
    caption = FindProperty(tag, CAPTION_COLUMN);
    value = FindProperty(tag, VALUE_COLUMN);
    width = FindProperty(tag, WIDTH_COLUMN);
    align = FindProperty(tag, ALIGN_COLUMN);
    
    //cout << "caption = " << caption << endl;
    //cout << "value = " << value << endl;
    //cout << "width = " << width << endl;
}
/**********************************************************************/
QString TableColumn::FindProperty(QString str, QString property_name)
{
    int position = str.find(property_name, 0, false);
    if ( position == -1 ) return "";
    
    position += property_name.length() + 1;
    while ( position < str.length() && str.ref(position) == ' ' ) position++;
    
    if ( position > str.length() - 4 ) return "";
    
    if ( str.ref(position) != '=' ) return "";
    position++;
    
    while ( position < str.length() && str.ref(position) == ' ' ) position++;
    
    if ( position > str.length() - 3 ) return "";
    
    if ( str.ref(position) = '"' ) {
	
	int position_end = str.find('"', position + 1, false);
	if ( position_end != -1 ) return str.mid(position + 1, position_end - position - 1);
    } 
    
    return "";
    
}
/**********************************************************************/
void TableColumn::SetCaption(QString cap)
{    
    caption = cap;
}
/**********************************************************************/
void TableColumn::SetValue(QString val)
{
    value = val;
}
/**********************************************************************/
void TableColumn::SetWidth(QString wid)
{
    width = wid;
}
/**********************************************************************/       
void TableColumn::SetAlign(QString al)
{
    align = al;
}
/**********************************************************************/       
QString  TableColumn::GetCaption()  {  return caption;  }
QString TableColumn::GetValue() { return value; }
QString TableColumn::GetWidth() { return width;  }
QString TableColumn::GetAlign() { return align;  }
/**********************************************************************/     

/*=========================End TableColumn class=============================*/
