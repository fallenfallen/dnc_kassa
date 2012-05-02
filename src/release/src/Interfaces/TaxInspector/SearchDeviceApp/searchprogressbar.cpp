/*====================================================================================
# «ДЭНСИ:Налоговый инспектор» module of «ДЭНСИ: КАССА» complex is an additional utility, wich allows to use FM/ECRP functions.
# © OOO "DNC Trade" 2011, Authors: Romanow Alexey.
# This is a FREE software, licensed under GNU/GPL terms. See details in LICENSING_ENG.TXT in main directory
#====================================================================================
# Модуль «ДЭНСИ:Налоговый инспектор» программного комплекса «ДЭНСИ: КАССА» - дополнительная утилита, предоставляющая доступ к функциям работы с ФП/ЭКЛЗ. 
# © ООО "ДЭНСИ Трейд" 2011, Авторы: Романов Алексей.
# Данная программа является свободным программным обеспечением, выпускается на условиях Стандартной Общественной Лицензии GNU/GPL
# Подробнее смотрите в файле LICENSING_RUS.TXT в основной директории
#====================================================================================*/

#include "searchprogressbar.h"

SearchProgressBar::SearchProgressBar(QWidget * parent) : QWidget(parent)
{
    setFixedHeight(130);
}

SearchProgressBar::~SearchProgressBar() {}

void SearchProgressBar::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

    painter.setPen(Qt::black);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect borderRect = QRect(10, height() - 24, width() - 20, 22);

    QLinearGradient gradient(borderRect.topLeft(), borderRect.bottomRight());
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, Qt::green);

    if (!description.isEmpty())
        painter.drawText(10, 10, width() - 10, 90, Qt::TextIncludeTrailingSpaces, description);

    painter.drawRect(borderRect);

    QPainterPath path;
    QRect progress(borderRect.left() + 2, borderRect.top() + 2,
                   (borderRect.width() - 4) * percentage, borderRect.height() - 4);

    path.addRect(progress);
    painter.setClipRect(borderRect, Qt::ReplaceClip);
    painter.setBrush(gradient);
    painter.drawPath(path);

    QFontMetricsF metrics(font());
    QString str = QString::number((int)(100 * percentage)) + "%";
    QRectF rect = metrics.boundingRect(str);
    QPointF pos = QPointF((borderRect.width() - rect.width()) / 2.0,
                          borderRect.bottom() - (borderRect.height() - rect.height()) / 2.0 - 2);

    painter.drawText(pos, str);
}

void SearchProgressBar::setOperationDescription(const QString &description)
{
    this->description = description;
    repaint();
}

void SearchProgressBar::setValueRange(double min, double max)
{
    this->min = min;
    this->max = max;
    this->scale = 1.0 / (min + max);
}

void SearchProgressBar::setProgressValue(double value)
{
    this->percentage = scale * (value - min);
    repaint();
}
