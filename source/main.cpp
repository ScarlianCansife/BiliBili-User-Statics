#include "mainwidget.h"
#include "biliuser.h"
#include "waitedialog.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget mainWidget;
    mainWidget.show();
    return a.exec();
}
