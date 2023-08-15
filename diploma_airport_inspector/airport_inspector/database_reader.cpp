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
