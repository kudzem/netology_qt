#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sb_initNum->setValue(200000);
    ui->rb_synchOff->setChecked(true);

   race1 = new Controller(&m);
   race2 = new Controller(&m);

   concurRace1 = new ExampleRace(&m);
   concurRace2 = new ExampleRace(&m);

    //Сигналы по завершению работы потоков
    connect(race1, &Controller::sig_WorkFinish, this, &MainWindow::show_result);
    connect(race2, &Controller::sig_WorkFinish, this, &MainWindow::show_result);

    connect(concurRace1, &ExampleRace::sig_Finish, this, &MainWindow::show_result);
    connect(concurRace2, &ExampleRace::sig_Finish, this, &MainWindow::show_result);
}

void
MainWindow::show_result()
{
    if(countFinish == 0){
        countFinish++;
    }
    else{
        ui->te_debug->append("Искомое число равно: " + QString::number(number) + ", а должно быть " +
                                 QString::number(ui->sb_initNum->value()*2));
        ui->pb_start->setEnabled(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Метод запускает два потока
void MainWindow::StartRace(void){


    if(ui->rb_qtConcur->isChecked()){
        auto sum = [&](){ concurRace1->DoWork(&number,false,ui->sb_initNum->value()); };
        QtConcurrent::run(sum).then(sum);
    }
    else{
        emit race1->operate(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());
        emit race2->operate(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());
    }
}

//Обработка кнопки "Старт"
void MainWindow::on_pb_start_clicked()
{
    ui->pb_start->setEnabled(false);
    countFinish = 0;
    number = 0;
    StartRace( );
}

