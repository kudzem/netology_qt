#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->le_departure->installEventFilter(this);
    ui->le_destination->installEventFilter(this);
    ui->le_date->installEventFilter(this);

    db_reader = new database_reader;
    connect(db_reader, &database_reader::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    db_reader->ConnectToDB();

    db_handler = new db_info_handler;

    departList = new QListView(ui->tab_functions);
    departList->hide();
    departList->installEventFilter(ui->tab_functions);

    destList = new QListView(ui->tab_functions);
    destList->hide();
    destList->installEventFilter(ui->tab_functions);

    calendar = new QCalendarWidget(ui->tab_functions);
    calendar->hide();
    calendar->installEventFilter(ui->tab_functions);

    connect(departList, &QListView::clicked, this, &MainWindow::departure_chosen);
    connect(destList, &QListView::clicked, this, &MainWindow::destination_chosen);

    connect(calendar, &QCalendarWidget::selectionChanged, this, &MainWindow::date_chosen);

    //departList->pos().setX(ui->le_departure->geometry().x() + ui->le_departure->pos().x() + ui->tabWidget->pos().x());
    //departList->pos().setY(ui->le_departure->geometry().y() + ui->tabWidget->pos().x());

    departListModel = new QStringListModel(this);
    departList->setModel(departListModel);
    departList->setGeometry(ui->le_departure->geometry().left() + ui->tab_functions->geometry().left(),
                            ui->le_departure->geometry().bottom() + ui->tab_functions->geometry().bottom(),
                            ui->le_departure->geometry().width(),
                            10*ui->le_departure->geometry().height());

    destListModel = new QStringListModel(this);
    destList->setModel(destListModel);
    destList->setGeometry(ui->le_destination->geometry().left() + ui->tab_functions->geometry().left(),
                            ui->le_destination->geometry().bottom() + ui->tab_functions->geometry().bottom(),
                            ui->le_destination->geometry().width(),
                            10*ui->le_destination->geometry().height());

    calendar->setGeometry(ui->le_date->geometry().left() + ui->tab_functions->geometry().left(),
                          ui->le_date->geometry().bottom() + ui->tab_functions->geometry().bottom(),
                          300,
                          200);

    connect(db_handler, &db_info_handler::sig_sendAirportList, this, &MainWindow::showAirportList);
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


void MainWindow::on_le_departure_textChanged(const QString &arg1)
{
    db_handler->getAirportListLike(db_reader, ui->le_departure->text());
}

// Implementation
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if( event->type() == QEvent::FocusIn )
    {
        if(watched == ui->le_departure || watched == departList)
        {
            db_handler->getAirportListLike(db_reader, ui->le_departure->text());
            departList->show();
        }
        else if(watched == ui->le_destination || watched == destList)
        {
                db_handler->getAirportListLike(db_reader, ui->le_destination->text());
                destList->show();
        }
        else if(watched == ui->le_date)
        {
                calendar->show();
        }
    }
    else if( event->type() == QEvent::FocusOut )
    {
        if(watched == ui->le_departure)
        {
            departList->hide();
        }
        else if(watched == ui->le_destination)
        {
            destList->hide();
        }
        else if(watched == ui->le_date)
        {
            calendar->hide();
        }
    }

    return false; // We return false to ignore the event and allow the child to recieve the event normally
}

void MainWindow::showAirportList(QStringList airports)
{
    if (ui->le_departure->hasFocus()) {
        departList->reset();
        departListModel->setStringList(airports);
    }
    else if (ui->le_destination->hasFocus()) {
        destList->reset();
        destListModel->setStringList(airports);
    }
}

void MainWindow::departure_chosen(const QModelIndex& index) {
    ui->le_departure->setText(departList->model()->itemData(index).value(0).toString());
    departList->hide();
    ui->le_destination->setFocus();
}

void MainWindow::destination_chosen(const QModelIndex& index) {
    ui->le_destination->setText(destList->model()->itemData(index).value(0).toString());
    destList->hide();
    ui->le_date->setFocus();
}

void MainWindow::date_chosen() {
    ui->le_date->setText(calendar->selectedDate().toString());
    calendar->hide();
    ui->pb_search->setFocus();
}


void MainWindow::on_le_destination_textChanged(const QString &arg1)
{
    db_handler->getAirportListLike(db_reader, ui->le_destination->text());
}

