#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QVector>



#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

//Количество полей данных необходимых для подключения к БД
#define NUM_DATA_FOR_CONNECT_TO_DB 5

//Перечисление полей данных
enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

//Типы запросов
typedef
enum class filmRequestType : uint8_t {

    All,
    Comedy,
    Horror

} filmRequestType_e;


class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    //void RequestToDB(QString request);
    void RequestData(QString request);
    void RequestAllFilms();
    void RequestFilms(filmRequestType_e filmSet);
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);
    void releaseLastModel();


signals:

   //void sig_SendDataFromDB(const QTableWidget *tableWg, int typeR);
   void sig_SendDataFromDB2(QAbstractItemModel *model);
   void sig_SendStatusConnection(bool);



private:

    QSqlDatabase* dataBase;
    QAbstractItemModel* lastModel = nullptr;
};

#endif // DATABASE_H
