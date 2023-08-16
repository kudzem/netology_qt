#include "database_reader.h"
#include <QVector>

const QVector<QString> credentials = { "981757-ca08998.tmweb.ru", "demo", "netology_usr_cpp", "CppNeto3", "5432" };

enum CredentialsFieldsForConnect{
    CredentialsFieldHostName,
    CredentialsFieldDBName,
    CredentialsFieldLogin,
    CredentialsFieldPass,
    CredentialsFieldDBPort
};

const QString POSTGRE_DRIVER = "QPSQL";
const QString DB_NAME = "MyDB";

database_reader::database_reader()
{
    db = new QSqlDatabase();

    *db = QSqlDatabase::addDatabase(POSTGRE_DRIVER, DB_NAME);
}

database_reader::~database_reader()
{
    DisconnectFromDB();
    delete db;
}


void database_reader::ConnectToDB()
{
    db->setHostName(credentials[CredentialsFieldHostName]);
    db->setDatabaseName(credentials[CredentialsFieldDBName]);
    db->setUserName(credentials[CredentialsFieldLogin]);
    db->setPassword(credentials[CredentialsFieldPass]);
    db->setPort(credentials[CredentialsFieldDBPort].toInt());

    bool status;
    status = db->open();
    emit sig_SendStatusConnection(status);
}


void database_reader::DisconnectFromDB()
{
    db->close();
    emit sig_SendStatusConnection(0);
}

//void database_reader::getListOfAirports()
//{
//    const QString request = "SELECT airport_name->>'ru' as \"airportName\", airport_code FROM bookings.airports_data";

//    model->setQuery(request,*db);

//    if (model->lastError().isValid())
//        qDebug() << model->lastError();

//    model->setHeaderData(0, Qt::Horizontal, tr("Название"));
//    model->setHeaderData(1, Qt::Horizontal, tr("Описание"));

//    releaseLastModel();
//    lastModel = model;

//    emit sig_SendDataFromDB2(model);
//}

QSqlQuery*
database_reader::requestRawQuery(QString requestStr)
{
    QSqlQuery* query = new QSqlQuery(*db);
    bool res = query->exec(requestStr);

    if (!res) {
        qDebug() << "Failed to execute query";
    }

    return query;
}
