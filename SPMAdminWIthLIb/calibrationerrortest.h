#ifndef CALIBRATIONERRORTEST_H
#define CALIBRATIONERRORTEST_H

#include <QObject>
#include <QDateTime>
#include "powersource.h"
#include "metercalibration.h"
#include "metererrortest.h"

#define PHASE_CALIBRATION_TEST      (1<<1)
#define NEUTRAL_CALIBRATION_TEST    (1<<2)
#define LOW_CURRENT_TEST            (1<<3)
#define HIGH_CURRENT_TEST           (1<<4)

class CalibrationErrorTest : public QObject
{
    Q_OBJECT
public:
    explicit CalibrationErrorTest(QObject *parent = nullptr);

    enum TEST_RESULT{
        TEST_RESULT_IDEAL,
        TEST_RESULT_NOT_APPLICABLE,
        TEST_RESULT_RUNNING,
        TEST_RESULT_PASS,
        TEST_RESULT_FAIL
    };
    typedef QList<TEST_RESULT> TestResults;

public slots:
    void startTest();
    void startTest(int testsToStart);
    void powerStablised();

    void phaseCalibrationPass();
    void phaseCalibrationFail();
    void neutralCalibrationPass();
    void neutralCalibrationFail();
    void lowCurrentTestPass();
    void lowCurrentTestFail();
    void highCurrentTestPass();
    void highCUrrentTestFail();

signals:
    void calibrationErrorTestResult(TestResults results);

private:
    void processTestResult();
    void setAllTestResult(TEST_RESULT result);

private:
    PowerSource* powerSource;
    MeterCalibration* phaseCalibration;
    MeterCalibration* neutralCalibration;
    MeterErrorTest* lowCurrentErrorTest;
    MeterErrorTest* highCurrentErrorTest;

    TEST_RESULT phaseCalibrationResult;
    TEST_RESULT neutralCalibrationResult;
    TEST_RESULT lowCurrentErrorTestResult;
    TEST_RESULT highCurrentErrorTestResult;

    int testsToStart;

    QDateTime startTestTime;
    QString tag;
};

#endif // CALIBRATIONERRORTEST_H
