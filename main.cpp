#include "widget.h"

#include <QApplication>
#include <QSystemTrayIcon>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QSystemTrayIcon sysTray(QIcon(":/title.ico"),&w);

    QMenu menu;
    auto exitAct = new QAction("exit",&sysTray);

    QObject::connect(exitAct,&QAction::triggered,[&](){
        QApplication::quit();
    });


    menu.addAction(exitAct);

    sysTray.setContextMenu(&menu);

    sysTray.show();

    w.show();
    return a.exec();
}
