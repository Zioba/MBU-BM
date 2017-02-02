#ifndef CONVERTER_H
#define CONVERTER_H

#include <QString>
#include <QStringList>
#include <QDebug>


class Converter
{
public:
    Converter();
    //закодировать датаграмму
    QByteArray encode(QStringList);
    //раскодировать датаграмму
    QStringList decode(QByteArray);
    //приведение длины строки к нужному формату
    QString dobei(QString, int);
private:
    //конвертирует обычный ip адрес в бинарную строку
    QString convertIpBinary(QString);
    //преобразование заданной строки в бинарную, и подгонка её к нужному размеру
    QString convertToBinary(QString, int);
    //преобразование бинарной строки в обычный ip адрес
    QString convertIpDex(QString);
    //преобразование бинарной строки в обычную
    QString convertToDex(QString s);
};

#endif // CONVERTER_H
