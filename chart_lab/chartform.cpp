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
    if (_chartView)
    layout->removeWidget(_chartView);
    delete layout;
    delete ui;
}

void
ChartForm::show_chart(QChartView* chartView)
{
    _chartView = chartView;
    layout->addWidget(chartView);
    chartView->show();
    show();
}
