#ifndef DISCSCHEMEMAPPER_H
#define	DISCSCHEMEMAPPER_H

#include "RDBMapper.h"
#include "DiscScheme.h"

using namespace std;

const string DISCSCHEME_ID_FIELD = "auto_scheme_id";
const string DISCSCHEME_CODE = "auto_scheme_id";
const string DISCSCHEME_LONGTITLE = "longtitle";
const string DISCSCHEME_SHORTCUT = "shortcut";
//-----(+)-----Zuskin-----14/12/2011-----
const string DISCSCHEME_IS_DEFAULT = "is_default";
//---------------------------------------

class DiscSchemeMapper : public RDBMapper {
public:
    DiscSchemeMapper();
    string getIdFieldName();
    Object * createObjectByDefault();

private:
    virtual Object* createObjectFromResultRecord(const ResultRecord & record);
    virtual void fromPersistent(Object* object, const ResultRecord & record);
    virtual void toPersistent(Object * object, RDBSetExpression& rdb_expression);
};

#endif	/* DISCSCHEMEMAPPER_H */

