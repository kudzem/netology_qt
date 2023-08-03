#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stopWatch = new StopWatch();
    connect(stopWatch, &StopWatch::notifyTmoExpired, this, &MainWindow::update_time);
}

MainWindow::~MainWindow()
{
    delete stopWatch;
    delete ui;
}

void MainWindow::update_time(MyWatchTime time)
{
    ui->label->setText(time.toString());
}



void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "Старт") {
        ui->pushButton->setText("Стоп");
        ui->pushButton_2->setEnabled(true);
        stopWatch->timer->start();
    }
    else
    {
        ui->pushButton->setText("Старт");
        ui->pushButton_2->setEnabled(false);
        stopWatch->timer->stop();
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    stopWatch->reset();
    ui->pushButton->setText("Старт");
    ui->pushButton_2->setEnabled(false);
    ui->label->setText("00s00");
    ui->plainTextEdit->clear();
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->plainTextEdit->appendPlainText(stopWatch->get_round().toString());
}

