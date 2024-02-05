#ifndef FUNCTIONAUTOTEST_H
#define FUNCTIONAUTOTEST_H

#include <QObject>
#include <QDebug>
#include "functiontest.h"

class FunctionAutoTest : public QObject
{
    Q_OBJECT
public:
    explicit FunctionAutoTest(QString portName, int N, QObject *parent = nullptr);

public slots:
    void unitTest();

private slots:
    void functionTestResult(FunctionTestResult* result);

signals:
    void testComplete();
    void unitTestUpdate(QString testUpdate);

private:
    FunctionTest* functionTest;

    int passCounter;
    int failCounter;
    int testCounter;
    QString PORTNAME;
    qint32 MAXTESTS;
    QString TESTNAME;
    QString tag;

    QString testUpdate="";
};

#endif // FUNCTIONAUTOTEST_H
