#include <QCoreApplication>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRelation>

#include <QtNetwork/QUdpSocket>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase* db = new QSqlDatabase();

    QString index_col = "id";
    QString display_col = "display?";

    QSqlRelation* table = new QSqlRelation("Clients",index_col,display_col);

    delete table;
    delete db;

    QUdpSocket* sock = new QUdpSocket(&a);
    //sock->connect();

    delete sock;

    return a.exec();
}
