#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
#include <QDateEdit>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRecord>
#include "logger.h"
#include "converter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //занесение в лог действия
    void makeLogNote(QString);
    //чтение датаграммы
    void readDatagram();
    //установка Ip адреса
    void setIp();
    void on_exitButton_clicked();
    void on_updBut_clicked();
    void on_clearBut_clicked();
    void on_combObjTableBut_clicked();
    void on_logTableBut_3_clicked();
    //обработка принятого сообщения
    void parsingMessage(QString);

private:
    Ui::MainWindow *ui;
    QUdpSocket udpSocket;
    QTimer timer;
    QSqlDatabase db;
    QSqlTableModel *model;
    QHostAddress myIp;
    QHostAddress targetIp;
    QString targetPort;
    QString myPort;
    Logger *logger;
    Converter *converter;
};

#endif // MAINWINDOW_H
