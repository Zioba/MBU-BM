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

    QByteArray getInformationAboutObj( QByteArray );

    void makeLogNote( QString );

     ~MainWindow();

private slots:
    QString makeDatagram( QString, int * );

    void readDatagram();

    void on_exitButton_clicked();

    void on_updBut_clicked();

    void on_clearBut_clicked();

    void on_itemSendCoord_triggered();

    void setIp();

    void on_combObjTableBut_clicked();

    void on_logTableBut_3_clicked();

    QString getCurrentDateAndTime();

private:
    Ui::MainWindow *ui;

    QSqlDatabase   db;
    QUdpSocket     udpSocket;
    QSqlTableModel *model;

    QHostAddress myIp;
    QHostAddress targetIp;
    QString      myPort;
    QString      targetPort;

    Logger    *logger;
    Converter *converter;
};

#endif // MAINWINDOW_H
