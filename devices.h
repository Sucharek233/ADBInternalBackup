#ifndef DEVICES_H
#define DEVICES_H

#include <QDialog>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class devices;
}

class devices : public QDialog
{
    Q_OBJECT

public:
    explicit devices(QWidget *parent = nullptr);
    ~devices();

    QString getSelected() {return sel;}

    void init();

private slots:
    void on_pushButton_Scan_clicked();

    void on_pushButton_Wireless_clicked();

    void on_pushButton_Close_clicked();

    void on_pushButton_Select_clicked();

    void on_pushButton_Disconnect_clicked();

private:
    Ui::devices *ui;

    QString sel;
};

#endif // DEVICES_H
