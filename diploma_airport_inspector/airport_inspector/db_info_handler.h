#ifndef DB_INFO_HANDLER_H
#define DB_INFO_HANDLER_H

#include <QObject>

#include "database_reader.h"

class db_info_handler : public QObject
{
    Q_OBJECT
public:
    explicit db_info_handler(QObject *parent = nullptr);

    void getAirportListLike(database_reader* db_reader, QString pattern);

signals:

    void sig_sendAirportList(QStringList airports);

private:
    QVector<QString> airports;

};

#endif // DB_INFO_HANDLER_H
