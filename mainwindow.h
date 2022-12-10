#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "thread.h"
#include "devices.h"
#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QScrollBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QStringList process(QString search, int folders);

    void pathInit();
    void writePath(QString path);

private slots:
    void statbarMsg(QString msg);

    void on_pushButton_Scan_clicked();

    void on_pushButton_Copy_clicked();

    void on_checkBox_Fast_toggled(bool checked);

    void on_pushButton_Turbo_clicked();

    void on_pushButton_Devices_clicked();

    void on_pushButton_BackPath_clicked();

private:
    Ui::MainWindow *ui;

    searchThread scan;
    devices device;
};
#endif // MAINWINDOW_H
