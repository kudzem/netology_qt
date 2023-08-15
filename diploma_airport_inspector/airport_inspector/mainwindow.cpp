#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db_reader = new database_reader;
    connect(db_reader, &database_reader::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    db_reader->ConnectToDB();
}

MainWindow::~MainWindow()
{
    delete db_reader;
    delete ui;
}


void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status)
    {
        setWindowTitle("Инспектор Аэропортов - Подключено к БД");
    }
    else
    {
        setWindowTitle("Инспектор Аэропортов - Не подключено к БД");
    }
}


void MainWindow::on_pb_search_clicked()
{
}

