#ifndef ERRORAUTOTEST_H
#define ERRORAUTOTEST_H

#include <QObject>
#include <QDateTime>
#include "powersource.h"
#include "metererrortest.h"

class ErrorAutoTest : public QObject
{
    Q_OBJECT
public:
    explicit ErrorAutoTest(QObject *parent = nullptr);

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

    void processTestResult();
    void lowCurrentTestResult(ErrorTestResult *);
    void highCurrentTestResult(ErrorTestResult *);

signals:

private:
    PowerSource* powerSource;
    MeterErrorTest* lowCurrentErrorTest;
    MeterErrorTest* highCurrentErrorTest;

    QDateTime startTime;

    TEST_RESULT lowCurrentErrorTestResult;
    TEST_RESULT highCurrentErrorTestResult;

    int passCounter;
    int failCounter;
};

#endif // ERRORAUTOTEST_H
