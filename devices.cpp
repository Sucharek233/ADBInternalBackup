#include "devices.h"
#include "ui_devices.h"

QStringList connected;

void devices::init()
{
    on_pushButton_Scan_clicked();
}

QStringList Devices()
{
    QProcess adb;
    adb.start("adb", QStringList() << "devices");
    adb.waitForFinished();
    QString devicesRaw = adb.readAll();
    devicesRaw.replace("List of devices attached", "");
    devicesRaw.replace("\r", "\n");
    devicesRaw.replace("\t", "\t\t");
    QStringList devicesL = devicesRaw.split("\n");
    QStringList devices;
    foreach(QString device, devicesL) {
        if (device.simplified() != "") {
            device.replace("\n", "");
            devices << device;
        }
    }
    return devices;
}

devices::devices(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::devices)
{
    ui->setupUi(this);
}

devices::~devices()
{
    delete ui;
}

void devices::on_pushButton_Scan_clicked()
{
    int saveSelection = ui->listWidget_Devices->currentRow();
    connected.clear();
    ui->listWidget_Devices->clear();
    QStringList devices = Devices();
    foreach(QString device, devices) {
        ui->listWidget_Devices->addItem(device);
        connected << device;
    }
    if (saveSelection != -1) {
        if (ui->listWidget_Devices->count() > saveSelection) {
            ui->listWidget_Devices->setCurrentRow(saveSelection);
        }
    }
    if (connected.isEmpty()) {sel = "";}
}

void devices::on_pushButton_Select_clicked()
{
    int selected = ui->listWidget_Devices->currentRow(); ui->listWidget_Devices->clear();
    foreach(QString device, connected) {ui->listWidget_Devices->addItem(device);}

    int curr = -1; QString currDevice;
    foreach(QString device, connected) {
        curr += 1;
        if (curr == selected) {currDevice = device; break;}
    }
    qDebug() << currDevice;

    QString stylesheet = "QMessageBox QLabel {font-size: 18px;} QMessageBox QPushButton {font-size: 14px;}";
    if (selected == -1) {
        QMessageBox selection;
        selection.setStyleSheet(stylesheet);
        selection.setWindowTitle("Not selected");
        selection.setText("You have not selected a device.\n"
                          "Please select one and click Select.");
        selection.exec();
    } else {
        ui->listWidget_Devices->setCurrentRow(selected);

        sel = currDevice.left(currDevice.lastIndexOf('\t') + 1);
        sel = sel.replace("\t\t", "");
        qDebug() << sel;

        QMessageBox selected;
        selected.setStyleSheet(stylesheet);
        selected.setWindowTitle("Device selected");
        selected.setText(sel + " has been selected.\n");
        selected.exec();
        close();
    }
}

void devices::on_pushButton_Wireless_clicked()
{
    QString ip = ui->lineEdit_IP->text();
    QString port = ui->lineEdit_Port->text();

    QProcess connect;
    connect.start("adb", QStringList() << "connect" << ip + ":" + port);
    connect.waitForFinished();

    on_pushButton_Scan_clicked();
}

void devices::on_pushButton_Disconnect_clicked()
{
    QProcess connect;
    connect.start("adb", QStringList() << "disconnect");
    connect.waitForFinished();

    on_pushButton_Scan_clicked();
}

void devices::on_pushButton_Close_clicked()
{
    close();
}
