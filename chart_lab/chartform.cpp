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
    chartView = nullptr;
}

ChartForm::~ChartForm()
{
    layout->removeWidget(chartView);
    delete layout;
    delete ui;
}

void
ChartForm::show_chart(QChart* chart)
{
    if (chartView) delete chartView;
    chartView = new QChartView(chart);
    layout->addWidget(chartView);

    chartView->show();
    show();
}
