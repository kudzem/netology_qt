#ifndef DATABASE_READER_H
#define DATABASE_READER_H

#include <QObject>
#include <QSqlDatabase>

class database_reader : public QObject
{
    Q_OBJECT
public:
    database_reader();
    virtual ~database_reader();
    void ConnectToDB();
    void DisconnectFromDB();

private:
    QSqlDatabase* db = nullptr;

signals:
    void sig_SendStatusConnection(bool);
};

#endif // DATABASE_READER_H
