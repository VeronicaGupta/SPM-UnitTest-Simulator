#ifndef PHASECALIBRATIONAUTOTEST_H
#define PHASECALIBRATIONAUTOTEST_H

#include <QObject>
#include <metercalibration.h>
#include "powersource.h"
#include <QDebug>
#include <QTimer>

class PhaseCalibrationAutoTest
{
public:
    explicit PhaseCalibrationAutoTest(QString portName, int N, QString s_portname, QObject *parent = nullptr);

    enum TEST_RESULT{
        TEST_RESULT_IDEAL,
        TEST_RESULT_RUNNING,
        TEST_RESULT_PASS,
        TEST_RESULT_FAIL
    };

    typedef QList<TEST_RESULT> TestResults;


public slots:
    void startTest();
    void restartTest();
    void powerStablised();

    void phaseCalibrationPass();
    void phaseCalibrationFail();

private:
    void processTestResult();
    void setAllTestResult(TEST_RESULT result);

signals:
    void calibrationAutoTestComplete(TestResults results);

private:
    PowerSource* powerSource;
    MeterCalibration* phaseCalibration;

    TEST_RESULT phaseCalibrationResult;

    QDateTime time;

    int passCounter;
    int failCounter;
    int testCounter;

    QString PORTNAME;
    qint32 MAXTESTS;

    QString tag;

    QString testUpdate="";
};

#endif // PHASECALIBRATIONAUTOTEST_H
