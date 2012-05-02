//


#ifndef STRIKEOUTTABITM_H
#define STRIKEOUTTABITM_H

#include <qtable.h>

class StrikeOutTableItem : public QTableItem
{
public :
    StrikeOutTableItem( QTable *t, EditType et, const QString &txt ) : QTableItem( t, et, txt ) {}
    void paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected );
};
#endif
