
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "datefile_editor.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_searchDir_clicked();

    void on_pushButton_help_clicked();

private:
    int GetInfo();                // получить информацию от пользователя

private:
    Ui::MainWindow *ui;

    bool change_flag;             // флаг изменения состояния (нужен против человеческих ошибок)
    DateFile_Editor *pDateEditor; // экземпляр изменятеля даты
};

#endif // MAINWINDOW_H
