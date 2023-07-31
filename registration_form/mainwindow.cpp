#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList distances { "Choose distance", "500m", "1km", "5km", "10km", "15km", "Half-marathon", "Marathon", "Ultra-marathon" };
    ui->comboBox->addItems(distances);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(6);

    this->setWindowTitle("Registration to our Run event");

    ui->pushButton->setText("Register");

    setFixedSize(size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_progress_bar()
{
    unsigned bp_value = 0;

    if(ui->lineEdit->text() != "") {
        bp_value += 1;
    }
    if(ui->lineEdit_2->text() != "") {
        bp_value += 1;
    }
    if(ui->lineEdit_3->text() != "") {
        bp_value += 1;
    }

    if(ui->radioButton->isChecked() ||
       ui->radioButton_2->isChecked()) {
        bp_value += 1;
    }

    if(ui->dateEdit->text() != "01.01.2000") {
        bp_value += 1;
    }

    if(ui->comboBox->itemText(ui->comboBox->currentIndex()) != "Choose distance") {
        bp_value += 1;
    }

    ui->progressBar->setValue(bp_value);

    ui->pushButton->setEnabled(bp_value == ui->progressBar->maximum());

    //ui->pushButton->toggled(true);

}



void MainWindow::on_lineEdit_editingFinished()
{
    update_progress_bar();
}


void MainWindow::on_lineEdit_2_editingFinished()
{
    update_progress_bar();
}


void MainWindow::on_lineEdit_3_editingFinished()
{
    update_progress_bar();
}


void MainWindow::on_radioButton_clicked()
{
    update_progress_bar();
}


void MainWindow::on_radioButton_2_clicked(bool checked)
{
    update_progress_bar();
}


void MainWindow::on_dateEdit_editingFinished()
{
    update_progress_bar();
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    update_progress_bar();
}


void MainWindow::on_pushButton_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::information(this, "Registration passed",
                                                                   tr("Registration completed successfully!\n"),
                                                                   QMessageBox::Ok);
    close();
}


void MainWindow::on_pushButton_2_clicked()
{
    close();
}

