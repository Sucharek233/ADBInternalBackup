#include "thread.h"

int crash = 0;

QStringList searchThread::process(QString search, int folders)
{
    QString modifier;
    if (folders == 1) {modifier = "-R";} else {modifier = "";}

    QProcess retrive;
    retrive.start("adb", QStringList() << "-s" << dev << "shell" << "ls " + modifier + " " + search);
    retrive.waitForFinished(INT_MAX);

    QString modify = retrive.readAll();
    modify.replace("\r", "\n"); modify.replace("\n\n", "\n");
    QStringList final = modify.split("\n");
    final.removeAll(QString(""));

    return final;
}

void searchThread::doTheMagic(QString phone)
{
    stuff = "Folders found:\n"; missingContents = "";
    emit update("disable");
    emit update("Searching phone's folders (might take a while)...");
    QStringList folder = process(phone, 1);
    QStringList foldersFound;
    foreach(QString entry, folder) {
        if (entry.contains("/")) {
            QString result = entry.replace(":", "\n");
            stuff += result; TE = result;
            if (gofast == 0) {msleep(1); emit update("updateTE");}
            QString folder = entry.replace("\n", "");
            foldersFound << folder;
        }
    }
    TE = "Folder scan completed.\n\n\n\nStarting file scan...\n"; stuff += TE;
    emit update("updateTE");

    int mCFolders = 0; int mCFiles = 0;
    int totalFoldersScanned = 0; int totalFilesScanned = 0;
    int crashStop = 0;
    int  msgBoxWarn = 0;
    emit update("Scanning files...");
    foreach(QString entry, foldersFound) {
        crashStop = 0;
        totalFoldersScanned += 1;
        QString entryCopy = entry; entryCopy.remove(0, 8);
        QString currFolder = local + "/" + entryCopy + "/";
        qDebug() << entry;
        QString phoneFolder = entry + "/";
        QDir local(currFolder);
        QString status; if (local.exists()) {status = "exists";} else {
            status = "doesn't exist";
            missingContents += entry + "\n";
            mCFolders += 1;
        }
        QString result = "\n" + entry + " - " + status + " locally\n"; stuff += result; TE = result; emit update("updateTE");
        QStringList files = process("\"" + entry + "\"", 0);
        int oneFileCounter = 0;
        foreach(QString entry, files) {
            totalFilesScanned += 1; oneFileCounter += 1;
            QFile local(currFolder + entry);
            QString status; if (local.exists()) {status = "exists";} else {
                status = "doesn't exist";
                missingContents += phoneFolder + entry + "\n";
                mCFiles += 1;
            }
            QString result = "└───" + entry + " - " + status + " locally\n"; stuff += result; TE = result;
            if (gofast == 0) {
                if (crashStop == 0) {
                    if (oneFileCounter > 50) {
                        if (msgBoxWarn == 0) {
                            emit update("msgBoxWarn");
                        }
                        TE = "\nhalted"; emit update("updateTE");
                        crashStop = 1; msgBoxWarn = 1;
                        msleep(500);
                    }
                    msleep(1); emit update("updateTE");
                }
            }
            if (stuff.count() - crash > 15000) {emit update("preventCrash"); qDebug() << "thread trigger"; crash += 10000; msleep(400);}
        }
    }
    stuff += "File scan finished.\n\n\nLocal missing files:\n";
    stuff += missingContents;
    QString results = "\n\n\nResults:\n"
                      "Total folders scanned: " + QString::number(totalFoldersScanned) + "\n"
                      "Total files scanned: " + QString::number(totalFilesScanned) + "\n\n"
                      "Total local missing folders found: " + QString::number(mCFolders) + "\n"
                      "Total local missing files found: " + QString::number(mCFiles) + "\n\n"
                      "Total folders/files scanned: " + QString::number(totalFoldersScanned + totalFilesScanned) + "\n";
    stuff += results;
    stuff += "Total missing folders/files found: " + QString::number(mCFolders + mCFiles) + "\n\n\n\nProcess completed.";
    emit update ("Displaying full output log...");
    emit update ("updateTEFull");
    emit update ("Scan finished");
    emit update ("enableCopy");
    emit update("enable");
}

void searchThread::turboScan(QString phone)
{
    int mCFolders = 0; int mCFiles = 0;
    int totalFoldersScanned = 0; int totalFilesScanned = 0;
    stuff = "Scan:\n";
    emit update("disable");
    emit update("Turbo scanning phone (usually takes about 20 seconds)...");
    QStringList folder = process(phone, 1);
    QStringList foldersFound;
    emit update ("Generating output log and scanning local files...");
    QString currFolder; QString phoneFolder;
    qDebug() << local << "local";
    foreach(QString entry, folder) {
        if (entry.contains("/")) {
            QString entryCopy = entry;
            entry.replace(":", "");
            QString entryCopyCopy = entry; entryCopyCopy.remove(0, 8);
            currFolder = local + "/" + entryCopyCopy + "/"; phoneFolder = entry + "/";
            QDir local(currFolder);
            QString status; if (local.exists()) {status = "exists";} else {status = "doesn't exist"; missingContents += entry + "\n"; mCFolders += 1;}
            QString result = entryCopy.replace(":", "") + " - " + status + " locally\n";
            stuff += "\n" + result;
            QString folder = entry.replace("\n", "");
            foldersFound << folder;
            totalFoldersScanned += 1;
        } else {
            QFile local(currFolder + entry);
            QString status; if (local.exists()) {status = "exists";} else {status = "doesn't exist"; missingContents += phoneFolder + entry + "\n"; mCFiles += 1;}
            QString result = "└───" + entry + " - " + status + " locally\n";
            totalFilesScanned += 1;
            stuff += result;
        }
    }
    emit update("Displaying the output log...");
    msleep(100);
    stuff += "File scan finished.\n\n\nLocal missing files:\n";
    stuff += missingContents;
    QString results = "\n\n\nResults:\n"
                      "Total folders scanned: " + QString::number(totalFoldersScanned) + "\n"
                      "Total files scanned: " + QString::number(totalFilesScanned) + "\n\n"
                      "Total local missing folders found: " + QString::number(mCFolders) + "\n"
                      "Total local missing files found: " + QString::number(mCFiles) + "\n\n"
                      "Total folders/files scanned: " + QString::number(totalFoldersScanned + totalFilesScanned) + "\n";
    stuff += results;
    stuff += "Total missing folders/files found: " + QString::number(mCFolders + mCFiles) + "\n\n\n\nProcess completed.";
    emit update ("updateTEFull");
    emit update ("Scan finished.");
    emit update ("enableCopy");
    emit update("enable");

}

void searchThread::retrive(QStringList missing)
{
    stuff = "Pulling files:\n\n"; emit update("updateTEFull");
    emit update("disable"); emit update ("Copying files from phone...");
    foreach(QString file, missing) { if (file != "/sdcard") {
        QProcess pull;
        QString localPath = local + "/" + file.left(file.lastIndexOf('/') + 1);
        qDebug() << localPath;
        QDir check(localPath); if (!check.exists()) {check.mkpath(localPath);}
        qDebug() << file << "to move";
        qDebug() << "/sdcard/" + file << "phone";
        qDebug() << localPath << "local";

        QFile fileCheck(local + "/" + file);
        qDebug() << local + file << "local";
        qDebug() << fileCheck.exists() << "check";
        if (fileCheck.exists()) {
            stuff += "Pulling: /sdcard/" + file + " - already exists, skipping\n";
            qDebug() << local + "/" + file << "exists";
        } else {
            qDebug() << local + file << "pulling";
            TE = "Pulling: /sdcard/" + file + "\n"; stuff += TE; emit update ("updateTE");
            pull.start("adb", QStringList() << "pull" << "/sdcard/" + file << localPath);
            pull.waitForFinished(INT_MAX);
        }
        emit update("preventCrash");}
    }
    stuff += "\n\n\nPull finished.";
    update("updateTEFull");
    emit update("enable"); emit update ("Copying finished.");
}

void searchThread::run()
{
    isRunning = 1; while(isRunning == 1) {
        TE = ""; missingContents = ""; stuff = "";
        if (turboSwitch_andother == 0) {
            doTheMagic("/sdcard/"/*, "C:/Users/janso/Downloads/Xiaomi/PocoF4CustomROMs/back/internal_storage_for_program/"*/);
        } else if (turboSwitch_andother == 1) {
            turboScan("/sdcard/"/*, "C:/Users/janso/Downloads/Xiaomi/PocoF4CustomROMs/back/internal_storage_for_program/"*/);
        } else {
            retrive(missing/*, "C:/Users/janso/Downloads/Xiaomi/PocoF4CustomROMs/back/internal_storage_for_program/sdcard/"*/);
        }
        stopRunning();
}   }

void searchThread::stopRunning()
{
    isRunning = 0;
}
