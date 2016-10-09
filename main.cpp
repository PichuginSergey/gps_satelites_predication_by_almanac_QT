
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    QObject::connect(w.exit, SIGNAL(clicked()),
                     &app, SLOT(quit()));
    w.show();

    return app.exec();
}
