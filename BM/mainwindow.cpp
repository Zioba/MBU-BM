#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deldialog.h"
#include "ipdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db=QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setPort(5432);
    db.setDatabaseName("Database_BM");
    db.setUserName("postgres");
    db.setPassword("perpentumzioba");
    if (!db.open()) {
        makeLogNote(db.lastError().text());
    }
    else {
        makeLogNote("Start working");
    }
    model = new QSqlTableModel(this,db);
    model->setTable("own_forces.combatobject_location");
    ui->tableView->setModel(model);
    setWindowTitle("BM");
    QPoint pos(0,1);
    this->move(pos);
    setIp();
    logger = new Logger(db);
    converter = new Converter();
    myIp.setAddress("127.0.0.1");
    targetIp.setAddress("127.0.0.1");
    myPort="5825";
    targetPort="5824";
    udpSocket.bind(5825);
    connect(&udpSocket, SIGNAL(readyRead()),this, SLOT(readDatagram()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete logger;
    delete converter;
}

void MainWindow::makeLogNote(QString s)
{
    ui->logField->append(tr("%1 %2").arg(QTime::currentTime().toString("hh:mm:ss")).arg(s));
}

void MainWindow::on_exitButton_clicked()
{
   this->close();
}

void MainWindow::on_updBut_clicked()
{
    model->select();
    ui->logField->append(tr("%1 table update").arg(QTime::currentTime().toString("hh:mm:ss")));
}

void MainWindow::on_clearBut_clicked()
{
    ui->logField->clear();
}

void MainWindow::on_itemSendCoord_triggered()
{
    QString trash;
    trash="53.10";
    int number=0;
    QString data=makeDatagram(trash,&number);
    qDebug()<<"number="<<number;
    QStringList list;
    list << myIp.toString()
         << targetIp.toString()
         << "17"
         << QString::number(data.length()+224)
         << myPort
         << targetPort
         << QString::number(data.length())
         << ""  //доделать чексумму
         << "0001"
         << QString::number(5)
         << "1"
         << "1"
         << data;
    QByteArray datagram=converter->encode(list);
    qDebug()<<targetPort.toLong(Q_NULLPTR,10);
    udpSocket.writeDatagram(datagram, targetIp, targetPort.toLong(Q_NULLPTR,10));
    makeLogNote("отправлен пакет");
    int x=logger->makeNote(1,QDate::currentDate().toString("dd.MM.yyyy")+QTime::currentTime().toString("hh:mm:ss.zzz"),1,data,2);
    if (x==0) {
        makeLogNote("успех");
    }
        else {
        if (x==2) makeLogNote("FAIL2");
        else makeLogNote("FAIL1");
    }
}

QString MainWindow::makeDatagram(QString q, int *number)
{
    QString answer="";
    answer.append("0");                        //метод сжатия
    answer.append(converter->dobei(q,6));      //отправитель добить до 6
    answer.append(converter->dobei("mbu",6));  //получатель
    answer.append("0");                        //категория данных
    answer.append("C");                        //данные о сообщении
    answer.append("C1");                       //Идентификатор приложения, которое  должно обрабатывать переданные данные.
    answer.append("=");                        //Признак начала передаваемых данных
    QSqlQuery query= QSqlQuery(db);
    QString s="";
    s=s+"SELECT st_x(obj_location), st_y(obj_location), st_z(obj_location), direction FROM own_forces.combatobject_location WHERE combat_hierarchy='"+q+"';";
    if (!query.exec(s)) {
        ui->logField->append(tr("%1 cant delete").arg(QTime::currentTime().toString("hh:mm:ss")));
    }
    else {
        ui->logField->append(tr("%1 delete %2 from table").arg(QTime::currentTime().toString("hh:mm:ss")).arg(q));
        while (query.next()) {
            //*number=query.value(0).toInt();
            answer.append(query.value(0).toString());
            answer.append(";");
            answer.append(query.value(1).toString());
            answer.append(";");
            answer.append(query.value(2).toString());
            answer.append(";");
            answer.append(query.value(3).toString());
            answer.append(";");
        }
    }
    answer.append("\r");
    qDebug()<<answer;
    return answer;
}

void MainWindow::setIp() {
    IpDialog dia;
    if (dia.exec()) {
        myIp=dia.value();
    }
}

void MainWindow::readDatagram()
{
    makeLogNote("получена датаграмма квитанция");
    QByteArray datagram;
    datagram.resize(udpSocket.pendingDatagramSize());
    udpSocket.readDatagram(datagram.data(), datagram.size());
    QStringList list=converter->decode(datagram);
    int x=logger->makeNote(1,QDate::currentDate().toString("dd.MM.yyyy")+QTime::currentTime().toString("hh:mm:ss.zzz"),1,list.at(12),3);
    if (x==0) {
        makeLogNote("получена датаграмма");
    }
        else {
        if (x==2) makeLogNote("FAIL2");
        else makeLogNote("FAIL1");
    }
}

void MainWindow::on_combObjTableBut_clicked()
{
    model->setTable("own_forces.combatobject_location");
    ui->tableView->setModel(model);
    model->select();
    makeLogNote("Загружены данные combat objects");
}

void MainWindow::on_logTableBut_3_clicked()
{
    model->setTable("log.log_table_message");
    ui->tableView->setModel(model);
    model->select();
    makeLogNote("Загружены данные log table");
}
