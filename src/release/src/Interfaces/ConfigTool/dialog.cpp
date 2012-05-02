#include "dialog.h"

#include <QApplication>
#include <QDesktopWidget>

int Dialog::show()
{
    setWindowCenter();
    QWidget::show();
}

void Dialog::setWindowCenter()
{
    move(qApp->desktop()->screen()->rect().center() - rect().center());
}
