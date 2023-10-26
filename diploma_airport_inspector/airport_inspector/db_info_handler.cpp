#include "db_info_handler.h"
#include <QSqlError>

db_info_handler::db_info_handler(QObject *parent)
    : QObject{parent}
{
    airport_list_fetched = false;
}

void
db_info_handler::getAllAirports(database_reader* db_reader)
{
    const QString request = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data ORDER BY airport_name->>'ru'";

    //qDebug() << request;
    QSqlQuery* airportList = db_reader->requestRawQuery(request);

    while(airportList->next()){
        QString v0 = airportList->value(0).toString();
        //qDebug() << v0;
        all_airports << v0;
    }

    airport_list_fetched = true;
}

void
db_info_handler::getAirportListLikeFromDB(database_reader* db_reader, QString pattern)
{
    if (pattern == "Любой")
    {
        pattern = "";
    }
    else if(pattern.size())
    {
        pattern[0] = pattern[0].toUpper();
    }


    const QString request = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data WHERE airport_name->>'ru' LIKE '" + pattern + "%' ORDER BY airport_name->>'ru'";

    //qDebug() << request;
    QSqlQuery* airportList = db_reader->requestRawQuery(request);

    QStringList airports;

    while(airportList->next()){
        QString v0 = airportList->value(0).toString();
        //qDebug() << v0;
        airports << v0;
    }

    emit sig_sendAirportList(airports);
}

void
db_info_handler::getAirportListLikeInternally(database_reader* db_reader, QString pattern)
{
    if (pattern == "Любой")
    {
        pattern = "";
        emit sig_sendAirportList(all_airports);
        return;
    }

    if(pattern.size())
    {
        pattern[0] = pattern[0].toUpper();
    }

    QStringList airports;


    QString airport;
    foreach (airport, all_airports)
    {
        if (airport.startsWith(pattern))
        {
            airports << airport;
        }
    }

    emit sig_sendAirportList(airports);
}

void
db_info_handler::getAirportListLike(database_reader* db_reader, QString pattern)
{
    if (airport_list_fetched)
    {
        getAirportListLikeInternally(db_reader, pattern);
    }
    else
    {
        getAirportListLikeFromDB(db_reader, pattern);
    }
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

    //qDebug() << request;
    //qDebug() << date.toString("yyyy-MM-dd");

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

    //qDebug() << request;

    QSqlQueryModel* flightList = db_reader->requestModelQuery(request);

    flightList->setHeaderData(0, Qt::Horizontal, tr("Рейс"));
    flightList->setHeaderData(1, Qt::Horizontal, tr("Из"));
    flightList->setHeaderData(2, Qt::Horizontal, tr("Время вылета"));
    flightList->setHeaderData(3, Qt::Horizontal, tr("В"));
    flightList->setHeaderData(4, Qt::Horizontal, tr("Время прибытия"));

    emit sig_sendFlightList(flightList);
}


const QString db_info_handler::requestToCountMonthlyDepartures =
    "SELECT count(*), EXTRACT(MONTH FROM scheduled_departure) as month  \
     FROM bookings.flights f \
     JOIN bookings.airports_data dep on dep.airport_code = f.departure_airport \
     WHERE dep.airport_name->>'ru' LIKE '%AIRPORT_PATTERN_TO_REPLACE' \
     AND (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) \
     GROUP BY month \
     ORDER BY month";


const QString db_info_handler::requestToCountMonthlyArrivals =
    "SELECT count(*), EXTRACT(MONTH FROM scheduled_arrival) as month  \
    FROM bookings.flights f \
    JOIN bookings.airports_data arrival on arrival.airport_code = f.arrival_airport \
      WHERE arrival.airport_name->>'ru' LIKE '%AIRPORT_PATTERN_TO_REPLACE' \
      AND (scheduled_arrival::date > date('2016-08-31') and scheduled_arrival::date <= date('2017-08-31')) \
      GROUP BY month \
      ORDER BY month";

const QString db_info_handler::requestToCountDailyDepartures =
    "SELECT count(*), EXTRACT(DAY FROM scheduled_departure) as day  \
    FROM bookings.flights f \
    JOIN bookings.airports_data dep on dep.airport_code = f.departure_airport \
      WHERE dep.airport_name->>'ru' LIKE '%AIRPORT_PATTERN_TO_REPLACE' \
      AND EXTRACT(MONTH FROM scheduled_arrival) = MONTH_PATTERN_TO_REPLACE \
      AND (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) \
      GROUP BY day \
      ORDER BY day";

const QString db_info_handler::requestToCountDailyArrivals =
    "SELECT count(*), EXTRACT(DAY FROM scheduled_arrival) as day  \
    FROM bookings.flights f \
    JOIN bookings.airports_data arrival on arrival.airport_code = f.arrival_airport \
      WHERE arrival.airport_name->>'ru' LIKE '%AIRPORT_PATTERN_TO_REPLACE' \
      AND EXTRACT(MONTH FROM scheduled_arrival) = MONTH_PATTERN_TO_REPLACE \
      AND (scheduled_arrival::date > date('2016-08-31') and scheduled_arrival::date <= date('2017-08-31')) \
    GROUP BY day \
    ORDER BY day";

void
db_info_handler::getFlightStatMonthly(database_reader* db_reader,
                                      QString& airportName,
                                      bool arrivingFlights)
{
    if (airportName == "Любой") airportName = "";

    QString request;

    if(arrivingFlights)
    {
        request = requestToCountMonthlyArrivals;
    }
    else
    {
        request = requestToCountMonthlyDepartures;
    }

    request = request.replace(QString("AIRPORT_PATTERN_TO_REPLACE"), airportName);

    //qDebug() << request;
    QSqlQuery* airportList = db_reader->requestRawQuery(request);

    //qDebug() << airportList->lastError();

    QVector<double> stats;
    stats.resize(12);

    while(airportList->next()){
        int flightsPerMonth = airportList->value(0).toInt();
        int month = airportList->value(1).toInt();
        //qDebug() << flightsPerMonth;
        stats[month-1] = flightsPerMonth;
    }

    if(arrivingFlights)
    {
        emit sig_sendMonthArrivalStat(stats);
    }
    else
    {
        emit sig_sendMonthDepartureStat(stats);
    }
}


void
db_info_handler::getFlightStatDaily(database_reader* db_reader,
                                    QString& airportName,
                                    int month,
                                    bool arrivingFlights)
{
    if (airportName == "Любой") airportName = "";

    QString request;

    if(arrivingFlights)
    {
        request = requestToCountDailyArrivals;
    }
    else
    {
        request = requestToCountDailyDepartures;
    }

    request = request.replace(QString("AIRPORT_PATTERN_TO_REPLACE"), airportName);
    request = request.replace(QString("MONTH_PATTERN_TO_REPLACE"), QString::number(month));

    //qDebug() << request;
    QSqlQuery* airportList = db_reader->requestRawQuery(request);

    //qDebug() << airportList->lastError();

    QVector<double> stats;
    stats.resize(31);

    while(airportList->next()){
        int flightsPerDay = airportList->value(0).toInt();
        int day = airportList->value(1).toInt();
        //qDebug() << flightsPerDay;
        stats[day-1] = flightsPerDay;
    }

    if(arrivingFlights)
    {
        emit sig_sendDayArrivalStat(stats);
    }
    else
    {
        emit sig_sendDayDepartureStat(stats);
    }
}

