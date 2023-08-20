#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QCalendarWidget>
#include <QStringListModel>
#include <QtCharts>

#include "database_reader.h"
#include "db_info_handler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void ReceiveStatusConnectionToDB(bool status);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_search_clicked();

    void on_le_departure_textChanged(const QString &arg1);

    void showAirportList(QVector<QString> airports);
    void showRelevantFlights(QAbstractItemModel *model);
    void showMonthlyArrivalStats(QList<double>& arrivals_per_month);
    void showMonthlyDepartureStats(QList<double>& departures_per_month);
    void showDailyArrivalStats(QList<double>& arrivals_per_day);
    void showDailyDepartureStats(QList<double>& departures_per_day);

    void departure_chosen(const QModelIndex& index);
    void destination_chosen(const QModelIndex& index);
    void date_chosen();

    void stat_airport_chosen(const QModelIndex& index);

    void on_le_destination_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pb_search_pressed();

    void on_pushButton_2_clicked();

    void on_le_airport_name_textChanged(const QString &arg1);

    void on_pb_show_load_clicked();

    void monthSelected(int index);

private:
    Ui::MainWindow *ui;

    database_reader* db_reader;

    db_info_handler* db_handler;

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    QListView* departList;
    QStringListModel* departListModel;

    QListView* destList;
    QStringListModel* destListModel;

    QCalendarWidget* calendar;

    QStatusBar *status_bar;


    QListView* airportStatList;
    QStringListModel* airportStatListModel;

    void initMonthlyChart();
    QChart* monthly_chart;
    QValueAxis* monthly_chart_axisY;
    QBarSeries* monthly_departures;
    QBarSeries* monthly_arrivals;
    QChartView* monthly_view;
    QBarSet* arv;
    QBarSet* dep;

    void initDailyChart();
    QChart* daily_chart;
    QLineSeries* daily_arrivals;
    QLineSeries* daily_departures;
    QChartView* daily_view;
    QValueAxis* daily_chart_axisY;

    QComboBox* monthSelector;

    double FindMax(QList<double>& data);

    void notifyNotConnected();

    QTimer* retryConnection;

    bool isClosed = false;

    void closeEvent(QCloseEvent* event);
};

#endif // MAINWINDOW_H
