#ifndef PRINT_RECEIPT
#define PRINT_RECEIPT

#include "receipt.h"
#include <qstring.h>
#include <qtextstream.h> 


const QString CAPTION_COLUMN = QString("caption");
const QString VALUE_COLUMN = QString("value");
const QString WIDTH_COLUMN = QString("width");
const QString ALIGN_COLUMN = QString("align");

class TableColumn {
    private :
       QString caption;
       QString value;
       QString width;
       QString align;
    public :
       TableColumn();
       
       void FromTag(QString tag);
       QString FindProperty(QString str, QString property_name);       
       
       void SetCaption(QString cap);
       void SetValue(QString val);
       void SetWidth(QString wid);
       void SetAlign(QString al);
       
       QString GetCaption();
       QString GetValue();
       QString GetWidth();
       QString GetAlign();
       
};


// текущая дата (в компьютере)
const QString DATE_TAG = QString("<DENCI_DATE>");
// итоговая сумма ( промежуточный итог )
const QString TOTALSUM_TAG = QString("<DENCI_TOTALSUM>");
// Итоговая сумма скидок
const QString DISCOUNTSUM_TAG = QString("<DENCI_DISCOUNTSUM>");
// итоговая сумма (с учетом скидок) числом
const QString CHECKSUM_TAG = QString("<DENCI_CHECKSUM>");
// итоговая сумма (с учетом скидки) прописью
const QString CHECKSUM_INWORDS_TAG = QString("<DENCI_CHECKSUM_INWORDS>");
// тип чека 
const QString CHECKTYPE_TAG = QString("<DENCI_CHECKTYPE>");
// номер чека 
const QString CHECKNUMBER_TAG = QString("<DENCI_CHECKNUMBER>");
//дата пробития чека 
const QString CHECKDATE_TAG = QString("<DENCI_CHECK_DATE>");
//фамилия кассира
const QString CASHIER_TAG = QString("<DENCI_CASHIER>");


const QString HTML_COMMENT_START = QString("<!--");
const QString HTML_COMMENT_END = QString("-->");

// открыть таблицу чека 
const QString CHECKTABLE_TAG = QString("<DENCI_CHECKTABLE>");	
// закрыть таблицу чека 
const QString END_CHECKTABLE_TAG = QString("</DENCI_CHECKTABLE>");
// столбец таблицы чека
const QString CHECK_COLUMN_TAG = QString("<DENCI_CHECKCOLUMN");


void PrintReceiptDocument(Receipt * receipt, const QString & document_path);


QString GetReceiptDate(Receipt * r);
QString GetReceiptTable(Receipt * receipt, vector <TableColumn> columns);
QString GetDate(QDate); 
QString GetCashier(Receipt * r);
QString ConvertToWords(double sum);

QString GetSum(double sum);
QString GetCheckType(int rt);

void InsertCheckTable(QString & html_str, Receipt * receipt); 
QString CreateCheckTable(QString tags, Receipt * receipt);
void RemoveComment(QString& html_str);

QString GetReceiptInfo(Receipt* receipt, QString value, int i);

       
       


#endif
