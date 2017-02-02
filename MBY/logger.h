#ifndef LOGGER_H
#define LOGGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRecord>


class Logger
{
public:
    Logger(QSqlDatabase);

    // 0 - success; 1 - error
    int makeNote(int, QString, int, QString, int);

private:
    QSqlDatabase db;
};

#endif // LOGGER_H
