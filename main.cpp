#include "mainwindow.h"

#include <QApplication>
#include <QStandardPaths>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if (!QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).exists()) {
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }
    if (!QFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config.ini").exists()) {
        QFile create(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config.ini"); create.open(QIODevice::ReadWrite); create.close();
    }

    return a.exec();
}
