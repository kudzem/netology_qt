#include "chartform.h"
#include "ui_chartform.h"
#include <QChartView>

ChartForm::ChartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartForm)
{
    ui->setupUi(this);
    layout = new QGridLayout;
    ui->widget->setLayout(layout);
}

ChartForm::~ChartForm()
{
    if (chartView != NULL) {
        layout->removeWidget(chartView);
    }
    delete layout;
    delete ui;
}

void
ChartForm::show_chart(QChartView* _chartView)
{
    if(!chartView) {
    chartView = _chartView;
    layout->addWidget(chartView);
    chartView->show();
    }
    show();
}

void ChartForm::on_ChartForm_destroyed()
{
}

