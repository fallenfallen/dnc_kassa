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

#include <QApplication>

#include "mainwindow.h"
#include "translator.h"

int main(int argc, char ** argv)
{
    Q_INIT_RESOURCE(resource);

    QApplication app(argc, argv);

    MainWindow window(conv("ДЭНСИ: Налоговый инспектор"));
    window.showWindow();

    return app.exec();
}

