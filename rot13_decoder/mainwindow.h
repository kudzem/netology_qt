#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

#include "./dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btn_encode_clicked();

    void on_btn_clear_clicked();


    void on_btn_settings_clicked();

private:
    Ui::MainWindow *ui;
    Dialog* dialog;

    QString rot13( const QString & input, unsigned prm ) const;

    void closeEvent (QCloseEvent *event);
};
#endif // MAINWINDOW_H
