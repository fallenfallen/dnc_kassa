
#include <qstring.h>
#include <qtable.h>

#include <vector>

using namespace std;

class PrintDocument {
    private :
	QString name;
	QString path;
    public :
	PrintDocument();   
        PrintDocument(QString doc_name, QString doc_path);
        ~PrintDocument();
	void SetName(QString doc_name);
	QString GetName();
	void SetPath(QString doc_path);
	QString GetPath();
	
	void Edit(QString doc_name, QString doc_path);
};


class DocumentList {
    private :
        vector <PrintDocument> documents;
        QTable * table;
    public :
	DocumentList();
        DocumentList(QTable * t);
        ~DocumentList();
	
	void SetTable(QTable * t);
	
	PrintDocument* GetCurrentDocument();
	PrintDocument GetDocumentAt(int index);
	
	void AddDocument(QString name, QString path);
	void AddDocument(PrintDocument pd);
	
	void EditDocument(QString name, QString path);
	void EditDocument(PrintDocument pd);
	
	void RemoveDocument();
	
	int Save(QString scheme_name);
	int Load(QString scheme_name);
	
};



