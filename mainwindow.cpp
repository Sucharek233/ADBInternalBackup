#include "mainwindow.h"
#include "ui_mainwindow.h"

QString stylesheet;
QString localPath;

void MainWindow::statbarMsg(QString msg)
{
    if (msg == "enable") {
        ui->pushButton_Scan->setEnabled(true);
        ui->pushButton_Turbo->setEnabled(true);
        ui->pushButton_Devices->setEnabled(true);
        ui->pushButton_BackPath->setEnabled(true);
    } else if (msg == "disable") {
        ui->pushButton_Copy->setEnabled(false);
        ui->pushButton_Scan->setEnabled(false);
        ui->pushButton_Turbo->setEnabled(false);
        ui->pushButton_Devices->setEnabled(false);
        ui->pushButton_BackPath->setEnabled(false);
    } else if (msg == "updateTE") {
        QString stuff = scan.getText();
        ui->plainTextEdit_Log->insertPlainText(stuff);
        ui->plainTextEdit_Log->verticalScrollBar()->setValue(ui->plainTextEdit_Log->verticalScrollBar()->maximum());
    } else if (msg == "updateTEFull"){
        QString stuff = scan.getFullText();
        ui->plainTextEdit_Log->setPlainText("");
        while(stuff != "") {
            QString TE = stuff.left(150000);
            stuff.remove(0, 150000);
            qDebug() << stuff.count();
            ui->plainTextEdit_Log->insertPlainText(TE);
        }
        ui->plainTextEdit_Log->verticalScrollBar()->setValue(ui->plainTextEdit_Log->verticalScrollBar()->maximum());
    } else if (msg == "TEnd") {
        ui->plainTextEdit_Log->verticalScrollBar()->setValue(ui->plainTextEdit_Log->verticalScrollBar()->maximum());
    } else if (msg == "preventCrash") {
        QString text = ui->plainTextEdit_Log->toPlainText();
        int check = text.count();
        qDebug() << check;
        if (check > 15000) {
            qDebug() << "triggered";
            int remove = check - 10000;
            text.remove(0, remove);
            ui->plainTextEdit_Log->setPlainText("");
            ui->plainTextEdit_Log->appendPlainText(text);
            ui->plainTextEdit_Log->verticalScrollBar()->setValue(ui->plainTextEdit_Log->verticalScrollBar()->maximum());
            qDebug() << (ui->plainTextEdit_Log->toPlainText()).count() << "finished";
        }
    } else if (msg == "enableCopy") {
        ui->pushButton_Copy->setEnabled(true);
    } else if (msg == "msgBoxWarn") {
        QMessageBox warn;
        warn.setStyleSheet(stylesheet);
        warn.setWindowTitle("Output log halted");
        warn.setText("Too many files to display in output log at once.\n"
                     "To prevent crashes, output log for listing some directories/folders will be paused.\n"
                     "Everything will be displayed in the final output log once the scan finishes.\n"
                     "When this gets triggered, you can see a 500 milisecond pause in the output log and there will be written \"halted\" under the current listing.\n\n"
                     "You can turn on Faster scanning to turn off file listing.");
        warn.exec();
    } else {
        ui->statusbar->showMessage(msg);
    }
}

void MainWindow::pathInit()
{
    QFile look(localPath + "/config.ini");
    look.open(QIODevice::ReadOnly);
    QString local = look.readLine();
    look.close();
    scan.setLocal(local);
}

void MainWindow::writePath(QString path)
{
    QFile file(localPath + "/config.ini");
    file.open(QIODevice::ReadWrite);
    file.resize(0);
    file.write(path.toUtf8());
    file.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit_Log->setReadOnly(true);

    QObject::connect(&scan,SIGNAL(update(const QString&)),SLOT(statbarMsg(const QString&)), Qt::AutoConnection);

    stylesheet = "QMessageBox QLabel {font-size: 18px;} QMessageBox QPushButton {font-size: 14px;}";
    ui->statusbar->showMessage("Please select a device to continue.");

    localPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Scan_clicked()
{
    ui->plainTextEdit_Log->setPlainText("");
    pathInit();
    scan.turbo_andother(0);
    scan.start();
}

void MainWindow::on_pushButton_Copy_clicked()
{
    pathInit();
    QFile file(localPath + "/config.ini"); file.open(QIODevice::ReadOnly); QString read = file.readLine(); file.close();
    if (read == "") {read = "nonsensetotriggerqdirbecauseitsaystrueifthedirectoryvariableisemptyforsomereason";} QDir check(read);
    qDebug() << read; qDebug() << check.exists();
    if (check.exists()) {
        QString missing = scan.getMC();
        QStringList miss = missing.split("\n"); miss.removeDuplicates(); miss.removeAll(QString("")); qDebug() << miss;
        if (miss.isEmpty()) {
            QMessageBox empty;
            empty.setStyleSheet(stylesheet);
            empty.setWindowTitle("Nothing to copy");
            empty.setText("Nothing left to copy.\n"
                          "Everything is backed up.");
            empty.exec();
            ui->pushButton_Copy->setEnabled(false);
        } else {
            scan.setMissing(miss);
            scan.turbo_andother(2);
            scan.start();
        }
    } else {
        QMessageBox noexist;
        noexist.setStyleSheet(stylesheet);
        noexist.setWindowTitle("Backup folder doesn't exist");
        noexist.setText("Backup folder doesn't exist/was moved.\n"
                        "Please select a different one.\n"
                        "A window will pop up after you press ok/close this window.\n"
                        "Please select your backup path there.\n"
                        "To change it in the future, click on the Select Path button.");
        noexist.exec();
        on_pushButton_BackPath_clicked();
    }
}

void MainWindow::on_pushButton_BackPath_clicked()
{
    QFileDialog getPath;
    QString dlPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    getPath.setDirectory(dlPath);
    getPath.setWindowTitle("Select backup path");
    getPath.setFileMode(QFileDialog::Directory);
    QString path;
    if (getPath.exec() == QFileDialog::Accepted) {
        path = getPath.selectedFiles().join("");
        scan.setLocal(path);
        writePath(path);
        QMessageBox sel;
        sel.setStyleSheet(stylesheet);
        sel.setWindowTitle("Folder selected");
        sel.setText("\"" + path + "\" has been selected as a folder where this program will put backups.\n"
                    "Your option will be saved.\n"
                    "To change it, click on the button Select Path under Copy missing/Backup");
        sel.exec();
    }
    qDebug() << path;
}


void MainWindow::on_checkBox_Fast_toggled(bool checked)
{
    if (checked == true) {scan.faster(1);} else {scan.faster(0);}
}

void MainWindow::on_pushButton_Turbo_clicked()
{
    pathInit();
    ui->plainTextEdit_Log->setPlainText("");
    scan.turbo_andother(1);
    scan.start();
}


void MainWindow::on_pushButton_Devices_clicked()
{
    device.init();
    device.exec();
    QString selected = device.getSelected();
    if (selected.isEmpty()) {
        QMessageBox empty;
        empty.setStyleSheet(stylesheet);
        empty.setWindowTitle("No device selected");
        empty.setText("No device has been selected\n"
                      "Please return to the devices menu a select a device.\n\n"
                      "Without a device selected, program cannot continue.");
        empty.exec();
        statbarMsg("disable");
        ui->pushButton_Devices->setEnabled(true);
        ui->statusbar->showMessage("Please select a device to continue.");
    } else {
        statbarMsg("enable");
        scan.setDevice(selected);
        ui->statusbar->showMessage("Device selected");
    }
}
