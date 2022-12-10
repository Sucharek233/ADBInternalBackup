#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

class searchThread : public QThread
{
   Q_OBJECT

public:
    QStringList process(QString search, int folders);

    QString getText() {return TE;}
    QString getFullText() {return stuff;}
    QString getMC() {return missingContents.replace("/sdcard/", "");}

    void setMissing(QStringList miss) {missing = miss;}
    void turbo_andother(int Switch) {turboSwitch_andother = Switch;}
    void setDevice(QString device) {dev = device;}
    void setLocal(QString loc) {local = loc;}

    void doTheMagic(QString phone);

    void turboScan(QString phone);

    void faster(int yeornah) {gofast = yeornah;}

    void retrive(QStringList missing);

public slots:
    void stopRunning();

protected:
   virtual void run();

signals:
   void update(QString);

private:
    bool isRunning;

    QString stuff;
    QString TE;
    QString missingContents;
    QStringList missing;
    QString dev;
    QString local;

    int gofast = 0;
    int turboSwitch_andother = 0;
};

#endif // THREAD_H
