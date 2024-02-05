#ifndef FUNCTIONAUTOTEST_H
#define FUNCTIONAUTOTEST_H

#include <QObject>
#include <QDateTime>
#include "functiontest.h"

class FunctionAutoTest : public QObject
{
    Q_OBJECT
public:
    explicit FunctionAutoTest(QString portName, QObject *parent = nullptr);

public slots:
    void startTest();
    void restartTest();

    void functionTestResult(FunctionTestResult* result);

signals:

private:
    FunctionTest* functionTest;
    QDateTime time;
    int passCounter;
    int failCounter;

};

#endif // FUNCTIONAUTOTEST_H
