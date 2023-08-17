#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QCalendarWidget>
#include <QStringListModel>

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

    void departure_chosen(const QModelIndex& index);
    void destination_chosen(const QModelIndex& index);
    void date_chosen();

    void on_le_destination_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pb_search_pressed();

    void on_pushButton_2_clicked();

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
};
#endif // MAINWINDOW_H
