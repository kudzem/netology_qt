#include "db_info_handler.h"

db_info_handler::db_info_handler(QObject *parent)
    : QObject{parent}
{

}

void
db_info_handler::getAirportListLike(database_reader* db_reader, QString pattern)
{
    const QString request = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data WHERE airport_name->>'ru' LIKE '" + pattern + "%' ORDER BY airport_name->>'ru'";

    qDebug() << request;
    QSqlQuery* airportList = db_reader->requestRawQuery(request);

    uint32_t conterRows = 0;

    QStringList airports;

    while(airportList->next()){
        QString v0 = airportList->value(0).toString();
        qDebug() << v0;
        ++conterRows;
        airports << v0;
    }

    //emit sig_SendDataFromDB(tableWg, requestAllFilms);
    emit sig_sendAirportList(airports);
}
