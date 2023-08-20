#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    retryConnection = new QTimer(this);
    retryConnection->setInterval(5000);

    status_bar = new QStatusBar(this);
    ui->gridLayout_3->addWidget(status_bar);
    status_bar->showMessage("Не подключено к базе");
    status_bar->setStyleSheet("color:red");

    ui->le_departure->installEventFilter(this);
    ui->le_destination->installEventFilter(this);
    ui->le_date->installEventFilter(this);
    ui->pb_search->installEventFilter(this);
    ui->le_airport_name->installEventFilter(this);

    db_reader = new database_reader;
    connect(db_reader, &database_reader::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);

    db_handler = new db_info_handler;

    departList = new QListView(ui->tab_functions);
    departList->hide();
    departList->installEventFilter(this);

    destList = new QListView(ui->tab_functions);
    destList->hide();
    destList->installEventFilter(this);

    airportStatList = new QListView(ui->tab_functions);
    airportStatList->hide();
    airportStatList->installEventFilter(this);

    calendar = new QCalendarWidget(ui->tab_functions);
    calendar->hide();
    calendar->installEventFilter(this);
    calendar->setMinimumDate(QDate(2016, 8, 15));
    calendar->setMaximumDate(QDate(2017, 9, 14));

    //ui->le_date->setText(calendar->selectedDate().toString());
    ui->le_departure->setFocus();

    connect(departList, &QListView::clicked, this, &MainWindow::departure_chosen);
    connect(destList, &QListView::clicked, this, &MainWindow::destination_chosen);
    connect(airportStatList, &QListView::clicked, this, &MainWindow::stat_airport_chosen);

    connect(calendar, &QCalendarWidget::selectionChanged, this, &MainWindow::date_chosen);

    departListModel = new QStringListModel(this);
    departList->setModel(departListModel);
    departList->setGeometry(ui->le_departure->geometry().left(),
                            ui->le_departure->geometry().bottom() + ui->tab_functions->geometry().bottom(),
                            ui->le_departure->geometry().width(),
                            10*ui->le_departure->geometry().height());

    destListModel = new QStringListModel(this);
    destList->setModel(destListModel);

    destList->setGeometry(ui->le_destination->geometry().left() + ui->tab_functions->geometry().left(),
                            ui->le_destination->geometry().bottom() + ui->tab_functions->geometry().bottom(),
                            ui->le_destination->geometry().width(),
                            10*ui->le_destination->geometry().height());

    airportStatListModel = new QStringListModel(this);
    airportStatList->setModel(airportStatListModel);

    airportStatList->setGeometry(ui->le_airport_name->geometry().left() + ui->tab_functions->geometry().left(),
                                 ui->le_airport_name->geometry().bottom() + ui->tab_functions->geometry().bottom(),
                                 ui->le_airport_name->geometry().width(),
                                 10*ui->le_airport_name->geometry().height());

    calendar->setGeometry(ui->le_date->geometry().left() + ui->tab_functions->geometry().left(),
                          ui->le_date->geometry().bottom() + ui->tab_functions->geometry().bottom(),
                          300,
                          200);

    connect(db_handler, &db_info_handler::sig_sendAirportList, this, &MainWindow::showAirportList);
    connect(db_handler, &db_info_handler::sig_sendFlightList, this, &MainWindow::showRelevantFlights);


    initMonthlyChart();
    initDailyChart();

    monthSelector = new QComboBox(this);
    ui->lo_for_daily->addWidget(monthSelector);

    ui->lo_for_chart->addWidget(monthly_view);
    ui->lo_for_daily->addWidget(daily_view);


    QStringList months = {"Январь", "Ферваль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь" };
    monthSelector->addItems(months);

    connect(db_handler, &db_info_handler::sig_sendMonthDepartureStat, this, MainWindow::showMonthlyDepartureStats);
    connect(db_handler, &db_info_handler::sig_sendMonthArrivalStat, this, MainWindow::showMonthlyArrivalStats);
    connect(db_handler, &db_info_handler::sig_sendDayDepartureStat, this, MainWindow::showDailyDepartureStats);
    connect(db_handler, &db_info_handler::sig_sendDayArrivalStat, this, MainWindow::showDailyArrivalStats);

    connect(monthSelector, &QComboBox::currentIndexChanged, this, MainWindow::monthSelected);

    connect(retryConnection, &QTimer::timeout, db_reader, &database_reader::ConnectToDB);

    setEnabledWidgets(false);

    db_reader->ConnectToDB();

    //ui->le_departure->ver
}

MainWindow::~MainWindow()
{
    delete db_reader;
    delete ui;
}

void MainWindow::setEnabledWidgets(bool flag){
    ui->pb_search->setEnabled(flag);
    ui->le_departure->setEnabled(flag);
    ui->le_destination->setEnabled(flag);
    ui->le_date->setEnabled(flag);

    ui->le_airport_name->setEnabled(flag);
    ui->pb_show_load->setEnabled(flag);

    monthSelector->setEnabled(flag);
}


void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status)
    {
        status_bar->showMessage("Подключено к базе");
        status_bar->setStyleSheet("color:green");
        setEnabledWidgets(true);
    }
    else
    {
        status_bar->showMessage("Не подключено к базе");
        status_bar->setStyleSheet("color:red");
        setEnabledWidgets(false);
        notifyNotConnected();
    }
}

void MainWindow::closeEvent(QCloseEvent* event){
    isClosed = true;
}

void MainWindow::notifyNotConnected ()
{
    if (this->isClosed) return;

    retryConnection->stop();

    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Инспектор аэропортов. Ошибка подключени к базе",
                                                               tr("Хотите попробовать подключиться еще раз?\n"),
                                                               QMessageBox::Close | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes)
    {
        retryConnection->start();
    }
    else if(resBtn == QMessageBox::No)
    {
    }
    else
    {
        exit(1);
    }
}

void MainWindow::on_pb_search_clicked()
{
    QString departureAirport = ui->le_departure->text();
    QString destinationAirport = ui->le_destination->text();

    if (ui->le_date->text() != "")
    {
        db_handler->getFlights(db_reader,
                               departureAirport,
                               destinationAirport,
                               calendar->selectedDate());
    }
    else
    {
        db_handler->getFlights(db_reader,
                               departureAirport,
                               destinationAirport);
    }
}


void MainWindow::on_le_departure_textChanged(const QString &arg1)
{
    QString departureFrom = ui->le_departure->text();
    db_handler->getAirportListLike(db_reader, departureFrom);
}

// Implementation
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = (QKeyEvent*)event;
        if (keyEvent->key() == Qt::Key_Down) {
            if(ui->le_departure->hasFocus()) {
                departList->setFocus();
            }
            else if (ui->le_destination->hasFocus()) {
                destList->setFocus();
            }
            else if (ui->le_airport_name->hasFocus()) {
                airportStatList->setFocus();
            }
            else if (ui->le_date->hasFocus()) {
                calendar->setFocus();
            }
        }
        else if (keyEvent->key() == Qt::Key_Escape) {
            if(ui->le_departure->hasFocus() || departList->hasFocus()) {
                departList->hide();
                ui->le_departure->setFocus();
            }
            else if (ui->le_destination->hasFocus() || destList->hasFocus()) {
                destList->hide();
                ui->le_destination->setFocus();
            }
            else if (ui->le_airport_name->hasFocus() || airportStatList->hasFocus()) {
                airportStatList->hide();
                ui->le_airport_name->setFocus();
            }
            else if (ui->le_date->hasFocus() || calendar->hasFocus()) {
                calendar->hide();
                ui->le_date->setFocus();
            }

        }
        else if (keyEvent->key() == Qt::Key_Return)
        {
            if(departList->hasFocus()) {
                departure_chosen(departList->currentIndex());
            }
            else if (destList->hasFocus()) {
                destination_chosen(destList->currentIndex());
            }
            else if (airportStatList->hasFocus()) {
                stat_airport_chosen(airportStatList->currentIndex());
            }
            else if (calendar->hasFocus()) {
                date_chosen();
            }
        }
    }
    else if( event->type() == QEvent::FocusIn )
    {
        if(watched == ui->le_airport_name || watched == airportStatList)
        {
            QString airport = ui->le_airport_name->text();

            if(airport == "Любой") {
                airport = "";
            }

            if (ui->le_airport_name->text() == "Любой") {
                ui->le_airport_name->setText("");
            }

            db_handler->getAirportListLike(db_reader, airport);
            airportStatList->show();
        }
        else if(watched == ui->le_departure || watched == departList)
        {
            QString airport = ui->le_departure->text();
            if(airport == "Любой") {
                airport = "";
            }

            if (ui->le_departure->text() == "Любой") {
                ui->le_departure->setText("");
            }

            db_handler->getAirportListLike(db_reader, airport);
            departList->show();
            calendar->hide();
        }
        else if(watched == ui->le_destination || watched == destList)
        {
            QString airport = ui->le_destination->text();
            if(airport == "Любой") {
                airport = "";
            }

            if (ui->le_destination->text() == "Любой") {
                ui->le_destination->setText("");
            }

            db_handler->getAirportListLike(db_reader, airport);
            destList->show();
            calendar->hide();
        }
        else if(watched == ui->le_date || watched == calendar)
        {
            calendar->show();
        }
        else if(watched == ui->pb_search)
        {
            calendar->hide();
        }
    }
    else if( event->type() == QEvent::FocusOut )
    {
        if(watched == ui->le_departure)
        {
            if (!departList->hasFocus()) {
                departList->hide();

                if (ui->le_departure->text() == "") {
                    ui->le_departure->setText("Любой");
                }
            }
        }
        else if(watched == ui->le_destination)
        {
            if (!destList->hasFocus()) {
                destList->hide();
            }

            if (ui->le_destination->text() == "") {
                ui->le_destination->setText("Любой");
            }
        }
        else if(watched == ui->le_airport_name)
        {
            if (!airportStatList->hasFocus()) {
                airportStatList->hide();
            }

            if (ui->le_airport_name->text() == "") {
                ui->le_airport_name->setText("Любой");
            }
        }
        else if(watched == ui->le_date)
        {
            //calendar->hide();
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
    else if (ui->le_airport_name->hasFocus()) {
        airportStatList->reset();
        airportStatListModel->setStringList(airports);
    }
}

void MainWindow::showRelevantFlights(QAbstractItemModel *model)
{
    ui->tv_flights->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv_flights->setModel(model);
    ui->tv_flights->show();

    // faster alternative to hide columns
    //    ui->tv_result->setColumnHidden(0,true);
    //    for(int i=3; i <=14 ; ++i) {
    //    ui->tv_result->setColumnHidden(3,true);
    //    }
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

void MainWindow::stat_airport_chosen(const QModelIndex& index) {
    ui->le_airport_name->setText(airportStatList->model()->itemData(index).value(0).toString());
    airportStatList->hide();
    ui->pb_show_load->setFocus();
}

void MainWindow::date_chosen() {
    ui->le_date->setText(calendar->selectedDate().toString());
    calendar->hide();
    ui->pb_search->setFocus();
}


void MainWindow::on_le_destination_textChanged(const QString &arg1)
{
    QString destinationTo = ui->le_destination->text();
    db_handler->getAirportListLike(db_reader, destinationTo);
}


void MainWindow::on_pushButton_clicked()
{
    QString tmp = ui->le_departure->text();

    ui->le_departure->setText(ui->le_destination->text());
    ui->le_destination->setText(tmp);

    on_pb_search_clicked();
}


void MainWindow::on_pb_search_pressed()
{
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->le_date->setText("");
    calendar->hide();
}


void MainWindow::on_le_airport_name_textChanged(const QString &arg1)
{
    QString destinationTo = ui->le_airport_name->text();
    db_handler->getAirportListLike(db_reader, destinationTo);
}

void MainWindow::monthSelected(int index) {

    //daily_view->hide();
    QString airport = ui->le_airport_name->text();

    db_handler->getFlightStatDaily(db_reader, airport, index + 1, true);
    db_handler->getFlightStatDaily(db_reader, airport, index + 1, false);
}


void MainWindow::on_pb_show_load_clicked()
{

    QString airport = ui->le_airport_name->text();

    if (ui->tabWidget_2->tabText(ui->tabWidget_2->currentIndex()) == "Ежемесячно")
    {
        maxArrivalsPerMonth = 0;
        db_handler->getFlightStatMonthly(db_reader,airport,true);
        db_handler->getFlightStatMonthly(db_reader,airport,false);
    }
    else if (ui->tabWidget_2->tabText(ui->tabWidget_2->currentIndex()) == "Ежедневно")
    {
        maxArrivalsPerDay = 0;
        db_handler->getFlightStatDaily(db_reader, airport, monthSelector->currentIndex() + 1, true);
        db_handler->getFlightStatDaily(db_reader, airport, monthSelector->currentIndex() + 1, false);
    }
}


void MainWindow::initMonthlyChart()
{
    monthly_chart = new QChart();
    monthly_chart->legend()->setVisible(true);
    monthly_chart->setTitle("Загрузка аэропорта по месяцам");

    monthly_arrivals = new QBarSeries();
    monthly_chart->addSeries(monthly_arrivals);

    arv = new QBarSet("Прибытия");
    monthly_arrivals->append(arv);

    dep = new QBarSet("Отправления");
    monthly_arrivals->append(dep);

    monthly_view = new QChartView(monthly_chart);

    QStringList categories {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
    auto axisX = new QBarCategoryAxis;
    axisX->append(categories);
    monthly_chart->addAxis(axisX, Qt::AlignBottom);
    monthly_arrivals->attachAxis(axisX);

    monthly_chart_axisY = new QValueAxis;
    monthly_chart->addAxis(monthly_chart_axisY, Qt::AlignLeft);
    monthly_arrivals->attachAxis(monthly_chart_axisY);
}


void MainWindow::initDailyChart()
{
    daily_chart = new QChart();
    daily_chart->legend()->setVisible(true);
    daily_chart->setTitle("Загрузка аэропорта в");

    daily_departures = new QLineSeries();
    daily_chart->addSeries(daily_departures);
    daily_departures->setName("Ежедневные отправления");

    daily_arrivals = new QLineSeries();
    daily_chart->addSeries(daily_arrivals);
    daily_arrivals->setName("Ежедневные прибытия");

    daily_view = new QChartView(daily_chart);

    daily_chart_axisX = new QValueAxis;
    daily_chart->addAxis(daily_chart_axisX, Qt::AlignBottom);
    daily_arrivals->attachAxis(daily_chart_axisX);
    daily_departures->attachAxis(daily_chart_axisX);
    daily_chart_axisX->setRange(0,31);

    daily_chart_axisY = new QValueAxis;
    daily_chart->addAxis(daily_chart_axisY, Qt::AlignLeft);
    daily_arrivals->attachAxis(daily_chart_axisY);
    daily_departures->attachAxis(daily_chart_axisY);
}

void MainWindow::showMonthlyArrivalStats(QVector<double>& arrivals_per_month)
{
    arv->remove(0,12);
    arv->append(arrivals_per_month);

    monthly_view->update();
    maxArrivalsPerMonth = FindMax(arrivals_per_month);
    monthly_chart->setTitle("Загрузка аэропорта " + ui->le_airport_name->text() + " по месяцам");
}

void MainWindow::showMonthlyDepartureStats(QVector<double>& departures_per_month)
{
    dep->remove(0,12);
    dep->append(departures_per_month);

    double maxDeparturesPerMonth = FindMax(departures_per_month);
    monthly_chart_axisY->setRange(0,  std::max(maxDeparturesPerMonth, maxArrivalsPerMonth) + 1);

    monthly_view->update();
}


void MainWindow::showDailyArrivalStats(QVector<double>& arrivals_per_day)
{
    daily_arrivals->clear();

    size_t s = arrivals_per_day.size();

    int i = 0;
    foreach(double num, arrivals_per_day)
    {
        daily_arrivals->append(i+1, arrivals_per_day[i]);
        ++i;
    }

    daily_chart_axisX->setRange(1, arrivals_per_day.size());
    maxArrivalsPerDay = FindMax(arrivals_per_day);
    daily_chart->setTitle("Загрузка аэропорта " + ui->le_airport_name->text() + ". " + monthSelector->itemText(monthSelector->currentIndex()));
    daily_view->update();
}

void MainWindow::showDailyDepartureStats(QVector<double>& departures_per_day)
{
    daily_departures->clear();

    size_t s = departures_per_day.size();

    int i = 0;
    foreach(double num, departures_per_day)
    {
        daily_departures->append(i+1, departures_per_day[i]);
        ++i;
    }

    daily_chart_axisX->setRange(1, departures_per_day.size());

    double maxDeparturesPerDay = FindMax(departures_per_day);
    daily_chart_axisY->setRange(0,  std::max(maxDeparturesPerDay, maxArrivalsPerDay) + 1);
    daily_view->update();
}

double MainWindow::FindMax(QVector<double>& data)
{
    double max = 0;
    foreach (double num, data){
        if(num > max){
            max = num;
        }
    }

    return max;
}


void MainWindow::on_pushButton_3_clicked()
{
    //ui->pushButton_3->setText()
}


void MainWindow::on_tab_functions_currentChanged(int index)
{

}


void MainWindow::on_le_departure_textEdited(const QString &arg1)
{
    departList->show();
}


void MainWindow::on_le_destination_textEdited(const QString &arg1)
{
    destList->show();
}


void MainWindow::on_le_airport_name_textEdited(const QString &arg1)
{
    airportStatList->show();
}

