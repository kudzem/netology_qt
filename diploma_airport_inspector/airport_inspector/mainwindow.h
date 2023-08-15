#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "database_reader.h"

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

private:
    Ui::MainWindow *ui;

    database_reader* db_reader;
};
#endif // MAINWINDOW_H
