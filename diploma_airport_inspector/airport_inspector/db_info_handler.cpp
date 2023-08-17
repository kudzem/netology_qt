#include "db_info_handler.h"

db_info_handler::db_info_handler(QObject *parent)
    : QObject{parent}
{

}

void
db_info_handler::getAirportListLike(database_reader* db_reader, QString pattern)
{
    if (pattern == "Любой") pattern = "";

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

void
db_info_handler::getFlights(database_reader* db_reader,
                            QString departure,
                            QString destination,
                            QDate date)
{
    if (departure == "Любой") departure = "";
    if (destination == "Любой") destination = "";

    const QString request = "SELECT flight_no, dep.airport_name->>'ru', scheduled_departure, arrival.airport_name->>'ru', scheduled_arrival  \
                             FROM bookings.flights f\
                             JOIN bookings.airports_data dep on dep.airport_code = f.departure_airport \
                             JOIN bookings.airports_data arrival on arrival.airport_code = f.arrival_airport \
                             WHERE dep.airport_name->>'ru' LIKE '%" + departure + "' \
                             AND arrival.airport_name->>'ru' LIKE '%" + destination + "' \
                             AND CAST(scheduled_departure as DATE) = '" + date.toString("yyyy-MM-dd") + "' \
                             ORDER BY scheduled_departure";

    qDebug() << request;
    qDebug() << date.toString("yyyy-MM-dd");

    QSqlQueryModel* flightList = db_reader->requestModelQuery(request);

    flightList->setHeaderData(0, Qt::Horizontal, tr("Рейс"));
    flightList->setHeaderData(1, Qt::Horizontal, tr("Из"));
    flightList->setHeaderData(2, Qt::Horizontal, tr("Время вылета"));
    flightList->setHeaderData(3, Qt::Horizontal, tr("В"));
    flightList->setHeaderData(4, Qt::Horizontal, tr("Время прибытия"));

    emit sig_sendFlightList(flightList);
}

void
db_info_handler::getFlights(database_reader* db_reader,
                            QString departure,
                            QString destination)
{
    if (departure == "Любой") departure = "";
    if (destination == "Любой") destination = "";

    const QString request = "SELECT flight_no, dep.airport_name->>'ru', scheduled_departure, arrival.airport_name->>'ru', scheduled_arrival  \
                             FROM bookings.flights f\
                             JOIN bookings.airports_data dep on dep.airport_code = f.departure_airport \
                             JOIN bookings.airports_data arrival on arrival.airport_code = f.arrival_airport \
                             WHERE dep.airport_name->>'ru' LIKE '%" + departure + "' \
                             AND arrival.airport_name->>'ru' LIKE '%" + destination + "' \
                             ORDER BY scheduled_departure";

    qDebug() << request;

    QSqlQueryModel* flightList = db_reader->requestModelQuery(request);

    flightList->setHeaderData(0, Qt::Horizontal, tr("Рейс"));
    flightList->setHeaderData(1, Qt::Horizontal, tr("Из"));
    flightList->setHeaderData(2, Qt::Horizontal, tr("Время вылета"));
    flightList->setHeaderData(3, Qt::Horizontal, tr("В"));
    flightList->setHeaderData(4, Qt::Horizontal, tr("Время прибытия"));

    emit sig_sendFlightList(flightList);
}
