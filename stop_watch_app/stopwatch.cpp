#include "stopwatch.h"

StopWatch::StopWatch(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);
    timer->setInterval(50);

    connect(timer, &QTimer::timeout, this, handleTmo);
}

void
StopWatch::handleTmo() {
    interval_counter++;
    notifyTmoExpired(MyWatchTime(interval_counter*timer->interval()));
}

void
StopWatch::start() {
    timer->start();
}

void
StopWatch::stop() {
    timer->stop();
    interval_counter=0;
    freezed_counter=0;
}

void
StopWatch::reset() {
    stop();
    round_number=0;
}

MyWatchRound
StopWatch::get_round() {
    round_number++;
    MyWatchRound round((interval_counter-freezed_counter)*timer->interval(), round_number);
    freezed_counter = interval_counter;
    return round;
}

