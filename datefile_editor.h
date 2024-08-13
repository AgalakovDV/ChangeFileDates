
#ifndef DATEFILE_EDITOR_H
#define DATEFILE_EDITOR_H

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>



class DateFile_Editor
{
private:
    QString sPathDir;       // путь до папки с файлами, в которой у файлов будем менять время создания
    QDateTime seed;         // главное значение дата и время
    int radius_minute;      // разброс в минутах
//    int delta_min;      // наименьшая разница времен между файлами в минутах
    uint countFiles;        // количество файлов в папке
    int divider = 6;

public:
    DateFile_Editor();

    void printDirInfo();
    int ChangeDate();

    void SetPathDir(QString _sPathDir){
        this->sPathDir = _sPathDir;
    }

    void SetSeed(QDateTime _seed){
        this->seed = _seed;
    }

    void Set_radius_minute(int _radius_minute){
        this->radius_minute = _radius_minute;
    }

/*    void Set_delta_min(int _delta_min){
        this->delta_min = _delta_min;
    }*/
};

#endif // DATEFILE_EDITOR_H
