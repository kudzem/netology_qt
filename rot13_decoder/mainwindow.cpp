#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./dialog.h"

#include <QMessageBox>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialog = new Dialog();
}

MainWindow::~MainWindow()
{
    delete dialog;
    delete ui;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ROT-13 Decoder",
                                                               tr("you want to exit?\n"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::on_btn_encode_clicked()
{
    QString* snull = nullptr;
    //snull->length();
    QString text_to_encode = ui->pte_src->toPlainText();

    QRadioButton* button7 = dialog->findChild<QRadioButton*>("radioButton");

    if (button7->isChecked()) {
       ui->pte_result->setPlainText(rot13(text_to_encode, 7));
    }
    else{
       ui->pte_result->setPlainText(rot13(text_to_encode, 13));
    }
}


QString MainWindow::rot13( const QString & input, unsigned prm) const
{
    QString r = input;
    int i = r.length();
    while( i-- ) {
        if (!r[i].isLetter()) continue;

        char c = r[i].toLatin1();

        if (c > 'A' && c <= 'M' ||
            c > 'a' && c <= 'm')
        {
            r[i] = QChar(c + prm);
        }
        else
        {
            r[i] = QChar(c - prm);
        }
    }
    return r;
}



void MainWindow::on_btn_clear_clicked()
{
    ui->pte_src->clear();
}


void MainWindow::on_btn_settings_clicked()
{
    dialog->show();
}

