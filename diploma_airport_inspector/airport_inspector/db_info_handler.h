#ifndef DB_INFO_HANDLER_H
#define DB_INFO_HANDLER_H

#include <QObject>
#include <QDate>

#include "database_reader.h"

class db_info_handler : public QObject
{
    Q_OBJECT
public:
    explicit db_info_handler(QObject *parent = nullptr);

    void getAirportListLike(database_reader* db_reader, QString pattern);

    void getFlights(database_reader* db_reader,
                    QString departure,
                    QString destination,
                    QDate date);

    void getFlights(database_reader* db_reader,
                    QString departure,
                    QString destination);

    void getFlightStatMonthly(database_reader* db_reader,
                              QString& airportName,
                              bool arrivingFligts);

signals:

    void sig_sendAirportList(QStringList airports);
    void sig_sendFlightList(QAbstractItemModel *model);
    void sig_sendMonthDepartureStat(QList<double>& stats);
    void sig_sendMonthArrivalStat(QList<double>& stats);

private:
    QVector<QString> airports;

    static const QString requestToCountMonthlyDepartures;
    static const QString requestToCountMonthlyArrivals;
};

#endif // DB_INFO_HANDLER_H
