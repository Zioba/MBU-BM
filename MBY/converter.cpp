#include "converter.h"

Converter::Converter()
{

}

QByteArray Converter::encode(QStringList list)
{
    QByteArray datagram;
    datagram.append(convertIpBinary(list.at(0)));           //адрес источника
    datagram.append(convertIpBinary(list.at(1)));           //адрес получателя
    datagram.append(convertToBinary(list.at(2),16));  //нули, номер протокола
    datagram.append(convertToBinary(list.at(3),16));  //длина UDP
    datagram.append(convertToBinary(list.at(4),16));  //порт источника
    datagram.append(convertToBinary(list.at(5),16));  //порт получателя
    datagram.append(convertToBinary(list.at(6),16));  //длина
    datagram.append(convertToBinary(list.at(7),16));  //чексумма
    datagram.append(list.at(8));                      //тип сообщения
    datagram.append(convertToBinary(list.at(9),28));  //УНИКАЛЬНЫЙ номер
    datagram.append(convertToBinary(list.at(10),16)); //всего датаграмм
    datagram.append(convertToBinary(list.at(11),16)); //номер в сообщении
    datagram.append(list.at(12));                     //данные
    return datagram;
}

QStringList Converter::decode(QByteArray datagram)
{
    QStringList list;
    QString source="";
    for (int i=0;i<32;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertIpDex(source);
    source="";
    for (int i=32;i<64;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertIpDex(source);
    source="";
    for (int i=64;i<80;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertToDex(source);
    source="";
    for (int i=80;i<96;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertToDex(source);
    source="";
    for (int i=96;i<112;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertToDex(source);
    source="";
    for (int i=112;i<128;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertToDex(source);
    source="";
    for (int i=128;i<144;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertToDex(source);
    source="";
    for (int i=144;i<160;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertToDex(source);
    source="";
    for (int i=160;i<164;i++)
    {
        source+=datagram.at(i);
    }
    list<<source;
    source="";
    for (int i=164;i<192;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertToDex(source);
    source="";
    for (int i=192;i<208;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertToDex(source);
    source="";
    for (int i=208;i<224;i++)
    {
        source+=datagram.at(i);
    }
    list<<convertToDex(source);
    source="";
    for (int i=224;i<datagram.size();i++)
    {
        source+=datagram.at(i);
    }
    list<<source;
    return list;
}

QByteArray Converter::generateReceiptResponse(QStringList list)
{
    QByteArray datagram;
    datagram.append(convertIpBinary(list.at(0)));           //адрес источника
    datagram.append(convertIpBinary(list.at(1)));           //адрес получателя
    datagram.append(convertToBinary(list.at(2),16));  //нули, номер протокола
    datagram.append(convertToBinary(list.at(3),16));  //длина UDP
    datagram.append(convertToBinary(list.at(4),16));  //порт источника
    datagram.append(convertToBinary(list.at(5),16));  //порт получателя
    datagram.append(convertToBinary(list.at(6),16));  //длина
    datagram.append(convertToBinary(list.at(7),16));  //чексумма
    datagram.append(list.at(8));                      //тип сообщения
    datagram.append(convertToBinary(list.at(9),28));  //УНИКАЛЬНЫЙ номер подтверждаемого сообщения
    datagram.append(convertToBinary(list.at(10),16)); //номер в сообщении
    datagram.append(convertToBinary(list.at(11),16)); //результат доставки
    return datagram;
}

QString Converter::convertIpBinary(QString s)
{
    QString answer="";
    int i=0;
    for (int j=0;j<4;j++) {
        QString source="";
        while ((i<s.size()) && (s.at(i)!='.')) {
            source.append(s.at(i));
            i++;
        }
        i++;
        answer.append(convertToBinary(source,8));
    }
    return answer;
}

QString Converter::convertToBinary(QString s, int len)
{
    long long answer=s.toLong(Q_NULLPTR,10);
    QString b2=QString("%1").arg(answer,0,2);
    while (b2.size()<len) {
        b2="0"+b2;
    }
    return b2;
}

QString Converter::convertIpDex(QString s)
{
    QString answer="";
    int i=0;
    for (int j=0;j<4;j++) {
        QString source="";
        source.append(s.at(i));
        i++;
        while (i%8!=0) {
            source.append(s.at(i));
            i++;
        }
        answer.append(convertToDex(source));
        if (i<32) answer.append(".");
    }
    return answer;
}

QString Converter::convertToDex(QString s)
{
    long long answer=s.toLong(Q_NULLPTR,2);
    QString b2=QString("%1").arg(answer,0,10);
    return b2;
}
