#include "database.h"
#include <QSqlError>

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();


}

DataBase::~DataBase()
{
    releaseLastModel();
    delete dataBase;
}

void DataBase::releaseLastModel() {
    if (lastModel) delete lastModel; lastModel = nullptr;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());


    ///Тут должен быть код ДЗ


    bool status;
    status = dataBase->open( );
    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
///*!
// * \brief Метод формирует запрос к БД.
// * \param request - SQL запрос
// * \return
// */
//void DataBase::RequestToDB(QString request)
//{
//    QSqlQuery query(*dataBase);
//    bool res = query.exec(request);

//    if (!res) {
//        qDebug() << "Failed to execute query";
//    }

//    QTableWidget* tableWg = new QTableWidget();

//    tableWg->setColumnCount(3);
//    tableWg->setRowCount(0);
//    QStringList hdrs;
//    hdrs << "Название" << "Год выпуска" << "Жанр";
//    tableWg->setHorizontalHeaderLabels(hdrs);

//    uint32_t conterRows = 0;

//    while(query.next()){
//        QString v0 = query.value(0).toString();
//        qDebug() << v0;

//        tableWg->insertRow(conterRows);

//        QString str;
//        for(int i = 0; i<tableWg->columnCount(); ++i){

//            str = query.value(i).toString();
//            QTableWidgetItem *item = new QTableWidgetItem(str);
//            tableWg->setItem(tableWg->rowCount() - 1, i, item);
//        }
//        ++conterRows;
//    }

//    emit sig_SendDataFromDB(tableWg, requestAllFilms);
//}


void DataBase::RequestFilms(filmRequestType_e filmSet)
{
    if (filmSet == filmRequestType_e::All) {
        RequestAllFilms();
    }
    else if (filmSet == filmRequestType_e::Comedy) {
        RequestData("SELECT title, description FROM film f JOIN film_category USING(film_id) JOIN category using(category_id) WHERE category.name = 'Comedy'");
    }
    else if (filmSet == filmRequestType_e::Horror) {
        RequestData("SELECT title, description FROM film f JOIN film_category USING(film_id) JOIN category using(category_id) WHERE category.name = 'Horror'");
    }
}

/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestData(QString request)
{
    QSqlQueryModel* model = new QSqlQueryModel;
    model->setQuery(request,*dataBase);

    if (model->lastError().isValid())
        qDebug() << model->lastError();

    model->setHeaderData(0, Qt::Horizontal, tr("Название"));
    model->setHeaderData(1, Qt::Horizontal, tr("Описание"));

    releaseLastModel();
    lastModel = model;

    emit sig_SendDataFromDB2(model);
}

void DataBase::RequestAllFilms()
{
    QSqlTableModel* model = new QSqlTableModel(nullptr, *dataBase);

    model->setTable("film");
    model->select();

    if (model->lastError().isValid())
        qDebug() << model->lastError();

    model->setHeaderData(0, Qt::Horizontal, tr("Id"));
    model->setHeaderData(1, Qt::Horizontal, tr("Название"));
    model->setHeaderData(2, Qt::Horizontal, tr("Описание"));
    model->setHeaderData(3, Qt::Horizontal, tr("Год выпуска"));
    model->setHeaderData(4, Qt::Horizontal, tr("Язык"));
    model->setHeaderData(5, Qt::Horizontal, tr("Оригинальный язык"));
    model->setHeaderData(6, Qt::Horizontal, tr("Срок аренды"));
    model->setHeaderData(7, Qt::Horizontal, tr("Цена"));
    model->setHeaderData(8, Qt::Horizontal, tr("продолжительность"));

    releaseLastModel();
    lastModel = model;

    emit sig_SendDataFromDB2(model);
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
