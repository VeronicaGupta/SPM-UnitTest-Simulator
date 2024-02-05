#ifndef POWERSTABLISATIONAUTOTEST_H
#define POWERSTABLISATIONAUTOTEST_H

#include <QDebug>
#include <QTimer>
#include "powersource.h"

class PowerStablisationAutoTest : QObject
{
    Q_OBJECT
public:
    explicit PowerStablisationAutoTest(QString psPortName, int N, QString cmriPortName, QStringList voltages, QStringList Currents, QObject *parent = nullptr);


public slots:
    void unitTest();

private slots:
    void StablisationTestResult();

signals:
    void testComplete();
    void unitTestUpdate(QString testUpdate);

private:
    PowerSource* powerSource;

    QDateTime time;

    int passCounter;
    int failCounter;
    int testCounter;
    QString PORTNAME;
    qint32 MAXTESTS;
    QString TESTNAME;
    QString tag;

    QString testUpdate="";

    int sourceStabliseDuration;
    QDateTime testStartTime;

    QStringList voltages;
    QStringList currents;

};

#endif // POWERSTABLISATIONAUTOTEST_H
