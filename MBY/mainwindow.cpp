#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deldialog.h"
#include "ipdialog.h"
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle( "MBU" );
    QPoint pos( 900, 0 );
    this->move( pos );
    ui->setupUi( this );
    db = QSqlDatabase::addDatabase( "QPSQL" );
    db.setHostName( "127.0.0.1" );
    db.setPort( 5432 );
    db.setDatabaseName( "Database_MBU" );
    db.setUserName( "postgres" );
    db.setPassword( "qwerty" );
    if ( !db.open() ) {
        makeLogNote( db.lastError().text() );
    }
    else {
        makeLogNote( "Start working" );
    }
    model = new QSqlTableModel( this, db );
    udpSocket.bind( 5824 );
    on_combObjTableBut_clicked();
    logger = new Logger( db );
    converter = new Converter();
    setIp();
    //myIp.setAddress( "127.0.0.1" );
    targetIp.setAddress( "127.0.0.1" );
    myPort = "5824";
    targetPort = "5825";
    connect( &udpSocket, SIGNAL( readyRead() ), this, SLOT( readDatagram() ) );
    /*QList<QHostAddress> list = QNetworkInterface::allAddresses();

     for(int nIter=0; nIter<list.count(); nIter++)

      {
          if(!list[nIter].isLoopback())
              if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
            qDebug() << list[nIter].toString();

      }*/
}

MainWindow::~MainWindow()
{
    delete ui;
    delete logger;
    delete converter;
}

void MainWindow::makeLogNote(QString s)
{
    ui->logField->append( tr( "%1 %2" ).arg( QTime::currentTime().toString( "hh:mm:ss" ) ).arg( s ) );
}

void MainWindow::readDatagram()
{
    QByteArray datagram;
    datagram.resize( udpSocket.pendingDatagramSize() );
    udpSocket.readDatagram( datagram.data(), datagram.size() );
    makeLogNote( "recived datagram" );
    QStringList messageMembersList = converter->decode( datagram );
    QString message = messageMembersList.at( 12 );
    parsingMessage( messageMembersList.at( 12 ) );
    bool x = logger->makeNote( 1, getCurrentDateAndTime(), 1, messageMembersList.at( 12 ), 3 );
    if ( x == true ) {
        makeLogNote( "получена датаграмма" );
    }
        else {
        makeLogNote( "ошибка занесения в лог" );
    }
    QStringList dataMembersList;
    dataMembersList << myIp.toString()
          << targetIp.toString()
          << "17"
          << QString::number(224)
          << myPort
          << targetPort
          << QString::number( messageMembersList.at( 12 ).length() )
          << ""  //доделать чексумму
          << "0010"
          << QString::number( 5 )
          << "1"
          << "1";
    QByteArray datagram2 = converter->generateReceiptResponse( dataMembersList );
    qDebug() << targetPort.toLong( Q_NULLPTR, 10 );
    udpSocket.writeDatagram( datagram2, targetIp, 5825 );
    x = logger->makeNote( 1, getCurrentDateAndTime(), 1, datagram2, 2 );
    if ( x == true ) {
        makeLogNote( "отправлено подтверждение получения" );
    }
        else {
        makeLogNote( "ошибка занесения в лог" );
    }
}

void MainWindow::on_exitButton_clicked()
{
   this->close();
}

void MainWindow::on_updBut_clicked()
{
    model->select();
    ui->logField->append( tr( "%1 table update" ).arg( QTime::currentTime().toString( "hh:mm:ss" ) ) );
}

void MainWindow::setIp() {
    IpDialog dia;
    if ( dia.exec() ) {
        myIp = dia.value();
    }
}

void MainWindow::on_clearBut_clicked()
{
    ui->logField->clear();
}

void MainWindow::on_combObjTableBut_clicked()
{
    model->setTable( "own_forces.combatobject_location" );
    ui->tableView->setModel( model );
    model->select();
    makeLogNote( "Загружены данные combat objects" );
}

void MainWindow::on_logTableBut_3_clicked()
{
    model->setTable( "log.log_table_message" );
    ui->tableView->setModel( model );
    model->select();
    makeLogNote( "Загружены данные log table" );
}

void MainWindow::parsingMessage( QString s )
{
    qDebug() << s;
    if ( s.at( 0 ) == "1" ) {
        makeLogNote( "oшибка, данные сжаты" );
        //не работаем пока со сжатием данных
        return;
    }
    bool trigger = false;
    QString object = "";
    for ( int i = 1; i < 7; i++ )
    {
        if ( s.at( i ) != '0' ) {
            trigger = true;
            object += s.at( i );
        }
        else if ( trigger ) {
            object += s.at( i );
        }
    }
    QString source = "";
    trigger = false;
    for ( int i = 7; i < 13; i++ )
    {
        if ( s.at( i ) != '0' ) {
            trigger = true;
            source += s.at( i );
        }
        else if ( trigger ) {
            source += s.at( i );
        }
    }
    QString data = "";
    for ( int i = 18; i < s.size(); i++ )
    {
        data += s.at( i );
    }
    QString x;
    QString y;
    QString z;
    QString dir;
    int i = 0;
    x = assistParser( data, i );
    y = assistParser( data, i );
    z = assistParser( data, i );
    dir = assistParser( data, i );
//    QSqlQuery query = QSqlQuery( db );
//    db.transaction();
//    query.prepare( "UPDATE own_forces.combatobject_location"
//                   "SET obj_location=ST_MakePoint(?,?,?), direction=?, date_edit=?"
//                   "WHERE combat_hierarchy=?;" );
//    query.addBindValue( x );
//    query.addBindValue( y );
//    query.addBindValue( z );
//    query.addBindValue( dir );
//    query.addBindValue( getCurrentDateAndTime() );
//    query.addBindValue( object );
//    query.exec();
//    qDebug() << query.lastError();
//    qDebug() << db.commit();
    //=====================================================================
    QSqlQuery query = QSqlQuery( db );
    QString queryString;
    queryString = "UPDATE own_forces.combatobject_location SET obj_location=ST_MakePoint ("+x+","+y+","+z+"), direction='"+dir+
            "', date_edit='"+ getCurrentDateAndTime() + "' WHERE combat_hierarchy='"+object+"';";
    if ( query.exec(queryString) ) {
        makeLogNote( "база обновлена" );
    }
    else {
        makeLogNote( "ошибка запроса" );
    }
}

QString MainWindow::getCurrentDateAndTime()
{
    return QDate::currentDate().toString( "dd.MM.yyyy" ) + QTime::currentTime().toString( "hh:mm:ss.zzz" );
}

QString MainWindow::assistParser( QString data, int &counter )
{
    QString answer = "";
    while ( data.at( counter ) != ';' ) {
        answer.append( data.at( counter ) );
        counter++;
    }
    counter++;
    return answer;
}
