
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "Enums.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->change_flag = false;
    ui->dateEdit_newDateCreate->setDate(QDate::currentDate());

    this->pDateEditor = new DateFile_Editor();
}

MainWindow::~MainWindow()
{
    delete pDateEditor;
    delete ui;
}


int MainWindow::GetInfo(){
    QString _sPathDir = ui->label_curPath->text();       // путь до папки с файлами, в которой у файлов будем менять время создания
    QDate _date = QDate(ui->dateEdit_newDateCreate->dateTime().date());       // дата пользователя
    QString timeStr = ui->textEdit_newTimeCreate->toPlainText();
    QStringList qsl = timeStr.split(":");
    if (qsl.size() != 3){ // ошибка во времени
        return WRONG_FORMAT;
    }

    bool checkTime[3] = {false, false, false};
    int h = qsl.first().toInt(checkTime);
    int m = qsl.at(1).toInt(checkTime + 1);
    int s = qsl.at(2).toInt(checkTime + 2);
    if (!(checkTime[0] && checkTime[1] && checkTime[2]) || (h < 0 || h > 23) || (m < 0 || m > 59) || (s < 0 || s > 59)){ // неправильно ввели время
        return WRONG_TIME;
    }

    QTime _time = QTime(h,m,s);                 // время пользователя
    QDateTime _seed = QDateTime(_date, _time);      // главное значение дата и время

    //int _radius_hours = ui->spinBox_radius->value();   // разброс в часах
    QDateTime razbros = ui->timeEdit_radius->dateTime();
    int _radius = razbros.time().hour() * 60 + razbros.time().minute();   // разброс в минутах

    if (_radius < 0){ // какая-то ошибка с разбросом
        return UNEXPECTED_ERROR;
    }

//    qDebug() << "_sPathDir" << _sPathDir;
//    qDebug() << "_seed" << _seed.date() << " " << _seed.time();
//    qDebug() << "_radius_hours" << _radius;

    //изменяем значения полей
    this->pDateEditor->SetPathDir(_sPathDir);
    this->pDateEditor->SetSeed(_seed);
    this->pDateEditor->Set_radius_minute(_radius);

    // запускаем алгоритм изменения даты
    this->pDateEditor->ChangeDate();

    return OK;
}


void MainWindow::on_pushButton_ok_clicked()
{
    if (!this->change_flag){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Папка не изменена.");
        msgBox.setText("Вы не изменили папку.\nВсё равно изменить дату файлов в этой папке?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();

        switch (ret){
        case QMessageBox::No:
            return;
        case QMessageBox::Yes:
            break;
        default:
            break;
        }

    }
    int er = this->GetInfo();
    switch (er){
    case OK:
        QMessageBox::information(0, "Congratulation", "Поздравляем! Даты файлов были успешно изменены!");
        this->change_flag = false;
        break;
    case WRONG_FORMAT:
        QMessageBox::critical(0, "Error", "Неверный формат времени. Укажите время через двоеточие в формате 'часы:минуты:секунды'.");
        break;
    case WRONG_TIME:
        QMessageBox::critical(0, "Error", "Ошибка во времени. Укажите время через двоеточие в формате 'чч:мм:сс'.");
        break;
    case UNEXPECTED_ERROR:
        QMessageBox::critical(0, "Error", "Непредвиденная ошибка. Попробуйте изменить разброс.");
        break;
    default:
        break;
    }

}


void MainWindow::on_pushButton_searchDir_clicked()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString fileNameStr = dialog.selectedFiles().at(0);
    int pos_dir = fileNameStr.lastIndexOf("/");
    QString dirStr = fileNameStr.mid(0, pos_dir);
    ui->label_curPath->setText(dirStr);

    this->change_flag = true;

}



void MainWindow::on_pushButton_help_clicked()
{
    QString helpStr = " 1) выбрать нужную папку с помощью кнопки 'Найти папку'\n\t(для выбора папки откройте любой файл в ней)\n 2) ввести дату\n 3) ввести время через двоеточие 'часы:минуты:секунды'\n 4) указать временной разброс\n 5) нажать 'Ok' ";
    QMessageBox::information(0, "Помощь", helpStr);
}

