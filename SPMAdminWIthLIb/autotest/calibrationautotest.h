#ifndef CALIBRATIONAUTOTEST_H
#define CALIBRATIONAUTOTEST_H

#include <QObject>
#include <metercalibration.h>
#include "powersource.h"

class CalibrationAutoTest : public QObject
{
    Q_OBJECT
public:
    explicit CalibrationAutoTest(QObject *parent = nullptr);

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
    void startTest(int testsToStart);
    void powerStablised();

    void phaseCalibrationPass();
    void phaseCalibrationFail();
    void neutralCalibrationPass();
    void neutralCalibrationFail();
    void bothCalibrationE1Pass();
    void bothCalibrationE1Fail();
    void bothCalibrationE2Pass();
    void bothCalibrationE2Fail();

private:
    void processTestResult();
    void setAllTestResult(TEST_RESULT result);

signals:
    void calibrationAutoTestComplete(TestResults results);

private:
    PowerSource* powerSource;
    MeterCalibration* phaseCalibration;
    MeterCalibration* neutralCalibration;
    MeterCalibration* bothCalibrationE1;
    MeterCalibration* bothCalibrationE2;

    TEST_RESULT phaseCalibrationResult;
    TEST_RESULT neutralCalibrationResult;
    TEST_RESULT bothCurrentE1TestResult;
    TEST_RESULT bothCurrentE2TestResult;
};

#endif // CALIBRATIONAUTOTEST_H
