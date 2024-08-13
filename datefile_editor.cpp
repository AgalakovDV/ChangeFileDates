
#include "datefile_editor.h"
#include <QDebug>

// создание случайных отклонений в секундах
QList<int> getRandDeviationSec(int count, int seed, int sec_min_radius){
    if (count <= 0)
        count = - count + 1;
    if (sec_min_radius <= 0)
        sec_min_radius = 1 - sec_min_radius;
    int div = sec_min_radius * 2;
    QList<int> devs;
    srand(seed);
    for (int i = 0; i < count; i++){
        devs.push_back(rand()%div - sec_min_radius);
    }
    return devs;
}


DateFile_Editor::DateFile_Editor()
{
    this->sPathDir = "C:/Users/agalakov.d.v/Documents/codes/qt_prj/qt_prj/datetime_changes_12_08_2024/test_4";
    this->seed = QDateTime(QDate(2020,01,13), QTime(1,13,1));
    this->radius_minute = 3;
//    this->delta_min = 60 * 5;
}


void DateFile_Editor::printDirInfo() {
    qDebug() << "\n дата и время: " << this->seed.date() << " " << this->seed.time();
    qDebug() << " разброс часов: " << this->radius_minute;
//    qDebug() << " дельта: " << this->delta_min;

    qDebug() << " текущая папка: " << this->sPathDir;
    qDebug() << " число файлов в папке: " << this->countFiles;
    qDebug() << "информация про записи";

}


int DateFile_Editor::ChangeDate(){
    this->countFiles = QDir(this->sPathDir).entryList(QDir::Files).count();
//    this->printDirInfo();

    QStringList fileNames = QDir(this->sPathDir).entryList(QDir::Files);
    int delta = this->radius_minute / fileNames.size(); // среднее время выполнение одного файла в минутах
    int sec_min_radius = delta * 60 / this->divider;            // уклонение от среднего в секундах
    qint64 msec = QDateTime::currentMSecsSinceEpoch();
    uint uSeed = 0;
    if (msec < 0)
        uSeed = 13;
    else
        uSeed = static_cast<uint>(msec);
    QList<int> deviations = getRandDeviationSec(fileNames.size(), uSeed, sec_min_radius);

    QDateTime curDateTime = QDateTime(seed);

    qDebug() << fileNames;
    for (int i = 0; i < fileNames.size(); ++i) {
        QString mName = fileNames.at(i);
        QFile curFile = QFile(this->sPathDir + "/" + mName);
        if (curFile.open(QIODevice::ReadWrite))
        {
            bool check_1 = false;
            bool check_2 = false;
            QDateTime resDateTime = curDateTime.addSecs(delta * 60 + deviations.at(i));
            check_1 = curFile.setFileTime(resDateTime, QFileDevice::FileBirthTime);
            check_2 = curFile.setFileTime(resDateTime, QFileDevice::FileModificationTime);
            qDebug() << " имя файла: " << curFile.fileName()
                     << "\n\t новая дата создания: " << curFile.fileTime(QFileDevice::FileBirthTime)
                     << "\n\t новая дата изменения: " << curFile.fileTime(QFileDevice::FileModificationTime);
            curFile.close();
            if (!check_1 || !check_2){
                qDebug() << "вот и всё никто не ждет";
            }
        }
        else{
            qDebug() << " Невозможно открыть файл " << curFile.fileName();
        }
        curDateTime = curDateTime.addSecs(delta * 60);
    }

    return 0;
}


