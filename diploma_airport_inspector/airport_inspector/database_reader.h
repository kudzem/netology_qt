#ifndef DATABASE_READER_H
#define DATABASE_READER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>

class database_reader : public QObject
{
    Q_OBJECT
public:
    database_reader();
    virtual ~database_reader();
    void ConnectToDB();
    void DisconnectFromDB();

    QSqlQuery* requestRawQuery(QString request);
    QSqlQueryModel* requestModelQuery(QString request);

private:
    QSqlDatabase* db = nullptr;
    QSqlQueryModel* model = nullptr;

signals:
    void sig_SendStatusConnection(bool);
};

#endif // DATABASE_READER_H
