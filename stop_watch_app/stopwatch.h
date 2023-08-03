#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>

class MyWatchTime {
    int sec = 0;
    int millisec = 0;

public:
    MyWatchTime(int millisec) : sec(millisec/1000), millisec(millisec%1000) {}

    QString toString() {
        QString res = "";
        res = QString::number(sec) + "s " + QString::number(millisec);
        return res;
    }
};

class MyWatchRound {
    MyWatchTime roundTime;
    int roundNumber;

public:
    MyWatchRound(int millisec, int round_number) : roundTime(millisec), roundNumber(round_number) {}

    QString toString() {
        QString res = "";
        res += "Круг ";
        res += QString::number(roundNumber);
        res += ", Время: ";
        res += roundTime.toString();
        return res;
    }
};

class StopWatch : public QObject
{
    Q_OBJECT
public:
    explicit StopWatch(QObject *parent = nullptr);

    void start();
    void stop();
    void reset();
    MyWatchRound get_round();

    QTimer* timer;

public slots:

    void handleTmo();

signals:

    void notifyTmoExpired(MyWatchTime);

private:

    int interval_counter = 0;
    int freezed_counter = 0;
    int round_number = 0;

};

#endif // STOPWATCH_H
