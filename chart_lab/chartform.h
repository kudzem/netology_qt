#ifndef CHARTFORM_H
#define CHARTFORM_H

#include <QWidget>
#include <QChartView>
#include <QGridLayout>

namespace Ui {
class ChartForm;
}

class ChartForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChartForm(QWidget *parent = nullptr);
    ~ChartForm();

public slots:

    void show_chart(QChartView* chart);

private slots:
    void on_ChartForm_destroyed();

private:
    Ui::ChartForm *ui;
    QGridLayout *layout;
    QChartView* chartView;
};

#endif // CHARTFORM_H
