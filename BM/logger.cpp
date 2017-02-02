#include "logger.h"

Logger::Logger(QSqlDatabase db)
{
    this->db=db;
}

//2)тип_сообщения, 3)дата, 4)отправитель/получатель, 5)пакет, 6)состояние обработки
int Logger::makeNote(int type, QString date, int x, QString package, int status)
{
    QSqlQuery query= QSqlQuery(db);
    QString s="";
    s=s+"SELECT * FROM log.log_table_message ORDER BY id_note DESC LIMIT 1;";
    if (!query.exec(s)) {
        return 2;
    }
    else {
        int id;
        while (query.next()) {
            id=query.value(0).toInt();
        }
        s="";
        id++;
        QSqlQuery query2= QSqlQuery(db);
        s=s+"INSERT INTO log.log_table_message VALUES ('"+QString::number(id)+"','"
                                                         +QString::number(type)+"','"
                                                         +date+"','"
                                                         +QString::number(x)+"','"
                                                         +package+"','"
                                                         +QString::number(status)+"');";
        if (!query2.exec(s)) {
            return 1;
        }
        else {
            return 0;
        }
    }
}
